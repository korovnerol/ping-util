#include "socket_utils.h"
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <linux/if_ether.h>    // ETH_P_ALL, ETH_P_IP
#include <linux/if_packet.h>   // sockaddr_ll
#include <linux/if.h>          // struct ifreq, IFNAMSIZ

int get_interface_info(const char *if_name, struct interface_info *info){
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) return -1;

    struct ifreq ifr;
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ);

    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) return -1;
    info->if_index = ifr.ifr_ifindex;

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) return -1;
    memcpy(info->mac_addr, ifr.ifr_hwaddr.sa_data, 6);

    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) return -1;
    info->ip_addr = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;

    close(fd);
    return 0;
}

int create_raw_socket(int maxtime){
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    struct timeval timeout ={maxtime, 0}; // 2 секунды
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    return fd;
}

void fill_sockaddr_ll(struct sockaddr_ll *addr, const struct interface_info *iface, const uint8_t *dst_mac){
    memset(addr, 0, sizeof(*addr));
    addr->sll_family = AF_PACKET;
    addr->sll_protocol = htons(ETH_P_IP);
    addr->sll_ifindex = iface->if_index;
    addr->sll_halen = 6;
    memcpy(addr->sll_addr, dst_mac, 6);
}