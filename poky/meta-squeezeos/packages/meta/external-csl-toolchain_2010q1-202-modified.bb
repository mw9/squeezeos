# This is a modified version of the external tool chain package
#  'external-csl-toolchain_2010q1-202.bb'.
# It is used to build the Community Firmware.

# Changes to 2010q1 are:
#
# 1) The time zone database '/usr/share/zoneinfo' is now populated by using
#    recipe 'tzdata-squeezeos'. This allows it to be updated in future. A
#    run time dependency on 'tzdata-squeezeos' ensures that it will be
#    populated.


PR = "r1"

# Base package

require external-csl-toolchain_2010q1-202.bb


# Override SRC_URI, because the base package incorporates a parameterized
# version number ${PV}, which no longer matches up.

SRC_URI = "https://sourcery.sw.siemens.com/public/gnu_toolchain/arm-none-linux-gnueabi/arm-2010q1-202-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2 \
        file://SUPPORTED"


# Remove timezone data and getconf shared libraries installed by base package.

do_install_append() {
        rm -r ${D}/usr/share/zoneinfo
        rm    ${D}/etc/localtime
        rm -r ${D}/usr/libexec/getconf
}


# Add run time dependency on 'tzdata-squeezeos' to populate
# '/usr/share/zoneinfo' and create '/etc/localtime' initial link.

RDEPENDS += "tzdata-squeezeos"


# Always clean our "automatic" dependency 'tzdata-squeezeos' whenever we
# clean this package. (Helps to avoid confusion if switching tool chain.)

do_clean[depends] += "tzdata-squeezeos:do_clean"
