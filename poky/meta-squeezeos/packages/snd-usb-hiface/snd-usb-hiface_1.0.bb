DESCRIPTION = "Linux USB ALSA kernel module for M2Tech hiFace audio devices"
HOMEPAGE = "https://github.com/ralph-irving/snd-usb-asyncaudio/"
LICENSE = "GPLv2"
PR = "r1"

inherit module-base

SRC_URI = " \
"

SRC_URI_append_fab4 = " \
        file://snd-usb-hiface.ko \
"

MODULES=/lib/modules/${KERNEL_VERSION}

do_install_append_fab4() {
        install -m 0755 -d ${D}/${MODULES}
        install -m 0644 ${S}/../snd-usb-hiface.ko ${D}/${MODULES}/snd-usb-hiface.ko
}

FILES_${PN}_fab4 += " \
        ${MODULES}/snd-usb-hiface.ko \
"
