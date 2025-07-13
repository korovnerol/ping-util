#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include "socket_utils.h"
#include "packet_builder.h"
#include "parser.h"

int main(int argc, char *argv[]){
    if (argc != 3 && argc != 4){
        fprintf(stderr, "Usage: %s <interface> <IPv4 address> [max waiting time]\n", argv[0]);
        return 1;
    }

    int timeout = 2;

    if(argc == 4){
        char *endptr;
        int val = strtol(argv[3], &endptr, 10);
        if (*endptr != '\0')
            printf("Invalid input format for the maximum waiting time, the default value (%d) will be set\n", timeout);
        else
            timeout = val;
    }

    const char *iface_name = argv[1];
    const char *target_ip_str = argv[2];

    struct interface_info iface;
    if (get_interface_info(iface_name, &iface) < 0){
        printf("get_interface_info FAILED\n");
        return 1;
    }

    uint32_t target_ip = inet_addr(target_ip_str);
    if (target_ip == INADDR_NONE){
        printf("Invalid IP address\n");
        return 1;
    }

    int sockfd = create_raw_socket(timeout);
    if (sockfd < 0){
        printf("Socket creating FAILED, try sudo\n");
        return 1;
    }

    uint8_t buffer[MAX_FRAME_SIZE];
    uint16_t id = getpid() & 0xFFFF;
    uint16_t seq = 1;

    uint8_t dst_mac[6] ={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    size_t frame_len = build_icmp_echo_request(buffer, iface.mac_addr, dst_mac, iface.ip_addr, target_ip, id, seq);

    struct sockaddr_ll addr;
    fill_sockaddr_ll(&addr, &iface, dst_mac);

    if (sendto(sockfd, buffer, frame_len, 0, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        printf("sendto FAILED\n");
        return 1;
    }

    while(1){
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (n < 0){
            printf("Timeout\n");
            break;
        }

        if (is_icmp_echo_reply(buffer, n, id)){
            uint8_t mac[6];
            extract_source_mac(buffer, mac);
            printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            break;
        }
    }
    close(sockfd);
    return 0;
}