import datetime
import random
from django.contrib.auth.models import User
from django.core.management.base import BaseCommand, CommandError
from greenhub.models import Score, Message, Sensor, State
from greenhub import rules

class Command(BaseCommand):
    args = ''
    help = 'Randomize sensor states.'

    def handle(self, *args, **options):
        try:
            users = User.objects.all()
        except User.DoesNotExist:
            raise CommandError('There is no registred user.')

        for user in users:
            sensors = Sensor.objects.filter(user=user).all()
            for sensor in sensors:
                previous = sensor.last_state()
                if sensor.type == "temp":
                    state = State(sensor=sensor, value=round(sensor.last_state().value + random.random()*6.0-3.0 ,2), date = datetime.datetime.now())
                elif sensor.type == "lux":
                    state = State(sensor=sensor, value=round(sensor.last_state().value + random.random()*20.0-10.0, 2), date = datetime.datetime.now())
                elif sensor.type in ("lamp", "heating"):
                    state = State(sensor=sensor, value=round(random.random()), date = datetime.datetime.now())
                else:
                    state = State(sensor=sensor, value=sensor.last_state().value, date = datetime.datetime.now())

                print "%s's sensor %s changed from %s to %s" % (user, sensor.name, previous.value, state.value)

                state.save()

        self.stdout.write('Successfully randomized !\n')
