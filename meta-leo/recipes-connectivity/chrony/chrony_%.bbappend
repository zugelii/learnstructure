do_install_append () {
	sed -i -e 's/.*server 192.168.22.22/server 192.168.3.9/' ${D}${sysconfdir}/chrony.conf
	sed -i -e 's/.*allow 192.168/allow 192.168/' ${D}${sysconfdir}/chrony.conf
	echo "server 192.168.1.25 iburst minpoll 2 prefer" >> ${D}${sysconfdir}/chrony.conf	
}
