#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <stdint.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/if_packet.h>

struct interface_info{
    uint8_t mac_addr[6];
    uint32_t ip_addr;
    int if_index;
};

int get_interface_info(const char *if_name, struct interface_info *info);
int create_raw_socket(int timeout);
void fill_sockaddr_ll(struct sockaddr_ll *addr, const struct interface_info *iface, const uint8_t *dst_mac);

#endif