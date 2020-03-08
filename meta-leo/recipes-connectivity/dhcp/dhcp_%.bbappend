FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
  file://udhcp.conf \
"
FILES_${PN}-client += "${sysconfdir}/dhcp/udhcp.conf"


do_install_append () {
	install -d ${D}${sysconfdir}/dhcp
	install -m 0644 ${WORKDIR}/udhcp.conf  ${D}${sysconfdir}/dhcp/udhcp.conf

}
