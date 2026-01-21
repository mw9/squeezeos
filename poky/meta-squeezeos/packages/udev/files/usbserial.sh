#!/bin/sh
#
# Called from udev
#
# Start/stop getty for usb-serial-dongles logins
#
port="`basename "$DEVNAME"`"

if [ -z "$port" ]; then
	port=ttyUSB0
fi

if [ -z "$ACTION"]; then
	ACTION=add
fi

case $ACTION in
	add)
		$(while true; do /sbin/getty $port 115200 VC vt100; done) &
		;;
	remove)
		if [ -x /usr/bin/killall ]; then
			killall "`basename "$0"`"
		fi
		;;
esac

exit 0
