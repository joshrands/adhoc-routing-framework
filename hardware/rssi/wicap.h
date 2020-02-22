/**
 * @file wicap.h
 * @date 2017-08-24
 * @author niym Sep 24, 2017
 * @source https://github.com/niym/wicap
 * @brief Using libpcap to capture 802.11 wifi packets, 
 * and then parsing radiotap header and 802.11 frame header.
 * @date 2020-02-10
 * @author Zachary Smeton (zsmeton@yahoo.com)
 * @brief Modified software to be use c++ instead of c
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <errno.h>
#include <string.h>
#include <pcap/pcap.h>
#include <cstdlib>
#include "radiotap_iter.h"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

struct ieee80211_hdr;

int mac_in_network(const u_char *packet);

static void print_radiotap_namespace(struct ieee80211_radiotap_iterator *iter);

static void print_test_namespace(struct ieee80211_radiotap_iterator *iter);

int save_packet(const pcap_t *pcap_handle, const struct pcap_pkthdr *header, 
        const u_char *packet, const char *file_name);

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);