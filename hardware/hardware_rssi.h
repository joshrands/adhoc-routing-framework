#ifndef HARDWAREAODV_H
#define HARDWAREAODV_H

#include "hardware_defines.h"
#include "radiotap_capture.h"
#include "safe_hash_map.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>

using namespace std;

class HardwareRSSI {
protected:
  SafeHashMap<IP_ADDR, int> rssiMap;
  SafeHashMap<char *, IP_ADDR> MACToIP;
  thread capturing;
  pcap_t *handle; /* Session handle */
  bpf_u_int32 netp;
  char errbuf[PCAP_ERRBUF_SIZE]; /* Error string */
  struct bpf_program fp;         /* The compiled filter */

public:
  // Constructors
  HardwareRSSI();
  ~HardwareRSSI();
  void captureData();
  friend void _capturePacket(u_char *args, const struct pcap_pkthdr *header,
                             const u_char *packet);
};

#endif