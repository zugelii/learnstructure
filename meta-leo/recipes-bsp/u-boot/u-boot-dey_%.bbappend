FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "git://git@github.conti.de/Corp-SnT-SmartRoad/digi-u-boot.git;protocol=ssh;branch=mca_watchdog_imx8_r2.2"
SRCBRANCH = "mca_watchdog_imx8_r2.2"
SRCREV = "${AUTOREV}"

SRC_URI_append = " \
  file://boot.txt \
"

PR = "r2.2"

