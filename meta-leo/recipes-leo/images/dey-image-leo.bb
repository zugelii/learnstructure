#
# Copyright (c) 2019  AG and subsidiaries
#
DESCRIPTION = "Yocto Image  - Digi iMX8X SBC PRO based"
LICENSE	    = "CLOSED"


inherit core-image
inherit dey-image


IMAGE_FEATURES += " \
  ssh-server-dropbear \
  package-management \
  dey-network \
  dey-wireless \
"

#IMAGE_ROOTFS_EXTRA_SPACE = "102400"

# FIXME:  for some reason the line below (PACKAGE_CLASSES) is ignored in this file and needs
#	  to be set to package_deb in local.conf
PACKAGE_CLASSES = "package_deb"
PACKAGE_FEED_URIS = "http://192.168.13.2:5678"

IMAGE_INSTALL_append = " dpkg  mosquitto \
	iperf3 mosquitto-clients chrony chronyc squashfs-tools vlan libcgroup tcpdump tzdata \
	libdigiapix screen htop stress"





mount_data_path() {
  echo "LABEL="data"    /data   ext4    defaults 0 0" >> ${IMAGE_ROOTFS}/etc/fstab
}
ROOTFS_POSTPROCESS_COMMAND += "mount_data_path; "
