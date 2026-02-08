SUMMARY = "A suite of security-related network utilities based on the SSH protocol including the ssh client and sshd server"
DESCRIPTION = "Only provides sftp-server utility for dropbear ssh server"
HOMEPAGE = "http://www.openssh.com/"
LICENSE = "BSD-2-Clause & BSD-3-Clause & ISC & MIT"

PR="r0"

SRC_URI = "https://ftp.openbsd.org/pub/OpenBSD/OpenSSH/portable/${PN}-${PV}.tar.gz"

S="${WORKDIR}/${PN}-${PV}"

inherit autotools

EXTRA_OECONF = "--disable-shared --disable-lastlog --disable-utmp --disable-utmpx --disable-wtmp --disable-wtmpx --without-openssl --without-libedit --without-zlib"

autotools_do_configure() {
        oe_runconf
}

do_compile () {
        oe_runmake sftp-server
}

do_install () {
        install -d ${D}${libexecdir}
        install -m 0755 ${S}/sftp-server ${D}${libexecdir}/sftp-server
}

RPROVIDES="sftp-server"
