
#include <iostream>

#include <stdio.h>
#include <pcap.h>

using namespace std;

int main(int argc, char *argv[])
{
        pcap_t *handle;
		char *dev, errbuf[PCAP_ERRBUF_SIZE];

        handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
        if (handle == NULL) 
        {
            fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
            return(2);
        }

        printf("Device: %s\n", dev);

        return(0);

}
