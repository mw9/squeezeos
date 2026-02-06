DESCRIPTION = "Linux USB ALSA kernel module for M2Tech hiFace audio devices"
HOMEPAGE = "https://github.com/ralph-irving/snd-usb-asyncaudio/"
LICENSE = "GPLv2"
PR = "r0"

SRC_URI = " \
"

SRC_URI_append_fab4 = " \
        file://snd-usb-hiface.ko \
"

FAB4MODULES=/lib/modules/2.6.26.8-rt16-332-g5849bfa

do_install_append_fab4() {
        install -m 0755 -d ${D}/${FAB4MODULES}
        install -m 0644 ${S}/../snd-usb-hiface.ko ${D}/${FAB4MODULES}/snd-usb-hiface.ko
}

FILES_${PN}_fab4 += " \
        ${FAB4MODULES}/snd-usb-hiface.ko \
"
