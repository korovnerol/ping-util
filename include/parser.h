#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stddef.h>

int is_icmp_echo_reply(const uint8_t *frame, size_t len, uint16_t id);
void extract_source_mac(const uint8_t *frame, uint8_t *mac_out);

#endif