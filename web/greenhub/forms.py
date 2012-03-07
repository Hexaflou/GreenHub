from django.forms.models import ModelForm
from greenhub.models import Sensor

class AddSensorForm(ModelForm):
    class Meta:
        model = Sensor
        exclude = ('user', )
    pass
