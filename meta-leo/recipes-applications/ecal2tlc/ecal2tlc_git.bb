# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "ecal to TLC"
LICENSE = "CLOSED"

DEPENDS = "ecal protobuf protobuf-native protobuf-compiler-plugins-native rapidjson"

SRC_URI = "gitsm://git@github.conti.de:/Corp-SnT-SmartRoad/eCal2TLC.git;branch=master;protocol=ssh; \
	   file://ecal2tlc.service \
	   "

SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"
do_install_append() {
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/ecal2tlc.service ${D}${systemd_unitdir}/system
}
inherit systemd
SYSTEMD_SERVICE_${PN} = "ecal2tlc.service"
SYSTEMD_AUTO_ENABLE = "disable"
