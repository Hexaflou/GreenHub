#!/usr/bin/env python
# -*- encoding: utf-8 -*-

# Dépandances Twisted
from twisted.internet.protocol import Factory, Protocol
from twisted.internet.endpoints import TCP4ServerEndpoint, UNIXServerEndpoint
from twisted.internet import reactor
import time, sys, os.path

# Dépandances Django
from django.core.management import setup_environ
import settings

setup_environ(settings)

# Dépendances GreenHub
from greenhub.models import *

# Autres dépendances
import json

# Configuration
debug = True

# Utilitaires
if debug:
    def debug(level, msg):
        """Affiche un message de debug sur la sortie standard, d'un niveau de sévérité 
        donné (level, chaîne de moins de cinq caractères)."""
        if type(msg) is not unicode:
            msg = msg.decode("utf-8")
        print u"[%s] %s > %s" % (datetime.datetime.now().strftime("%d/%m/%y %H:%M:%S"), level.rjust(5), msg)
else:
    def debug(level, msg):
        """Ne fait rien. Cette fonction est utilisée si le mode Debug est désactivé ; pour
        éviter de faire des tests à chaque message, la fonction est définie comme vide
        en début de programme."""
        pass

class GreenhubProtocol(Protocol):
    def __init__(self):
        self.current_buffer = ""
        self.user_id = None

    def computeMessage(self, message):
        msg_type = message["msg_type"]
        debug("INFO", "Traitement d'un message de type \"%s\"" % msg_type)

        if msg_type == "login":
            new_user_id = int(message["user_id"])
            if self.user_id is not None and self.user_id in self.factory.clients:
                debug("INFO", "Utilisateur #%d déloggué (se loggue avec le numéro #%d)." % (self.user_id, new_user_id))
                del self.factory.clients[self.user_id]
            self.user_id = new_user_id
            self.factory.clients[self.user_id] = self
            debug("INFO", "Utilisateur #%d loggué." % self.user_id)

        elif msg_type == "new_state":
            try:
                sensor = Sensor.objects.filter(user__id = self.user_id, hardware_id = message["hardware_id"])[0]
            except IndexError:
                debug("ERROR", u"Aucun sensor connu pour l'ID matériel %s (user = %s, valeur = %s)." % (message["hardware_id"], self.user_id, float(message["new_value"])))
                return

            state = State(sensor = sensor, value=float(message["new_value"]), captured_at = datetime.datetime.fromtimestamp(int(message["date"])))

            state.save()
            debug("INFO", u"Sauvegarde de l'état pour le senseur à l'ID %s (user = %s, valeur = %s)" % (message["hardware_id"], self.user_id, float(message["new_value"])))

        else:
            # OMG DON'T KNOW x3333
            pass

    def dataReceived(self, data):
        self.current_buffer += data

        index = 0

        index = self.current_buffer.find('}', 0) # Trouve le premier }

        while index != -1:
            try:
                one_msg = self.current_buffer[0:index + 1].strip()
                debug("DEBUG", "Message reçu : %s" % repr(one_msg))
                obj = json.loads(one_msg)

                self.computeMessage(obj)

                self.current_buffer = self.current_buffer[index + 1:] # Ne garde que de index + 1 à la fin

                index = 0

            except ValueError:
                debug("WARN", "Impossible d'analyser les données reçue (pas encore assez ?)")

            index = self.current_buffer.find('}', index + 1)

        pass

    def connectionMade(self):
        debug("INFO", "Nouvelle connection.")

    def connectionLost(self, reason):
        if self.user_id is not None and self.user_id in self.factory.clients:
            del self.factory.clients[self.user_id]
            debug("INFO", "Connection perdue (de l'utilisateur #%d)." % self.user_id)
        else:
            debug("INFO", "Connection perdue.")


class GreenhubFactory(Factory):
    protocol = GreenhubProtocol

    def __init__(self):
        self.clients = {}
        pass

class GProxyProtocol(Protocol):
    def __init__(self):
        self.current_buffer = ""

    def dataReceived(self, data):
        self.current_buffer += data

        index = 0

        index = self.current_buffer.find('}', 0)

        while index != -1:
            try:
                debug("DEBUG", "Message reçu : %s" % repr(self.current_buffer[0:index + 1].strip()))

                obj = json.loads(self.current_buffer[0:index + 1].strip())

                uid = obj["user_id"]

                try:
                    self.factory.remote.clients[uid].transport.write(json.dumps(obj["message"]))
                    debug("INFO", "Message de type %s envoyé à l'utilisateur #%d." % (obj["message"]["msg_type"], uid))
                except KeyError:
                    debug("ERROR", u"(Proxy) L'utilisateur #%d n'est pas connecté, impossible de lui envoyer le message." % uid)
                    pass

                self.current_buffer = self.current_buffer[index + 1:]

                index = 0

            except ValueError:
                pass # Pas la bonne accolade (JSON imbriqué)

            index = self.current_buffer.find('}', index + 1)

        pass

    pass

class GProxyFactory(Factory):
    protocol = GProxyProtocol

    def __init__(self, remote):
        self.remote = remote
        pass

endpoint = TCP4ServerEndpoint(reactor, 1863)
gfactory = GreenhubFactory()
endpoint.listen(gfactory)

localendpoint = UNIXServerEndpoint(reactor, "/tmp/greenhub.sock")
localendpoint.listen(GProxyFactory(gfactory))

debug("INFO", "Serveur démarré.")
reactor.run()
