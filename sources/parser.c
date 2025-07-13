#include "parser.h"
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <linux/if_ether.h>
#include <string.h>

int is_icmp_echo_reply(const uint8_t *frame, size_t len, uint16_t id){
    if (len < sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct icmphdr)) return 0;

    const struct ethhdr *eth = (const struct ethhdr*)frame;
    if (ntohs(eth->h_proto) != ETH_P_IP) return 0;

    const struct iphdr *ip = (const struct iphdr*)(frame + sizeof(struct ethhdr));
    if (ip->protocol != IPPROTO_ICMP) return 0;

    const struct icmphdr *icmp = (const struct icmphdr*)(frame + sizeof(struct ethhdr) + ip->ihl * 4);
    if (icmp->type != ICMP_ECHOREPLY) return 0;
    if (ntohs(icmp->un.echo.id) != id) return 0;

    return 1;
}

void extract_source_mac(const uint8_t *frame, uint8_t *mac_out){
    memcpy(mac_out, frame + 6, 6);
}
