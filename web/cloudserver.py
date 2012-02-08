from twisted.internet.protocol import Factory, Protocol
from twisted.internet.endpoints import TCP4ServerEndpoint, UNIXServerEndpoint
from twisted.internet import reactor
import time, sys, os.path

from django.core.management import setup_environ
from . import settings

setup_environ(settings)

from greenhub.models import *
import json

class GreenhubProtocol(Protocol):
    def __init__(self):
        self.current_buffer = ""

    def computeMessage(self, message):

        msg_type = message["msg_type"]

        if msg_type == "login":
            self.user_id = int(message["user_id"])
            self.factory.clients[self.user_id] = self
            pass

        elif msg_type == "new_state":

            try:
                sensor = Sensor.objects.filter(user_id = self.user_id, mac_address = message["mac_address"])[0]
            except IndexError:
                return

            state = State(sensor = sensor, value=float(message["new_value"]), date = time.gmtime(int(message["date"])))

            state.save()

            pass

        else:
            # OMG DON'T KNOW x3333
            pass

    def dataReceived(self, data):
        self.current_buffer += data

        index = 0

        index = self.current_buffer.find('}', 0)

        while index != -1:
            try:
                obj = json.loads(self.current_buffer[0:index].strip())

                self.computeMessage(obj)

                self.current_buffer = self.current_buffer[index:]

                index = 0

            except ValueError:
                pass

            index = self.current_buffer.find('}', index)

        pass

class GreenhubFactory(Factory):
    # This will be used by the default buildProtocol to create new protocols:
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
                obj = json.loads(self.current_buffer[0:index].strip())

                uid = obj["user_id"]

                try:
                    self.factory.remote.clients[uid].transport.write(json.dumps(obj["message"]))
                except IndexError:
                    pass

                self.current_buffer = self.current_buffer[index:]

                index = 0

            except ValueError:
                pass

            index = self.current_buffer.find('}', index)

        pass

    pass

class GProxyFactory(Factory):
    protocol = GProxyProtocol

    def __init__(self, remote):
        self.remote = remote

        pass

endpoint = TCP4ServerEndpoint(reactor, 8007)
gfactory = GreenhubFactory()
endpoint.listen(gfactory)

localendpoint = UNIXServerEndpoint(reactor, 9999)
localendpoint.listen(GProxyFactory(gfactory))

reactor.run()
