# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "ecu/rsu/cannet/camera power off/on"
LICENSE = "CLOSED"

DEPENDS = "rapidjson"

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/relayContrl.git;branch=master;protocol=ssh; \
	   "
SRCREV  = "${AUTOREV}"


S = "${WORKDIR}/git"

inherit cmake
FILES_${PN}-dev += "${libdir}/cmake/*"

