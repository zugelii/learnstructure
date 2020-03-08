# Copyright (c) 2019 Continental AG and subsidiaries

SUMMARY = "backend agent for china smart road pilot"
LICENSE = "CLOSED"

DEPENDS = "openssl curl dbus sdbus-c++ libxml2"

SRC_URI = "gitsm://git@github.conti.de/Corp-SnT-SmartRoad/BEAgent.git;protocol=ssh"
SRCREV  = "${AUTOREV}"

# magic to auto rebuild when git head updates
# PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit cmake
