# -*- encoding: utf-8 -*-

from django import template
from greenhub.messages import msgs
import json

register = template.Library()

@register.filter
def format_state(value, sensor_type):
    if sensor_type in ('lamp', 'heating'):
        if value:
            return u"Allumé"
        else:
            return u"Eteint"
    elif sensor_type in ('window'):
        if value:
            return u"Ouverte"
        else:
            return u"Fermée"
    elif sensor_type in ('temp'):
        return '%d°C' % round(value)
    elif sensor_type in ('lux'):
        return '%d lux' % round(value)
    else:
        return value

@register.filter
def format_msg(value, data):
    try:
        if len(data):
            data_obj = json.loads(data)

            try:
                return msgs[value] % data_obj
            except IndexError:
                return msgs[value]

        else:
            return msgs[value]
    except KeyError:
        return "{Message inconnu, code = %s}" % value