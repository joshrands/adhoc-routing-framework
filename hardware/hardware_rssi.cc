#include "hardware_rssi.h"

/**
 * @brief Construct a new Hardware RSSI object
 *
 */
HardwareRSSI::HardwareRSSI() {
  // Set up libpcap
  handle = pcap_open_live(INTERFACE_NAME, BUFSIZ, 1, 100, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "[ERROR]:[RSSI]:Couldn't open device %s: %s\n", INTERFACE_NAME, errbuf);
    exit(2);
  }
  /* Compile and apply the filter */
  if (pcap_compile(handle, &fp, RSSI_NETWORK_FILER, 0, netp) == -1) {
    fprintf(stderr, "[ERROR]:[RSSI]:Couldn't parse filter %s: %s\n", RSSI_NETWORK_FILER,
            pcap_geterr(handle));
    exit(2);
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "[ERROR]:[RSSI]:Couldn't set filter %s: %s\n", RSSI_NETWORK_FILER,
            pcap_geterr(handle));
    exit(2);
  }
  // run capturing thread
  capturing = thread(&HardwareRSSI::captureData, this);
  capturing.detach();
}

HardwareRSSI::~HardwareRSSI() {
  /* cleanup */
  pcap_freecode(&fp);
  pcap_close(handle);
}

/**
 * @brief captures and handles a single packet
 *
 */
void _capturePacket(u_char *args, const struct pcap_pkthdr *header,
                        const u_char *packet) {
  HardwareRSSI *capturer = reinterpret_cast<HardwareRSSI *>(args);
  RadiotapData defaultData;
  RadiotapData data = getRadiotapData(args, header, packet);
  if(data.dbmAntsignal != defaultData.dbmAntsignal){
    IP_ADDR ip;
    if(capturer->MACToIP.find(data.srcAddr,ip)){
      capturer->rssiMap.insert(ip, data.dbmAntsignal);
    } else {
      fprintf(stderr, "[ERROR]:[RSSI]: Invalid MAC Address: %s\n",
              data.srcAddr);
    }
  }
}

/**
 * @brief Continuously captures packets and stores the more recent rssi data
 *
 */
void HardwareRSSI::captureData() {
  while (true) {
    /* now we can set our callback function */
    pcap_loop(handle, 0, _capturePacket, (u_char *)this);
  }
}