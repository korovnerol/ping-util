#include "packet_builder.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <linux/if_ether.h>

static uint16_t checksum(const void *data, size_t len){
    const uint16_t *buf = data;
    uint32_t sum = 0;
    for (; len > 1; len -= 2)
        sum += *buf++;
    if (len)
        sum += *(uint8_t*)buf;
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    return ~sum;
}

size_t build_icmp_echo_request(uint8_t *buffer,
                               const uint8_t *src_mac,
                               const uint8_t *dst_mac,
                               uint32_t src_ip,
                               uint32_t dst_ip,
                               uint16_t id,
                               uint16_t seq){
    struct ethhdr *eth = (struct ethhdr*)buffer;
    memcpy(eth->h_dest, dst_mac, 6);
    memcpy(eth->h_source, src_mac, 6);
    eth->h_proto = htons(ETH_P_IP);

    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ip->version = 4;
    ip->ihl = 5;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
    ip->id = htons(0);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP;
    ip->check = 0;
    ip->saddr = src_ip;
    ip->daddr = dst_ip;
    ip->check = checksum(ip, sizeof(struct iphdr));

    struct icmphdr *icmp = (struct icmphdr*)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = htons(id);
    icmp->un.echo.sequence = htons(seq);
    icmp->checksum = 0;
    icmp->checksum = checksum(icmp, sizeof(struct icmphdr));

    return sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct icmphdr);
}
