FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
    file://dts.patch \
"



do_configure_prepend () {
	# Function support for usb serial
	echo "CONFIG_USB_SERIAL_PL2303=y"   >> ${B}/.config
	echo "CONFIG_USB_SERIAL_FTDI_SIO=y" >> ${B}/.config
	echo "CONFIG_USB_SERIAL_CONSOLE=y"  >> ${B}/.config
	echo "CONFIG_FUSE_FS=y" 		>> ${B}/.config
	echo "CONFIG_CUSE=y"		>> ${B}/.config
	echo "CONFIG_IP_ADVANCED_ROUTER=y"  >> ${B}/.config
	echo "CONFIG_IP_MULTIPLE_TABLES=y"  >> ${B}/.config
	
	# Function support for squashfs (needed for rauc bundle handling)
	echo "CONFIG_BLK_DEV_LOOP=y"	    >> ${B}/.config
	echo "CONFIG_SQUASHFS=y"		    >> ${B}/.config
	echo "CONFIG_SQUASHFS_FILE_CASH=y"	    >> ${B}/.config
	echo "CONFIG_SQUASHFS_DECOMP_SINGLE=y"  >> ${B}/.config
	echo "CONFIG_SQUASHFS_ZLIB=y"	    >> ${B}/.config
	echo "CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE=3" >> ${B}/.config

        #support ip tables
	echo "CONFIG_NF_CONNTRACK=y"                       >> ${B}/.config
	echo "CONFIG_NF_CONNTRACK_FTP=y"          >> ${B}/.config
	echo "CONFIG_NF_CONNTRACK_TFTP=y"          >> ${B}/.config
	echo "CONFIG_NF_CT_NETLINK=y"          >> ${B}/.config
	echo "CONFIG_NF_CT_NETLINK_TIMEOUT=y"          >> ${B}/.config
	echo "CONFIG_NF_TABLES=y"          >> ${B}/.config
	echo "CONFIG_NF_TABLES_INET=y"          >> ${B}/.config
	echo "CONFIG_NF_TABLES_NETDEV=y"          >> ${B}/.config
	echo "CONFIG_NFT_RT=y"          >> ${B}/.config
	echo "CONFIG_NFT_LIMIT=y"          >> ${B}/.config
	echo "CONFIG_NFT_MASQ=y"          >> ${B}/.config
	echo "CONFIG_NFT_REDIR=y"          >> ${B}/.config
	echo "CONFIG_NFT_NAT=y"          >> ${B}/.config
	echo "CONFIG_NFT_FWD_NETDEV=y"          >> ${B}/.config
	echo "CONFIG_NF_CONNTRACK_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NF_SOCKET_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NFT_CHAIN_ROUTE_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NFT_DUP_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NFT_FIB_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NF_TABLES_ARP=y"          >> ${B}/.config
	echo "CONFIG_NF_LOG_ARP=y"          >> ${B}/.config
	echo "CONFIG_NFT_CHAIN_NAT_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NFT_MASQ_IPV4=y"          >> ${B}/.config
	echo "CONFIG_NFT_REDIR_IPV4=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_IPTABLES=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_FILTER=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_TARGET_REJECT=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_NAT=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_TARGET_MASQUERADE=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_TARGET_NETMAP=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_TARGET_REDIRECT=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_MANGLE=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_RAW=y"          >> ${B}/.config
	echo "CONFIG_IP_NF_SECURITY=y"          >> ${B}/.config
	echo "CONFIG_NF_CONNTRACK_IPV6=y"          >> ${B}/.config
	echo "CONFIG_NF_SOCKET_IPV6=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_IPTABLES=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_MATCH_IPV6HEADER=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_FILTER=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_TARGET_REJECT=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_MANGLE=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_RAW=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_NAT=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_TARGET_MASQUERADE=y"          >> ${B}/.config
	echo "CONFIG_IP6_NF_TARGET_NPT=y"          >> ${B}/.config
}
