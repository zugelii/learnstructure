#!/bin/sh
iptables -t nat -F
iptables -t nat -A PREROUTING -p tcp -i eth1 --dport 180 -j DNAT --to 192.168.1.10:80
iptables -t nat -A POSTROUTING -p tcp -d 192.168.1.10 --dport 80 -j SNAT --to 192.168.1.25
iptables -t nat -A PREROUTING -p tcp -d 192.168.46.0/24 --dport 180 -j DNAT --to 192.168.1.10:80

echo 1 > /proc/sys/net/ipv4/ip_forward


