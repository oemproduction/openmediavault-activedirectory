#!/bin/sh
#
# This file is part of OpenMediaVault.
#
# @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
# @author    Your Name <your.email@example.com>
# @copyright Copyright (c) 2025 Your Name
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
#       <idstart>1000</idstart>
#       <idend>60000</idend>
#       <cachetime>0</cachetime>
#       <mapuntrusted>0</mapuntrusted>
#       <rpconly>0</rpconly>
#       <ticketlife>0</ticketlife>
#       <renewlife></renewlife>
#     </activedirectory>
#   </services>
# </config>
########################################################################

if ! omv-confdbadm exists "conf.service.activedirectory"; then
    omv-confdbadm read --defaults "conf.service.activedirectory" | omv-confdbadm update "conf.service.activedirectory" -
fi

exit 0