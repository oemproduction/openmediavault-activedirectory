<?php

class OMVRpcServiceTftp extends \OMV\Rpc\ServiceAbstract {
    public function getName()
    {
        return "ActiveDirectory";
    }

    /**
     * Initialize the RPC service methods.
     */
    public function initialize() {
        $this->registerMethod("getSettings");
        $this->registerMethod("setSettings");
    }

    /**
     * Get the Active Directory configuration settings, including Samba workgroup.
     * @param array $params The method parameters.
     * @param array $context The context of the caller.
     * @return array The configuration object.
     */
    public function getSettings($params, $context) {
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
    public function setSettings($params, $context) {
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
            "mapuntrusted" => $params["mapuntrusted"],
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

