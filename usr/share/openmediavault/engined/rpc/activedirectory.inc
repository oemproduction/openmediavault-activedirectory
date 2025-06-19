<?php
/**
 * This file is part of OpenMediaVault.
 *
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Jason Lo <jason@oemproduction.com>
 *
 * OpenMediaVault is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * OpenMediaVault is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenMediaVault. If not, see <http://www.gnu.org/licenses/>.
 */
namespace Engined\Rpc;

class OMVRpcServiceActiveDirectory extends \OMV\Rpc\ServiceAbstract {
    public function getName()
    {
        return "ActiveDirectory";
    }

    /**
     * Initialize the RPC service methods.
     */
    public function initialize() {
        $this->registerMethod("get");
        $this->registerMethod("set");
    }

    /**
     * Get the Active Directory configuration settings, including Samba workgroup.
     * @param array $params The method parameters.
     * @param array $context The context of the caller.
     * @return array The configuration object.
     */
    public function get($params, $context) {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        $db = \OMV\Config\Database::getInstance();
        // Get Active Directory settings
        $adObject = $db->get("conf.service.activedirectory");
        $adSettings = $adObject->getAssoc();
        // Get Samba workgroup
        $smbObject = $db->get("conf.service.smb");
        $smbSettings = $smbObject->getAssoc();
        // Append Samba workgroup to AD settings
        $adSettings["workgroup"] = $smbSettings["workgroup"];
        return $adSettings;
    }

    /**
     * Set the Active Directory configuration settings.
     * @param array $params The method parameters.
     * @param array $context The context of the caller.
     * @return array The configuration object.
     */
    public function set($params, $context) {
        // Validate the RPC caller context.
		$this->validateMethodContext($context, [
			"role" => OMV_ROLE_ADMINISTRATOR
		]);
		// Validate the parameters of the RPC service method.
		$this->validateMethodParams($params, "rpc.activedirectory.set");
        $db = \OMV\Config\Database::getInstance();
        
        // Update Active Directory settings
        $adObject = new \OMV\Config\ConfigObject("conf.service.activedirectory");
        $adObject->setAssoc([
            "enable" => $params["enable"],
            "hostname" => $params["hostname"],
            "domain" => $params["domain"],
            "adminuser" => $params["adminuser"],
            "adminpw" => $params["adminpw"],
            "idstart" => $params["idstart"],
            "idend" => $params["idend"],
            "cachetime" => $params["cachetime"],
            "defaultdomain" => $params["defaultdomain"],
            "rpconly" => $params["rpconly"],
            "ticketlife" => $params["ticketlife"],
            "renewlife" => $params["renewlife"]
        ]);
        $db->set($adObject);
        
        // Update Samba workgroup
        $smbObject = $db->get("conf.service.smb");
        $smbObject->set("workgroup", $params["workgroup"]);
        $db->set($smbObject);
        
        $assocArray = $adObject->getAssoc();
        $assocArray["workgroup"] = $param["workgroup"];
        
        return $assocArray;
    }
}
