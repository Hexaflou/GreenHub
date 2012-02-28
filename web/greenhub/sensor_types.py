
class sensor_type:
    pass

class light_sensor(sensor_type):
    title = "Capteur de luminescence"

    def format_value(value):
        return "%s°C" % value


class temp_sensor(sensor_type):
    title = "Capteur de température"

    def format_value(value):
        return "%s lux" % value

class contact_sensor(sensor_type):
    title = "Capteur de contact (porte, fenêtre...)"

    def format_value(value):
        if value > 0:
            return "fermé"
        else:
            return "ouvert"

class switch_sensor(sensor_type):
    title = "Capteur d'interrupteur (boutons)"

    def format_value(value):
        return value # Unused on the interface