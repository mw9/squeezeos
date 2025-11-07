DESCRIPTION = "SqueezePlay - Fab4 specific code"
LICENSE = "Logitech Public Source License"

PR = "r5"

PROVIDES = "squeezeplay-fab4-bin"

DEPENDS += "squeezeplay"

SRC_URI=" \
	${RALPHY_SQUEEZEOS}/libspotify-0.0.6-linux6-armv6.tar.gz \
	file://fab4_bsp.so \
	file://libspotify.c \
	"

S = "${WORKDIR}/"

do_compile() {
	${CC} ${TARGET_CFLAGS} -I${WORKDIR}/libspotify-0.0.6-linux6-armv6/include/ -c -fPIC libspotify.c -o libspotify.o
	${CC} ${TARGET_CFLAGS} ${TARGET_LDFLAGS} -shared -s -o ${WORKDIR}/libspotify.so.6.0.600035 libspotify.o
}

do_install() {
	# move lua fab4 bsp module to correct location.
	mkdir -p ${D}${libdir}/lua/5.1
	install -m 0755 ${WORKDIR}/fab4_bsp.so ${D}${libdir}/lua/5.1/fab4_bsp.so

	install -m 0755 ${WORKDIR}/libspotify.so.6.0.600035 ${D}${libdir}/libspotify.so.6.0.600035
	cd ${D}${libdir}; ln -sf libspotify.so.6.0.600035 libspotify.so.6

}

FILES_${PN} += "${datadir} ${libdir}/lua/5.1"
FILES_${PN}-dbg += "${libdir}/lua/5.1/.debug"
