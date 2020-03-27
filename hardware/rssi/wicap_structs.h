/**
 * @file wicap_structs.h
 * @date 2017-08-24
 * @author niym
 * @source https://github.com/niym/wicap
 * @brief Using libpcap to capture 802.11 wifi packets,
 * and then parsing radiotap header and 802.11 frame header.
 * @date 2020-02-10
 * @author Zachary Smeton (zsmeton@yahoo.com)
 * @brief Pulled structs from wicap.h and wicap.cc and c++ified
 */
#ifndef WICAP_STRUCTS_H
#define WICAP_STRUCTS_H

#include "radiotap_iter.h"
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

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

/**
 * @brief Stores radiotap and generic header information
 *
 */
struct RadiotapData {
  char *srcAddr;
  char *destAddr;
  char *bssid;
  double rate;
  u_int16_t channelFrequency;
  int8_t dbmAntsignal;
  int8_t dbmAntnoise;
  int8_t dbAntsignal;
  int8_t dbAntnoise;

  RadiotapData()
      : srcAddr(nullptr), destAddr(nullptr), bssid(nullptr), rate(-1),
        channelFrequency(0), dbmAntsignal(0), dbmAntnoise(0), dbAntsignal(0),
        dbAntnoise(0) {}
};

/**
 * @brief IEEE80211 Network Header
 *
 */
struct IEEE80211Hdr {
  unsigned short frame_control;
  unsigned short duration_id;
  unsigned char addr1[6];
  unsigned char addr2[6];
  unsigned char addr3[6];
  unsigned short seq_ctrl;
  unsigned short addr4[6];
} __attribute__((packed));

/**
 * @brief The align_sizes for radiotap data
 *
 */
static const struct radiotap_align_size AlignSize000000_00[53] = {
    {1, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
    {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
    {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
    {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
    {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
    {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {1, 4}};

/**
 * @brief Vendor namespace data
 *
 */
static const ieee80211_radiotap_namespace vnsArray[] = {
    {
        .align_size = AlignSize000000_00,
        .n_bits = sizeof(AlignSize000000_00),
        .oui = 0x000000,
        .subns = 0,
    },
};

/**
 * @brief Vender namespace struct
 *
 */
static const struct ieee80211_radiotap_vendor_namespaces vns = {
    .ns = vnsArray,
    .n_ns = sizeof(vnsArray) / sizeof(vnsArray[0]),
};

static const struct radiotap_override overrides[] = {{
    .field = 14,
    .align = 4,
    .size = 4,
}};

#endif