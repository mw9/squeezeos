SECTION = "base"
LICENSE = "OSL"
DESCRIPTION = "A collection of utilities and DSOs to handle compiled objects."
DEPENDS = "libtool"
PR = "r2"

SRC_URI = "http://distro.ibiblio.org/pub/linux/distributions/gentoo/distfiles/elfutils-${PV}.tar.gz \
	   file://warnings.patch;patch=1"

inherit autotools

# Package binaries that overlap with binutils separately
PACKAGES =+ "${PN}-binutils"
FILES_${PN}-binutils = "\
    ${bindir}/addr2line \
    ${bindir}/ld \
    ${bindir}/nm \
    ${bindir}/readelf \
    ${bindir}/size \
    ${bindir}/strip"
# Fix library issues
FILES_${PN} =+ "${libdir}/*-${PV}.so"

SOV = "${PV}"
do_stage () {
	install -m 0644 libelf/libelf.a ${STAGING_LIBDIR}/libelf.a
	install -m 0755 libelf/libelf.so.1 ${STAGING_LIBDIR}/libelf-${SOV}.so
	ln -sf libelf-${SOV}.so ${STAGING_LIBDIR}/libelf.so.1
	ln -sf libelf.so.1 ${STAGING_LIBDIR}/libelf.so
	install -m 0644 libasm/libasm.a ${STAGING_LIBDIR}/libasm.a
	install -m 0755 libasm/libasm.so.1 ${STAGING_LIBDIR}/libasm-${SOV}.so
	ln -sf libasm-${SOV}.so ${STAGING_LIBDIR}/libasm.so.1
	ln -sf libasm.so.1 ${STAGING_LIBDIR}/libasm.so
	install -m 0644 libdw/libdw.a ${STAGING_LIBDIR}/libdw.a
	install -m 0755 libdw/libdw.so.1 ${STAGING_LIBDIR}/libdw-${SOV}.so
	ln -sf libdw-${SOV}.so ${STAGING_LIBDIR}/libdw.so.1
	ln -sf libdw.so.1 ${STAGING_LIBDIR}/libdw.so
	install -m 0644 libebl/libebl.a ${STAGING_LIBDIR}/libebl.a
	install -d ${STAGING_LIBDIR}/elfutils/
	install -m 0755 libebl/libebl_*.so ${STAGING_LIBDIR}/elfutils/

	install -m 0644 ${S}/libelf/libelf.h ${STAGING_INCDIR}/
	install -m 0644 ${S}/libelf/gelf.h ${STAGING_INCDIR}/
	install -m 0644 ${S}/libelf/nlist.h ${STAGING_INCDIR}/
	install -d ${STAGING_INCDIR}/elfutils
	install -m 0644 ${S}/libebl/libebl.h ${STAGING_INCDIR}/elfutils/
	install -m 0644 ${S}/libdw/libdw.h ${STAGING_INCDIR}/elfutils/
	install -m 0644 ${S}/libasm/libasm.h ${STAGING_INCDIR}/elfutils/
}

# The elfutils package contains symlinks that trip up insane
INSANE_SKIP_elfutils = "1"
