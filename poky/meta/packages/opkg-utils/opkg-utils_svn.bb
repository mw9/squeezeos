DESCRIPTION = "OPKG Package Manager Utilities"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"
RDEPENDS = "python"
PR = "r3"

SRC_URI = "https://github.com/ralph-irving/squeezeos-pokycache/raw/refs/heads/public/${SQUEEZEOS_DISTRO_VERSION}/distfiles/opkg-utils_svn.openmoko.org_.trunk.src.host._4534_.tar.gz"

S = "${WORKDIR}/opkg-utils"

inherit autotools_stage

S = "${WORKDIR}/opkg-utils"
