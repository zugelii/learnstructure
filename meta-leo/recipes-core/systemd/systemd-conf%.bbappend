do_install_append () {
	sed -i -e 's/.*RuntimeWatchdogSec.*/RuntimeWatchdogSec=60/' ${D}${sysconfdir}/systemd/system.conf	
}

