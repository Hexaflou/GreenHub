import datetime
from sys import stdout
from greenhub.models import  Sensor, State
import settings
import matplotlib.pyplot as plot
import matplotlib.dates as mdates

def create_graph(user,type):

    sensors = Sensor.objects.filter(user=user,type=type).all()
    stdout.write(u'longueur sensors %d\n' % len(sensors))

    if len(sensors) == 0:
        return

    sizex = 500; sizey= 200; mydpi = 80
    fig = plot.figure(figsize=(sizex/mydpi, sizey/mydpi), dpi=mydpi)

    ax = fig.add_subplot(111)
    plot.xlabel('Date')
    plot.ylabel('%s' % type)

    for sensor in sensors :
        states = State.objects.filter(sensor=sensor).order_by('captured_at')\
        .filter(captured_at__gte = datetime.datetime.now() - datetime.timedelta(days=10))

        if not len(states):
            return

        values = []
        dates = []

        for state in states:
            values.append(state.value)
            dates.append(state.captured_at)



        ax.plot(dates, values)
        #ax.xticks(drange(min(score_date), max(score_date), datetime.timedelta(days=1)))
        ax.xaxis.set_major_locator(mdates.DayLocator())
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%a %d'))


        plot.ylim(ymin=min(values) - 10, ymax=max(values) + 10)

    plot.savefig(settings.PROJECT_ROOT + settings.STATIC_URL + "images/graph/%s-%s.png" % (type, user.username))