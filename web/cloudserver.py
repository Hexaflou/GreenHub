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
        print u"[%s] %s > %s" % (datetime.datetime.now().strftime("%d/%m/%y %H:%M:%S"), level.rjust(5), msg.decode("utf-8"))
else:
    def debug(level, msg):
        """Ne fait rien. Cette fonction est utilisée si le mode Debug est désactivé ; pour
        éviter de faire des tests à chaque message, la fonction est définie comme vide
        en début de programme."""
        pass

class GreenhubProtocol(Protocol):
    def __init__(self):
        self.current_buffer = ""

    def computeMessage(self, message):
        msg_type = message["msg_type"]
        debug("INFO", "Traitement d'un message de type \"%s\"" % msg_type)

        if msg_type == "login":
            self.user_id = int(message["user_id"])
            self.factory.clients[self.user_id] = self
            debug("INFO", "Utilisateur #%d loggué." % self.user_id)
            pass

        elif msg_type == "new_state":
            try:
                sensor = Sensor.objects.filter(user__id = self.user_id, mac_address = message["mac_address"])[0]
            except IndexError:
                debug("ERROR", "Aucun sensor connu pour l'adresse MAC %s (user = %s)." % (message["mac_address"], self.user_id))
                return

            state = State(sensor = sensor, value=float(message["new_value"]), date = time.gmtime(int(message["date"])))

            state.save()
            debug("INFO", "Sauvegarde de l'état pour le senseur à l'adresse %s (valeur = %s)" % (mac_address, float(message["new_value"])))

            pass

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
		raise

            index = self.current_buffer.find('}', index)

        pass

    def connectionMade(self):
        debug("INFO", "Nouvelle connection.")

    def connectionLost(self, reason):
        debug("INFO", "Connection perdue.")

class GreenhubFactory(Factory):
    protocol = GreenhubProtocol

    def __init__(self):
        self.clients = {}
        pass

class GProxyProtocol(Protocol):

    def dataReceived(self, data):
        self.current_buffer += data

        index = 0

        index = self.current_buffer.find('}', 0)

        while index != -1:
            try:
                obj = json.loads(self.current_buffer[0:index + 1].strip())

                uid = obj["user_id"]

                try:
                    self.factory.remote.clients[uid].transport.write(json.dumps(obj["message"]))
                except IndexError:
                    pass

                self.current_buffer = self.current_buffer[index + 1:]

                index = 0

            except ValueError:
                debug("WARN", "Impossible d'analyser les données reçues par Django (pas encore assez ?)")

            index = self.current_buffer.find('}', index)

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
