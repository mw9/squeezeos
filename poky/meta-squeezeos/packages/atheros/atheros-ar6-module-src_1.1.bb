DESCRIPTION = "Atheros AR6 sdio wlan driver"
SECTION = "base"
LICENSE = "binary only"

PV = "1.1"
PR = "r3"
DV = "83"

PROVIDES = "atheros-ar6-module"

DEPENDS = "virtual/kernel"

SRC_URI="${RALPHY_SQUEEZEOS}/atheros-ar6ksdk.build_sw.${DV}.tar.gz \
	 file://AR6kSDK.build_sw.${DV}.baby.patch;patch=1 \
	 file://atheros_configpm.patch;patch=1 \
	 file://atheros_nostatupdate.patch;patch=1 \
	 file://BMI_read_mem_failure.patch;patch=1 \
	 file://change-ar6k-semaphore-type.patch;patch=1 \
	\
	 file://0002-too-short-debug.patch;patch=1 \
	 file://0003-driver-print-version.patch;patch=1 \
	 file://0004-backport-old-ioctl-target-stats.patch;patch=1 \
	 file://0005-support-old-and-new-firmware-target-stats.patch;patch=1 \
	 file://0006-backport-old-ioctl-wmiconfig.patch;patch=1 \
	 file://0007-log-power-setting.patch;patch=1 \
	 file://0008-log-phy-rate-change.patch;patch=1 \
	 file://0009-arp-dhcp-logging.patch;patch=1 \
	 file://0010-driver-debug-hdr-ptr.patch;patch=1 \
	 file://0011-driver-debug-stop-endpoint.patch;patch=1 \
	 file://0012-log-wmi-set-BSS-filter-command.patch;patch=1 \
	\
	 file://bmiloader \
	 file://wmiconfig \
	 file://eeprom.AR6002 \
	 file://wlan \
	 file://loadAR6000l.sh \
	 file://athwlan.bin \
	 file://data.patch.hw2_0.bin \
	 file://calData_ar6102_15dBm.bin \
	\
	 file://restart-wifi.sh \
	 file://watch-arp.sh \
	 file://wlan.conf \
	 file://SetupWifiRobustness \
"

S = "${WORKDIR}/AR6kSDK.build_sw.${DV}"

inherit module-base

INHIBIT_PACKAGE_STRIP = 1

do_compile() {
	cd ${S}/host
	oe_runmake LDFLAGS=""
}

do_install() {
        INSTALL_DIR=${D}/${base_libdir}/atheros
        install -m 0755 -d ${INSTALL_DIR}

        # kernel module
        install -m 0644 ${S}/host/os/linux/ar6000.ko ${INSTALL_DIR}/ar6000.ko

        # tools
        install -m 0755 ${WORKDIR}/bmiloader ${INSTALL_DIR}/bmiloader
        install -m 0755 ${WORKDIR}/wmiconfig ${INSTALL_DIR}/wmiconfig
        install -m 0755 ${WORKDIR}/eeprom.AR6002 ${INSTALL_DIR}/eeprom.AR6002

        # firmware
        install -m 0644 ${WORKDIR}/athwlan.bin ${INSTALL_DIR}/athwlan.bin
        install -m 0644 ${WORKDIR}/data.patch.hw2_0.bin ${INSTALL_DIR}/data.patch.hw2_0.bin
        install -m 0644 ${WORKDIR}/calData_ar6102_15dBm.bin ${INSTALL_DIR}/calData_ar6102_15dBm.bin

        # scripts
        install -m 0755 -d ${D}${sysconfdir}/init.d
        install -m 0755 ${WORKDIR}/wlan ${D}${sysconfdir}/init.d/wlan
        install -m 0755 ${WORKDIR}/loadAR6000l.sh ${INSTALL_DIR}/loadAR6000l.sh
	install -m 0755 ${WORKDIR}/watch-arp.sh ${INSTALL_DIR}/watch-arp.sh
	install -m 0755 ${WORKDIR}/restart-wifi.sh ${INSTALL_DIR}/restart-wifi.sh

	# config file
        install -m 0755 ${WORKDIR}/wlan.conf ${D}${sysconfdir}/wlan.conf

	# Settings applet
	install -m 0755 -d ${D}${datadir}/jive/applets/SetupWifiRobustness
	install -m 0755 ${WORKDIR}/SetupWifiRobustness/SetupWifiRobustnessApplet.lua ${D}${datadir}/jive/applets/SetupWifiRobustness/SetupWifiRobustnessApplet.lua
	install -m 0755 ${WORKDIR}/SetupWifiRobustness/SetupWifiRobustnessMeta.lua ${D}${datadir}/jive/applets/SetupWifiRobustness/SetupWifiRobustnessMeta.lua
	install -m 0755 ${WORKDIR}/SetupWifiRobustness/strings.txt ${D}${datadir}/jive/applets/SetupWifiRobustness/strings.txt
}

PACKAGES = "atheros-ar6-module-dbg atheros-ar6-module"

FILES_atheros-ar6-module = "${base_libdir}/modules/${KERNEL_VERSION} ${base_libdir}/atheros ${sysconfdir} ${datadir}/jive/applets"
FILES_atheros-ar6-module-dbg = "${base_libdir}/modules/${KERNEL_VERSION}/.debug ${base_libdir}/atheros/.debug"

