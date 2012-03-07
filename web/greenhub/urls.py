from django.conf.urls.defaults import patterns, include, url

urlpatterns = patterns('',
    url(r'^$', 'greenhub.views.index'),
    url(r'^home', 'greenhub.views.home'),
    url(r'^sensors', 'greenhub.views.sensors'),
    url(r'^settings', 'greenhub.views.settings'),

    url(r'^send-command/(?P<type>.*)', 'greenhub.views.command'),

    url(r'^login/?$', 'django.contrib.auth.views.login', {'template_name': 'login.html'}),
    url(r'^logout/?$', 'greenhub.views.logout', {}),
)
