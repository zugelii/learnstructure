# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "camera data fusion"
LICENSE = "CLOSED"

DEPENDS = "ecal protobuf protobuf-native protobuf-compiler-plugins-native rapidjson libeigen"

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/CFusion.git;branch=master;protocol=ssh; \
	   file://cfusion.service \
"

SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"

do_install_append() {
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/cfusion.service ${D}${systemd_unitdir}/system
}
inherit systemd
SYSTEMD_SERVICE_${PN} = "cfusion.service"
SYSTEMD_AUTO_ENABLE = "enable"
