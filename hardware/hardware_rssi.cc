#include "hardware_rssi.h"

/**
 * @brief Construct a new Hardware RSSI object
 *
 */
HardwareRSSI::HardwareRSSI(bool _collectAll) : collectAll(_collectAll), count(0) {
  // Set up libpcap
  handle = pcap_open_live(INTERFACE_NAME, BUFSIZ, 1, 100, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "[RSSI]:[ERROR]: Couldn't open device %s: %s\n", INTERFACE_NAME, errbuf);
    exit(2);
  }
  /* Compile and apply the filter */
  if (pcap_compile(handle, &fp, RSSI_NETWORK_FILER, 0, netp) == -1) {
    fprintf(stderr, "[RSSI]:[ERROR]: Couldn't parse filter %s: %s\n", RSSI_NETWORK_FILER,
            pcap_geterr(handle));
    exit(2);
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "[RSSI]:[ERROR]: Couldn't set filter %s: %s\n", RSSI_NETWORK_FILER,
            pcap_geterr(handle));
    exit(2);
  }
}

HardwareRSSI::~HardwareRSSI() {
  /* cleanup */
  pcap_freecode(&fp);
  pcap_close(handle);
}


void _capturePacket(u_char *args, const struct pcap_pkthdr *header,
                        const u_char *packet) {
  HardwareRSSI *capturer = reinterpret_cast<HardwareRSSI *>(args);
  RadiotapData defaultData;
  RadiotapData data = getRadiotapData(args, header, packet);
  if(data.dbmAntsignal != defaultData.dbmAntsignal){
    IP_ADDR ip;
    if(capturer->collectAll || capturer->MACToIP.find(string(strdup(data.srcAddr)), ip)){
      capturer->rssiMap.insert(data.srcAddr, data.dbmAntsignal);
      capturer->count ++;
      if(RSSI_DEBUG)
        printf("[RSSI]:[DEBUG]: RSSI data for %s -> %d dbm\n", data.srcAddr, data.dbmAntsignal);
    }
  }
}

void HardwareRSSI::captureData() {
  while (true) {
    /* now we can set our callback function */
    pcap_loop(handle, 0, _capturePacket, (u_char *)this);
  }
}

void HardwareRSSI::addNeighbors(const char* mac, IP_ADDR ip){
  MACToIP.insert(string(strdup(mac)), ip);
  IPToMAC.insert(ip, string(strdup(mac)));
}

set<IP_ADDR> HardwareRSSI::getIPNeighbors(){
  throw(runtime_error("TODO: Implement getIPNeighbors (Add .keys() to SafeHashMap)"));
  return set<IP_ADDR>();
}

set<const char*> HardwareRSSI::getMACNeighbors(){
  throw(runtime_error("TODO: Implement getMACNeighbors (Add .keys() to SafeHashMap)"));
  return set<const char*>();
}

set<IP_ADDR> HardwareRSSI::getIPAvailable(){
  throw(runtime_error("TODO: Implement getMACNeighbors (Add .keys() to SafeHashMap)"));
  return set<IP_ADDR>();
}

set<const char*> HardwareRSSI::getMACAvailable(){
  throw(runtime_error("TODO: Implement getMACAvailable (Add .keys() to SafeHashMap)"));
  return set<const char*>();
}

int HardwareRSSI::getRSSI(IP_ADDR ip){
  string mac;
  if(IPToMAC.find(ip, mac)){
    int rssi;
    if(rssiMap.find(mac, rssi)){
      return rssi;
    }
  }

  return 0;
}

int HardwareRSSI::getRSSI(const char* mac){
  int rssi;
  if(rssiMap.find(string(strdup(mac)),rssi)){
    return rssi;
  }

  return 0;
}