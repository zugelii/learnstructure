# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "ecal to RSU"
LICENSE = "CLOSED"

DEPENDS = "ecal protobuf protobuf-native protobuf-compiler-plugins-native rapidjson mosquitto"

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/eCal2RSU.git;branch=master;protocol=ssh; \
	   file://ecal2rsu.service \
	   "

SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"
do_install_append() {
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/ecal2rsu.service ${D}${systemd_unitdir}/system
}

inherit systemd
SYSTEMD_SERVICE_${PN} = "ecal2rsu.service"
SYSTEMD_AUTO_ENABLE = "enable"
