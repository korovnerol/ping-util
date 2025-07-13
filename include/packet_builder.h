#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

#include <stdint.h>
#include <stddef.h>

#define MAX_FRAME_SIZE 1514

size_t build_icmp_echo_request( uint8_t *buffer,
                                const uint8_t *src_mac,
                                const uint8_t *dst_mac,
                                uint32_t src_ip,
                                uint32_t dst_ip,
                                uint16_t id,
                                uint16_t seq);

#endif