import datetime
from django.contrib.auth.models import User
from django.core.management.base import BaseCommand, CommandError
from greenhub.models import Score
from greenhub import rules

MALUS = 5
REFUND_HOUR = 12
REFUND_AMOUNT = 20

class Command(BaseCommand):
    args = ''
    help = 'Calculate scores for all users.'

    def handle(self, *args, **options):
        try:
            users = User.objects.all()
        except User.DoesNotExist:
            raise CommandError('There is no registred user.')

        for user in users:
            try:
                previous = score_value = Score.objects.filter(user=user).order_by('date').reverse()[0].value
            except IndexError:
                previous = score_value = 0

            for is_rule_transgressed in rules.all_rules:
                if is_rule_transgressed(user):
                    score_value -= MALUS

            if datetime.datetime.now().hour == REFUND_HOUR:
                score_value += REFUND_AMOUNT

            score_obj = Score(user=user, date=datetime.datetime.now(), value=score_value)
            score_obj.save()

            self.stdout.write(u'%s goes from %s to %s\n' % (user, previous, score_value))

        self.stdout.write('Done !\n')
