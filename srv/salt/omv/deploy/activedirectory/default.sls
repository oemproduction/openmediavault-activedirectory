# This file is part of OpenMediaVault.
#
# @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
# @author    Jason Lo <jason@oemproduction.com>
#
# OpenMediaVault is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# OpenMediaVault is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with OpenMediaVault. If not, see <http://www.gnu.org/licenses/>.

{% set config = salt['omv_conf.get']('conf.service.activedirectory') %}

{% if config.enable | to_bool %}

configure_nsswitch:
  file.managed:
    - name: /etc/nsswitch.conf
    - source: salt://{{ tpldir }}/files/etc_nsswitch_conf.j2
    - template: jinja
    - context:
        config: {{ config | json }}
    - user: root
    - group: root
    - mode: 644

configure_krb5:
  file.managed:
    - name: /etc/krb5.conf
    - source: salt://{{ tpldir }}/files/etc_krb5_conf.j2
    - template: jinja
    - context:
        config: {{ config | json }}
    - user: root
    - group: root
    - mode: 644

include:
  - omv.deploy.samba

join_domain:
  cmd.run:
    - name: "net ads join -U {{ config.adminuser }}%{{ config.adminpw }}"
    - require:
      - file: configure_krb5

winbind_service_restart:
  cmd.run:
    - name: systemctl restart winbind
    - require:
      - file: configure_krb5

{% else %}
revert_nsswitch:
  file.replace:
    - name: /etc/nsswitch.conf
    - pattern: '\s*winbind\s*'
    - repl: ''

disable_winbind_service:
  service.dead:
    - name: winbind
    - enable: False

leave_domain:
  cmd.run:
    - name: "net ads leave -U {{ config.adminuser }}%{{ config.adminpw }}"      
{% endif %}