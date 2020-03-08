FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
  file://interfaces.eth0 \
  file://cp_interfaces.sh \
  file://cp_interfaces.service \
  file://en_ipforward.sh \
  file://en_ipforward.service \
"

do_install_append() {
  #cat ${WORKDIR}/interfaces.eth0 >> ${D}${sysconfdir}/network/interfaces
  #install -d ${D}${systemd_system_unitdir}
  #install ${WORKDIR}/cp_interfaces.service ${D}${systemd_system_unitdir}
  #install ${WORKDIR}/cp_interfaces.sh ${D}${sysconfdir}/network/
  #mv ${D}${sysconfdir}/network/interfaces ${D}${sysconfdir}/network/interfaces.default
  #cd ${D}${sysconfdir}/network
  #ln -s /data/sysconf/network/interfaces
  #install ${WORKDIR}/en_ipforward.service ${D}${systemd_system_unitdir}
  #install ${WORKDIR}/en_ipforward.sh      ${D}${sysconfdir}/network/
}

inherit systemd
#SYSTEMD_SERVICE_${PN} += "cp_interfaces.service"
#SYSTEMD_SERVICE_${PN} += "en_ipforward.service"
