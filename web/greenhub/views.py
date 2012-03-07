# Create your views here.
from django.contrib.auth.decorators import login_required
from django.contrib import auth
from django.http import HttpResponse, Http404
from django.template import loader
from django.shortcuts import redirect, render
from django.template.context import Context, RequestContext
from greenhub.forms import AddSensorForm
from greenhub.models import Sensor, Score, Message
import socket, json, time, datetime
from datetime import timedelta

def index(request):
    if request.user.is_authenticated():
        return redirect("greenhub.views.home")

    return render(request, 'index.html', {})

@login_required
def home(request):
    try:
        last_score = Score.objects.filter(user=request.user).order_by('calculated_at').reverse()[0]
    except IndexError:
        last_score = None

    messages = Message.objects.filter(user=request.user).order_by('emitted_at').reverse().all()
    scores = Score.objects.filter(user=request.user).order_by('calculated_at').reverse().all()

    return render(request, "home.html", {
        'login_name': request.user.first_name,
        'sensors': sensors,
        'messages': messages,
        'last_score': last_score,
        'scores': scores,
        'seven_days_ago': datetime.datetime.now() - timedelta(days=7),
        'user': request.user
    })

@login_required
def sensors(request):
    sensors = Sensor.objects.filter(user=request.user).all()

    return render(request, "sensors.html", { "sensors": sensors })

@login_required
def settings(request):
    if request.method == 'POST':
        form = AddSensorForm(request.POST)
        if form.is_valid():
            return redirect("greenhub.views.index")
    else:
        sensor = Sensor()
        sensor.user = request.user
        form = AddSensorForm(instance=sensor)

    return render(request, "settings.html", { 'add_sensors_form': form })

def logout(request):
    auth.logout(request)
    return redirect("greenhub.views.index")

@login_required
def command(request, type=None):
    if type == "lamp-on":
        msg = {"msg_type": "action", "hardware_id": "0021CBE5a03", "value": 5}
    elif type == "lamp-off":
        msg = {"msg_type": "action", "hardware_id": "0021CBE5a03", "value": 7}
    elif type == "real-fast":
        msg = {"msg_type": "realtime", "interval": 5}
    elif type == "real-slow":
        msg = {"msg_type": "realtime", "interval": 30}
    elif type == "real-off":
        msg = {"msg_type": "realtime", "interval": 0}
    elif type == "get-value-test":
        msg = {"msg_type": "last_state", "hardware_id": "0001B015eC"}
    else:
        raise Http404

    try:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)        
        sock.connect("/tmp/greenhub.sock")
        sock.send(json.dumps({ "user_id": request.user.id, "message": msg}))
        sock.close()
    except socket.error:
        raise Http404

    return redirect("greenhub.views.home")


