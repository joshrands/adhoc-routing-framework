#include "radiotap_capture.h"

static void printPacket(u_char *args, const struct pcap_pkthdr *header,
                        const u_char *packet) {
  RadiotapData defaultData;
  static int count = 0;
  RadiotapData data = getRadiotapData(args, header, packet);

  printf("Packet(%d)\n", count);
  if (data.srcAddr != defaultData.srcAddr)
    printf("\tSource Address: %s\n", data.srcAddr);
  if (data.destAddr != defaultData.destAddr)
    printf("\tDestination Address: %s\n", data.destAddr);
  if (data.bssid != defaultData.bssid)
    printf("\tBSSID: %s\n", data.bssid);
  if (data.channelFrequency != defaultData.channelFrequency)
    printf("\tChannel Frequency: %d\n", data.channelFrequency);
  if (data.dbmAntsignal != defaultData.dbmAntsignal)
    printf("\tRSSI (dbm): %d\n", data.dbmAntsignal);
  if (data.dbAntsignal != defaultData.dbAntsignal)
    printf("\tRSSI (db): %d\n", data.dbAntsignal);
  
  count++;
}

int main(int argc, char *argv[]) {
  pcap_t *handle; /* Session handle */
  char *dev;      /* The device to sniff on */
  bpf_u_int32 netp;
  char errbuf[PCAP_ERRBUF_SIZE]; /* Error string */
  struct bpf_program fp;         /* The compiled filter */
  char filterExp[] = "";         /* The filter expression */
  int numPackets = 0;            /* number of packets to capture */

  /* check for capture device name on command-line */
  if (argc == 2) {
    dev = argv[1];
  } else {
    fprintf(stderr, "error: unrecognized command-line options\n\n");
    return 1;
  }

  /* Open the session in promiscuous mode */
  handle = pcap_open_live(dev, BUFSIZ, 1, 100, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
    return (2);
  }
  /* Compile and apply the filter */
  if (pcap_compile(handle, &fp, filterExp, 0, netp) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filterExp,
            pcap_geterr(handle));
    return (2);
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filterExp,
            pcap_geterr(handle));
    return (2);
  }

  /* print capture info */
  printf("Device: %s\n", dev);
  printf("Number of packets: %d\n", numPackets);
  printf("Filter expression: %s\n", filterExp);

  /* now we can set our callback function */
  pcap_loop(handle, numPackets, printPacket, (u_char *)handle);

  /* cleanup */
  pcap_freecode(&fp);
  pcap_close(handle);

  printf("\nCapture complete.\n");

  return (0);
}