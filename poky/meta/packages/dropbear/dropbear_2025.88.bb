DESCRIPTION = "Dropbear is a lightweight SSH and SCP Implementation"
HOMEPAGE = "http://matt.ucc.asn.au/dropbear/dropbear.html"
SECTION = "console/network"
LICENSE = "MIT"
DEPENDS = "zlib"
PROVIDES = "ssh sshd"
RPROVIDES = "ssh sshd"

SRC_URI = "https://matt.ucc.asn.au/dropbear/releases/dropbear-${PV}.tar.bz2 \
	file://localoptions.h \
	file://gcc.patch;patch=1 \
	file://remove-500mb-channel-window-limit.patch;patch=1 \
	"

PR="r1"

inherit autotools

CFLAGS_prepend = " -I. -std=gnu99 "
LD = "${CC}"

SBINCOMMANDS = "dropbear dropbearkey dropbearconvert"
BINCOMMANDS = "dbclient ssh scp"
EXTRA_OEMAKE = 'MULTI=1 SCPPROGRESS=1 PROGRAMS="${SBINCOMMANDS} ${BINCOMMANDS}"'
EXTRA_OECONF += "--srcdir=src"

do_configure_prepend () {
	cp ${WORKDIR}/localoptions.h .
}

do_configure_append() {
	sed -i 's!src/src!src!g' Makefile
	ln -sv ../libtommath src/libtommath
	ln -sv ../libtomcrypt src/libtomcrypt
}

do_install() {
	install -d ${D}${sysconfdir} \
		   ${D}${sysconfdir}/init.d \
		   ${D}${sysconfdir}/default \
		   ${D}${sysconfdir}/dropbear \
                   ${D}${bindir} \
		   ${D}${sbindir} \
		   ${D}${localstatedir}

	install -m 0755 dropbearmulti ${D}${sbindir}/
	ln -s ${sbindir}/dropbearmulti ${D}${bindir}/dbclient
	
	for i in ${SBINCOMMANDS}
	do
		ln -s ./dropbearmulti ${D}${sbindir}/$i
	done
}

pkg_postinst () {
	update-alternatives --install ${bindir}/scp scp ${sbindir}/dropbearmulti 20
	update-alternatives --install ${bindir}/ssh ssh ${sbindir}/dropbearmulti 20
}

pkg_postrm_append () {
  if [ -f "${sysconfdir}/dropbear/dropbear_rsa_host_key" ]; then
        rm ${sysconfdir}/dropbear/dropbear_rsa_host_key
  fi
  if [ -f "${sysconfdir}/dropbear/dropbear_dss_host_key" ]; then
        rm ${sysconfdir}/dropbear/dropbear_dss_host_key
  fi
  if [ -f "${sysconfdir}/dropbear/dropbear_ecdsa_host_key" ]; then
        rm ${sysconfdir}/dropbear/dropbear_ecdsa_host_key
  fi
  if [ -f "${sysconfdir}/dropbear/dropbear_ed25519_host_key" ]; then
        rm ${sysconfdir}/dropbear/dropbear_ed25519_host_key
  fi
  update-alternatives --remove ssh ${bindir}/dropbearmulti
  update-alternatives --remove scp ${bindir}/dropbearmulti
}

