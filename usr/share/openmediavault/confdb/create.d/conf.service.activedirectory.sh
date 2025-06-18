#!/bin/sh
#
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

set -e

. /usr/share/openmediavault/scripts/helper-functions

########################################################################
# Update the configuration.
# <config>
#   <services>
#     <activedirectory>
#       <enable>0</enable>
#       <hostname></hostname>
#       <domain></domain>
#       <adminuser></adminuser>
#       <adminpw></adminpw>
#       <idstart>10000</idstart>
#       <idend>60000</idend>
#       <cachetime>3600</cachetime>
#       <defaultdomain>1</defaultdomain>
#       <rpconly>1</rpconly>
#       <ticketlife>600</ticketlife>
#       <renewlife>7d</renewlife>
#     </activedirectory>
#   </services>
# </config>
########################################################################

if ! omv-confdbadm exists "conf.service.activedirectory"; then
    omv-confdbadm read --defaults "conf.service.activedirectory" | omv-confdbadm update "conf.service.activedirectory" -
fi

exit 0