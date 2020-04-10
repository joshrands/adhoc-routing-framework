/**
 * @file radiotap_capture.h
 * @date 2017-08-24
 * @author niym
 * @source https://github.com/niym/wicap
 * @brief Using libpcap to capture 802.11 wifi packets,
 * and then parsing radiotap header and 802.11 frame header.
 * @date 2020-02-10
 * @author Zachary Smeton (zsmeton@yahoo.com)
 * @brief Modified to use c++
 */

#ifndef RADIOTAP_CAPTURE_H
#define RADIOTAP_CAPTURE_H

#include "radiotap_iter.h"
#include "wicap_structs.h"
#include <cstdlib>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void setRadiotapNamespace(struct ieee80211_radiotap_iterator *iter,
                                 RadiotapData &data);
RadiotapData getRadiotapData(u_char *args, const struct pcap_pkthdr *header,
                             const u_char *packet);

#endif