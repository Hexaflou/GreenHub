import datetime
from numpy.core.numeric import arange

from django.contrib.auth.models import User
from django.core.management.base import BaseCommand, CommandError
from matplotlib.dates import drange
from greenhub.graph import create_graph
from greenhub.models import Score, Sensor, State
import settings
import matplotlib.pyplot as plot
import matplotlib.dates as mdates


class Command(BaseCommand):
    args = ''
    help = 'Generate a score graph for all users.'

    def handle(self, *args, **options):
        try:
            users = User.objects.all()
        except User.DoesNotExist:
            raise CommandError('There is no registred user.')

        for user in users:

            create_graph(user,"temp")
            #create_graph(user,"lux")
            #create_graph(user,"heating")
            #create_graph(user,"lamp")

            scores = Score.objects.filter(user=user).order_by('calculated_at')\
                          .filter(calculated_at__gte = datetime.datetime.now() - datetime.timedelta(days=10))

            if not len(scores):
                continue

            score_values = []
            score_date = []

            for score in scores:
                score_values.append(score.value)
                score_date.append(score.calculated_at)

            sizex = 600; sizey= 200; mydpi = 80
            fig = plot.figure(figsize=(sizex/mydpi, sizey/mydpi), dpi=mydpi)

            ax = fig.add_subplot(111)

            ax.plot(score_date, score_values)
            #ax.xticks(drange(min(score_date), max(score_date), datetime.timedelta(days=1)))
            ax.xaxis.set_major_locator(mdates.DayLocator())
            ax.xaxis.set_major_formatter(mdates.DateFormatter('%a %d'))

            plot.xlabel('Date')
            plot.ylabel('Score')
            plot.ylim(ymin=min(score_values) - 10, ymax=max(score_values) + 10)

            plot.savefig(settings.PROJECT_ROOT + settings.STATIC_URL + "images/graph/score-%s.png" % user.username)



            self.stdout.write(u'Graph generated for %s\n' % user)

        self.stdout.write('Done !\n')

