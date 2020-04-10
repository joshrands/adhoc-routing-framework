/**
 * @file radiotap_capture.cc
 * @author niym
 * @source https://github.com/niym/wicap
 * @brief Using libpcap to capture 802.11 wifi packets,
 * and then parsing radiotap header and 802.11 frame header.
 * @author Zachary Smeton (zsmeton@yahoo.com)
 * @brief Restructuring of wicap to be a class
 * @date 03-09-2020
 *
 */

#include "radiotap_capture.h"

void setRadiotapNamespace(struct ieee80211_radiotap_iterator *iter,
                                 RadiotapData &data) {
  switch (iter->this_arg_index) {
  case IEEE80211_RADIOTAP_RATE:
    data.rate = (double)*iter->this_arg / 2;
    break;
  case IEEE80211_RADIOTAP_CHANNEL:
    data.channelFrequency = (u_int16_t)*iter->this_arg;
    break;
  case IEEE80211_RADIOTAP_FHSS:
  case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
    data.dbmAntsignal = (int8_t)*iter->this_arg;
    break;
  case IEEE80211_RADIOTAP_DBM_ANTNOISE:
    data.dbmAntnoise = (int8_t)*iter->this_arg;
    break;
  case IEEE80211_RADIOTAP_LOCK_QUALITY:
  case IEEE80211_RADIOTAP_TX_ATTENUATION:
  case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
  case IEEE80211_RADIOTAP_DBM_TX_POWER:
  case IEEE80211_RADIOTAP_ANTENNA:
  case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
    data.dbAntsignal = (int8_t)*iter->this_arg;
    break;
  case IEEE80211_RADIOTAP_DB_ANTNOISE:
    data.dbAntnoise = (int8_t)*iter->this_arg;
    break;
  default:
    break;
  }
}

RadiotapData getRadiotapData(u_char *args, const struct pcap_pkthdr *header,
                             const u_char *packet) {
  int err;
  ieee80211_radiotap_iterator iter;
  RadiotapData data;

  err = ieee80211_radiotap_iterator_init(
      &iter, (struct ieee80211_radiotap_header *)packet, 2014, &vns);
  if (err) {
    fprintf(stderr,
            "[ERROR]:[RADIOTAP]: malformed radiotap header (init returns %d)\n",
            err);
    return data;
  }

  /**
   * Parsing captured data packet and print radiotap information.
   */
  while (!(err = ieee80211_radiotap_iterator_next(&iter))) {
    if (iter.is_radiotap_ns)
      setRadiotapNamespace(&iter, data);
  }

  if (err != -ENOENT) {
    fprintf(stderr, "[ERROR]:[RADIOTAP]:malformed radiotap data\n");
    return data;
  }

  struct IEEE80211Hdr *hdr = (struct IEEE80211Hdr *)(packet + iter._max_length);

  // set the source, dest, an bssid addresses
  data.srcAddr = (char *)malloc(sizeof(char) * 30);
  snprintf(data.srcAddr, 30, MACSTR, MAC2STR(hdr->addr2));
  data.destAddr = (char *)malloc(sizeof(char) * 30);
  snprintf(data.destAddr, 30, MACSTR, MAC2STR(hdr->addr1));
  data.bssid = (char *)malloc(sizeof(char) * 30);
  snprintf(data.bssid, 30, MACSTR, MAC2STR(hdr->addr3));

  return data;
}