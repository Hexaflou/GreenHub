from django.conf.urls.defaults import patterns, include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'GreenHub.views.home', name='home'),
    # url(r'^GreenHub/', include('GreenHub.foo.urls')),
    url(r'^$', 'greenhub.views.index'),
    url(r'^home', 'greenhub.views.home'),
    url(r'^send-command/(?P<type>.*)', 'greenhub.views.command'),

    url(r'^login/$', 'django.contrib.auth.views.login', {'template_name': 'login.html'}),

    # Uncomment the admin/doc line below to enable admin documentation:
    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)
