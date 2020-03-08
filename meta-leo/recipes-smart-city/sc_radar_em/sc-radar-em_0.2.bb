SUMMARY = "Radar Enviroment Model"
LICENSE = "CLOSED"

DEPENDS += "runtask ecal protobuf protobuf-native protobuf-compiler-plugins-native cmake-functions"

SRC_URI = "gitsm://github.conti.de/Corp-SnT-SmartRoad/SCRadarEM.git;protocol=http;branch=changsha;"
SRC_URI += " \
    file://sc_radar_em.service \
		file://Changsha.rtx \
"
#PV = "0.1+git${SRCPV}"
# pick up latest rev for this module. Note this a deferred evaluation!
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"
do_install_append(){
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/sc_radar_em.service ${D}${systemd_unitdir}/system
	install -d ${D}/${sysconfdir}/sc_radar_em
	install -m 0755 ${WORKDIR}/Changsha.rtx ${D}/${sysconfdir}/sc_radar_em/Changsha.rtx
}

inherit systemd
SYSTEMD_SERVICE_${PN} = "sc_radar_em.service"
SYSTEMD_AUTO_ENABLE = "enable"
