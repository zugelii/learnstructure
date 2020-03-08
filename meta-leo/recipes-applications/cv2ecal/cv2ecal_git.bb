# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "cv data to ecal"
LICENSE = "CLOSED"

DEPENDS = "ecal protobuf protobuf-native protobuf-compiler-plugins-native zeromq opencv rapidjson"

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/Cv2eCAL.git;branch=master;protocol=ssh; \
	   file://cv2ecal.service \
"

SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"

do_install_append() {
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/cv2ecal.service ${D}${systemd_unitdir}/system
}
inherit systemd
SYSTEMD_SERVICE_${PN} = "cv2ecal.service"
SYSTEMD_AUTO_ENABLE = "enable"
