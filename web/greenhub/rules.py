# -*- coding: utf-8 -*-
from greenhub.models import Sensor

TEMP_MAX = 22
LUX_MAX = 100

def set_code(code):
    def decorator(func):
        func.code = code
        return func
    return decorator

def avg_sensor_last_hour(user, type):
    sensors = Sensor.objects.filter(user=user, type=type).all()

    avgs = []
    for sensor in sensors:
        avgs.append(sensor.last_hour_state())

    if not len(avgs) or None in avgs:
        return None

    return sum(avgs) / len(avgs)

@set_code(code="using-heating-while-hot")
def is_wasting_heat(user):
    avg_temp = avg_sensor_last_hour(user, u"temp")
    avg_heating = avg_sensor_last_hour(user, u"heating")

    if avg_temp is None or avg_heating is None:
        return False

    return avg_temp > TEMP_MAX and avg_heating > 0.5

@set_code(code="opening-window-while-heating")
def is_wasting_heat2(user):
    avg_heating = avg_sensor_last_hour(user, u"heating")
    avg_window = avg_sensor_last_hour(user, u"window")

    if avg_heating is None or avg_window is None:
        return False

    return avg_window > 0.5 < avg_heating

@set_code(code="using-lamp-during-daytime")
def is_wasting_lamp(user):
    avg_lamp = avg_sensor_last_hour(user, u"lamp")
    avg_lux =  avg_sensor_last_hour(user, u"lux")

    if avg_lamp is None or avg_lux is None:
        return False

    return avg_lux > LUX_MAX and avg_lamp >= 0.5

all_rules = [is_wasting_heat, is_wasting_heat2, is_wasting_lamp]