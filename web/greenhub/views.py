# Create your views here.
from django.contrib.auth.decorators import login_required
from django.http import HttpResponse
from django.template import loader
from django.template.context import Context
from greenhub.models import Sensor, Score, Message

def index(request):
    t = loader.get_template('index.html')
    c = Context({ })
    return HttpResponse(t.render(c))

@login_required
def home(request):
    t = loader.get_template('home.html')

    last_score = Score.objects.filter(user=request.user).order_by('calculated_at').reverse()[0]
    sensors = Sensor.objects.filter(user=request.user).all()
    messages = Message.objects.filter(user=request.user).order_by('emitted_at').reverse().all()
    scores = Score.objects.filter(user=request.user).order_by('calculated_at').reverse().all()

    c = Context({
        'login_name': request.user.first_name,
        'sensors': sensors,
        'messages': messages,
        'last_score': last_score,
        'scores': scores,
        'user': request.user })
    return HttpResponse(t.render(c))