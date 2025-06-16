<?php
/**
 * This file is part of OpenMediaVault.
 *
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Your Name <your.email@example.com>
 * @copyright Copyright (c) 2025 Your Name
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
 */
class OMVModuleActiveDirectory extends \OMV\Engine\Module\ServiceAbstract
		implements \OMV\Engine\Notify\IListener {
	/**
	 * Get the module name.
	 */
	public function getName() {
		return "activedirectory";
	}

    /**
	 * Defines the modules that should be deployed after this module.
	 * @return array
	 */
	//public function deployAfter(): array {
	//	return ["smb"];
	//}

	/**
	 * Get the module status.
	 */
	public function getStatus() {
		$db = \OMV\Config\Database::getInstance();
		$object = $db->get("conf.service.activedirectory");
		$smbdSystemCtl = new \OMV\System\SystemCtl("smbd");
		$winbindSystemCtl = new \OMV\System\SystemCtl("winbind");
		return [
			"name" => $this->getName(),
			"title" => gettext("Active Directory"),
			"enabled" => $object->get("enable"),
			"running" => $smbdSystemCtl->isActive() && $winbindSystemCtl->isActive()
		];
	}

	/**
	 * Bind listeners.
	 */
	function bindListeners(\OMV\Engine\Notify\Dispatcher $dispatcher) {
		$dispatcher->addListener(
			OMV_NOTIFY_MODIFY,
			"org.openmediavault.conf.service.activedirectory",
			[$this, "setDirty"]);
        $moduleMngr = \OMV\Engine\Module\Manager::getInstance();
		$dispatcher->addListener(
            OMV_NOTIFY_MODIFY,
            "org.openmediavault.conf.service.activedirectory",
            [$moduleMngr->getModule("samba"), "setDirty"]);
	}
}
?>