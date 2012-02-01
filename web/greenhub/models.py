# -*- coding: utf-8 -*-
import datetime

from django.contrib.auth.models import User
from django.db import models

# Create your models here.
from django.db.models.aggregates import Avg

class Sensor(models.Model):
    name = models.CharField(max_length=200)

    type = models.CharField(max_length=20, choices=((u'temp', u'Température'),
                                                    (u'heating', u'Chauffage'),
                                                    (u'lux', u'Luminosité'),
                                                    (u'lamp', u'Lampe'),
                                                    (u'window', u'Fenêtre')), default='temp')

    description = models.CharField(max_length=200, blank=True)

    mac_address = models.CharField(max_length=20, blank=True)

    user = models.ForeignKey(User)

    def last_state(self):
        return self.state_set.order_by("captured_at").reverse()[0]

    def last_hour_state(self):
        return self.state_set.filter(captured_at__gte = datetime.datetime.now() - datetime.timedelta(hours=1)).aggregate(Avg('value'))['value__avg']

    def __unicode__(self):
        return u"%s - %s" % (self.name, self.description)

class State(models.Model):
    captured_at = models.DateTimeField('Date de capture', default=datetime.datetime.now)
    value = models.FloatField('Valeur')
    sensor = models.ForeignKey(Sensor)

    def __unicode__(self):
        return u"%s - %s (%s)" % (self.captured_at, self.value, self.sensor.name)

class Score(models.Model):
    calculated_at = models.DateTimeField('Date', default=datetime.datetime.now)
    value = models.FloatField('Score')
    user = models.ForeignKey(User)

    def __unicode__(self):
        """
        Retourne une représentation textuelle du score.
        """
        return "%s" % self.value

    def __repr__(self):
        """
        Retourne une représentation de debug du score.
        """
        return "<Score: user=%s value=%s calculated_at=%s>" % (self.user, self.value, self.calculated_at)

class Message(models.Model):
    user = models.ForeignKey(User)

    code = models.CharField('Code', max_length=30)
    data = models.CharField('Données', max_length=1000, blank=True)

    emitted_at = models.DateTimeField('Date de création', auto_now_add=True)

    def __unicode__(self):
        return u"%s : %s" % (self.user, self.code)
