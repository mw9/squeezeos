DESCRIPTION = "netcat"
SUMMARY = "TCP/IP swiss army knife"
SECTION = "bin"
LICENSE = "GPL"

PR = "r0"

SRC_URI="http://coast.cs.purdue.edu/pub/tools/unix/netutils/${PN}/nc110.tgz \
	file://unstripped.patch;patch=1 \
	file://glibc-resolv-h.patch;patch=1 \
	file://arm-timer.patch;patch=1 \
	file://posix-setjmp.patch;patch=1 \
	file://no-sleep-punt.patch;patch=1 \
	file://single-verbose.patch;patch=1 \
	file://use-getservbyport.patch;patch=1 \
	file://read-overflow.patch;patch=1 \
	file://inet-aton.patch;patch=1 \
	file://udp-broadcast.patch;patch=1 \
	file://quit.patch;patch=1 \
	file://dash-port.patch;patch=1 \
	file://sh-c.patch;patch=1 \
	file://tos.patch;patch=1 \
	file://rservice-buf.patch;patch=1 \
	file://so-keepalive.patch;patch=1 \
	file://nodup-stderr.patch;patch=1 \
	file://help-exit-failure.patch;patch=1 \
	file://darwin-ipproto.patch;patch=1 \
	file://select-nfds.patch;patch=1 \
	file://proxy-doc.patch;patch=1 \
	file://655881-Makefile-LDFLAGS.patch;patch=1 \
	file://655881-netcat.c.format.patch;patch=1 \
	file://netcat-not-nc.patch;patch=1 \
"

S = "${WORKDIR}/"

do_compile() {
	${CC} ${TARGET_CFLAGS} -DDEBIAN_VERSION=\"${PV}\" -DLINUX -DGAPING_SECURITY_HOLE -DIP_TOS -DTELNET -o ${PN} ${PN}.c
}

do_install() {
	install -m 0755 -d ${D}${bindir}
	install -m 0755 ${S}/${PN} ${D}${bindir}/${PN}
}
