#ifndef HARDWARERSSI_H
#define HARDWARERSSI_H

#include "hardware_defines.h"
#include "radiotap_capture.h"
#include "safe_hash_map.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include <set>
#include <stdexcept>

using namespace std;


/**
 * @brief Hardware RSSI is a class which uses libpcap to gather rssi data on sniffed packets
 * HardwareRSSI uses thread safe hashmaps for completetely safe concurrent access to the map
 * 
 * There are two modes:
 * - collectAll: Collects data about all sniffed packets
 * - selective (default): Collects data only on addresses which are added to the listen list
 * 
 * Usage:
 * 
 * 
 */
class HardwareRSSI {
protected:
  SafeHashMap<char *, int> rssiMap;
  SafeHashMap<char *, IP_ADDR> MACToIP;
  SafeHashMap<IP_ADDR, char *> IPToMAC;
  pcap_t *handle; /* Session handle */
  bpf_u_int32 netp;
  char errbuf[PCAP_ERRBUF_SIZE]; /* Error string */
  struct bpf_program fp;         /* The compiled filter */
  bool collectAll;

public:
  // Constructors
  /**
   * @brief Construct a new HardwareRSSI object
   * 
   * @param _collectAll whether rssi data about all mac addresses should be stored (if not the mac address must be manually added)
   */
  HardwareRSSI(bool _collectAll=false);

  ~HardwareRSSI();

  /**
   * @brief Continuously captures packets and stores the more recent rssi data
   *
   */
  void captureData();
  
  /**
   * @brief captures and handles a single packet, friend of HardwareRSSI
   *
   */
  friend void _capturePacket(u_char *args, const struct pcap_pkthdr *header,
                             const u_char *packet);
  
  
  /**
   * @brief Adds the mac address to th
   * 
   * @param mac 
   */
  void addNeighbors(char* mac, IP_ADDR ip);

  /**
   * @brief Returns the set of all known IP addresses. Theses addresses must have been added to the listening set via addNeighbors()
   * 
   * @return set<IP_ADDR>
   */
  set<IP_ADDR> getIPNeighbors();

  /**
   * @brief Returns the set of all known MAC addresses. Theses addresses must have been added to the listening set via addNeighbors()
   * 
   * @return set<char*> 
   */
  set<char*> getMACNeighbors();

  /**
   * @brief Returns the set of IP Addresses with available data. Even when in collectAll mode getIPAvailable() is a subset of getIPNeighbors()
   * 
   * @return set<IP_ADDR> ip addresses
   */
  set<IP_ADDR> getIPAvailable();

  /**
   * @brief Returns the set of MAC Addresses with available data
   * 
   * @return set<char*> mac addresses
   */
  set<char*> getMACAvailable();

  /**
   * @brief Gets the rssi data for the specificied IP Address
   * 
   * @param ip the ip address
   * @return int the rssi (dbm) (0 on error)
   */
  int getRSSI(IP_ADDR ip);

  /**
   * @brief Gets the rssi data for the specificied MAC Address
   * 
   * @param mac the MAC address
   * @return int the rssi (dbm)
   */
  int getRSSI(char* mac);
};

#endif