SUMMARY = "sdbus-c++"
DESCRIPTION = "High-level C++ D-Bus library designed to provide easy-to-use yet powerful API in modern C++"

SECTION = "libs"

LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://${WORKDIR}/sdbus-cpp-${PV}/COPYING;md5=1803fa9c2c3ce8cb06b4861d75310742"

inherit cmake pkgconfig systemd ptest python3native

DEPENDS += "expat ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', 'meson-native ninja-native git-native gperf-native gettext-native coreutils-native util-linux glibc libcap', d)}"
RDEPENDS_${PN} = "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'libsystemd', 'glibc libcap', d)}"

SRC_URI = "git://github.com/Kistler-Group/sdbus-cpp.git;protocol=https;destsuffix=sdbus-cpp-${PV};branch=master;tag=v${PV}"
SRC_URI += "file://run-ptest"

TEST_BIN_DIR = "/opt/test/bin"

# Allow CMake to use git on the host machine
OECMAKE_FIND_ROOT_PATH_MODE_PROGRAM = "BOTH"
# Ninja is the default CMake generator in Yocto now, so let's set Unix Makefiles explicitly
OECMAKE_GENERATOR = "Unix Makefiles"

EXTRA_OECMAKE = "-DBUILD_CODE_GEN=ON \
                 -DBUILD_DOC=ON \
                 -DBUILD_DOXYGEN_DOC=OFF \
                 ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '-DBUILD_LIBSYSTEMD=OFF', '-DBUILD_LIBSYSTEMD=ON -DLIBSYSTEMD_VERSION=242', d)} \
                 ${@bb.utils.contains('PTEST_ENABLED', '1', '-DBUILD_TESTS=ON -DTESTS_INSTALL_PATH=${TEST_BIN_DIR}', '-DBUILD_TESTS=OFF', d)}"

S = "${WORKDIR}/sdbus-cpp-${PV}"

PACKAGES += "${PN}-test"
ALLOW_EMPTY_${PN}-test = "1"
RDEPENDS_${PN}-test = "${PN}"
RDEPENDS_${PN}-ptest = "${PN}-test"

# Don't take org.sdbuscpp.integrationtests.conf file into the base package
FILES_${PN} = "${libdir}/"

FILES_${PN}-test += "${sysconfdir}/dbus-1/system.d/"
FILES_${PN}-test += "${TEST_BIN_DIR}/"

FILES_${PN}-dev += "${bindir}/sdbus-c++-xml2cpp"
FILES_${PN}-dev += "${libdir}/cmake/sdbus-c++/sdbus-c++-config.cmake"
FILES_${PN}-dev += "${libdir}/cmake/sdbus-c++/sdbus-c++-config-version.cmake"
