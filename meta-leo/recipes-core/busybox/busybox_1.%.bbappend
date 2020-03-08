FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
    file://conti.png \
    file://enable-coredump.cfg \
"

