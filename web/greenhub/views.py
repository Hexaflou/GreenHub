# Create your views here.
from django.contrib.auth.decorators import login_required
from django.http import HttpResponse, Http404
from django.template import loader
from django.shortcuts import redirect, render
from django.template.context import Context, RequestContext
from greenhub.models import Sensor, Score, Message
import socket, json, time, datetime
from datetime import timedelta

def index(request):
    t = loader.get_template('index.html')
    c = RequestContext(request, { })
    return HttpResponse(t.render(c))

@login_required
def home(request):
    t = loader.get_template('home.html')

    last_score = Score.objects.filter(user=request.user).order_by('calculated_at').reverse()[0]
    sensors = Sensor.objects.filter(user=request.user).all()
    messages = Message.objects.filter(user=request.user).order_by('emitted_at').reverse().all()
    scores = Score.objects.filter(user=request.user).order_by('calculated_at').reverse().all()

    c = RequestContext(request, {
        'login_name': request.user.first_name,
        'sensors': sensors,
        'messages': messages,
        'last_score': last_score,
        'scores': scores,
        'seven_days_ago': datetime.datetime.now() - timedelta(days=7),
        'user': request.user })
    return HttpResponse(t.render(c))

@login_required
def command(request, type=None):
    if type == "test":
        msg = {"msg_type": "action", "hardware_id": "48151623eT", "action": "yoshiii"}
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


