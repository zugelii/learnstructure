# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "hardware watchdog/fun control"
LICENSE = "CLOSED"

DEPENDS = "ecal mosquitto libdigiapix rapidjson zeromq protobuf protobuf-native protobuf-compiler-plugins-native "

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/HwMonitor.git;branch=master;protocol=ssh; \
	   file://hwmoniter.service \
	   "
SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"
do_install_append() {
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/hwmoniter.service ${D}${systemd_unitdir}/system
}

inherit systemd
SYSTEMD_SERVICE_${PN} = "hwmoniter.service"
SYSTEMD_AUTO_ENABLE = "enable"
