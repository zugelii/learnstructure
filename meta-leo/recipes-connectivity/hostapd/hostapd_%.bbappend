do_install_append () {
	sed -i -e 's/ssid=ap-wlan1/ssid=conti-wlan1/' ${D}${sysconfdir}/hostapd_wlan1.conf
	sed -i -e 's/.*wpa_passphrase.*/wpa_passphrase=contismartrd/' ${D}${sysconfdir}/hostapd_wlan1.conf	
}
