# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "CANNET200 (can to ethernet converter) eCAL integration"
LICENSE = "CLOSED"


DEPENDS = "ecal protobuf ctzs-dependencies protobuf-native protobuf-compiler-plugins-native rapidjson"

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/Cannet2Ecal.git;branch=master;protocol=ssh; \
	   file://cannet2ecal.service \
"
SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"


do_install_append() {
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/cannet2ecal.service ${D}${systemd_unitdir}/system
}
inherit systemd
SYSTEMD_SERVICE_${PN} = "cannet2ecal.service"
SYSTEMD_AUTO_ENABLE = "enable"
