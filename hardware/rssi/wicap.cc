#include "wicap.h"

static int fcshdr = 0;

struct ieee80211_hdr {
	unsigned short frame_control;
	unsigned short duration_id;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char addr3[6];
	unsigned short seq_ctrl;
	unsigned short addr4[6];
} __attribute__ ((packed));

static const struct radiotap_align_size align_size_000000_00[53] = {{1,4}, 
{4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, 
{4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, 
{4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, 
{4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, 
{4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {4,4}, {1,4}};

static const ieee80211_radiotap_namespace vns_array[] = {																															
	{
		.align_size = align_size_000000_00,
		.n_bits = sizeof(align_size_000000_00),
		.oui = 0x000000,
		.subns = 0,
	},
};

static const struct ieee80211_radiotap_vendor_namespaces vns = {
	.ns = vns_array,
	.n_ns = sizeof(vns_array)/sizeof(vns_array[0]),
};


int mac_in_network(const u_char *packet){	
	int err = 0;
	struct ieee80211_radiotap_iterator iter;
    err = ieee80211_radiotap_iterator_init(&iter,(struct ieee80211_radiotap_header*) packet, 2014, &vns);
	if (err) {
		printf("malformed radiotap header (init returns %d)\n", err);
		return 1;
	}
	struct ieee80211_hdr *hdr =(struct ieee80211_hdr *)(packet + iter._max_length);
	char* mac_str = (char*) malloc(sizeof(char)*19);
    sprintf(mac_str, MACSTR, MAC2STR(hdr->addr2));
   	return(strcmp("b8:27:eb:87:af:f5", mac_str));
}

static void print_radiotap_namespace(struct ieee80211_radiotap_iterator *iter)
{
	switch (iter->this_arg_index) {
	case IEEE80211_RADIOTAP_TSFT:
		printf("\tTSFT: %lu\n", le64toh(*(unsigned long long *)iter->this_arg));
		break;
	case IEEE80211_RADIOTAP_FLAGS:
		printf("\tflags: %02x\n", *iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_RATE:
		printf("\trate: %lf\n", (double)*iter->this_arg/2);
		break;
	case IEEE80211_RADIOTAP_CHANNEL:
		printf("\tchannel frequency: %d\n", (u_int16_t)*iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_FHSS:
	case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
		printf("\tdbm antsignal: %d\n", (int8_t)*iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_DBM_ANTNOISE:
		printf("\tdbm antnoise: %d\n", (int8_t)*iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_LOCK_QUALITY:
	case IEEE80211_RADIOTAP_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DBM_TX_POWER:
	case IEEE80211_RADIOTAP_ANTENNA:
	case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
		printf("\tdb antsignal: %d\n", (int8_t)*iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_DB_ANTNOISE:
		printf("\tdb antnoise: %d\n", (int8_t)*iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_TX_FLAGS:
		break;
	case IEEE80211_RADIOTAP_RX_FLAGS:
		if (fcshdr) {
			printf("\tFCS in header: %.8x\n",
				le32toh(*(uint32_t *)iter->this_arg));
			break;
		}
		printf("\tRX flags: %#.4x\n",
			le16toh(*(uint16_t *)iter->this_arg));
		break;
	case IEEE80211_RADIOTAP_RTS_RETRIES:
	case IEEE80211_RADIOTAP_DATA_RETRIES:
		break;
		break;
	default:
		printf("\tBOGUS DATA\n");
		break;
	}
}

static void print_test_namespace(struct ieee80211_radiotap_iterator *iter)
{
	switch (iter->this_arg_index) {
	case 0:
	case 52:
		printf("\t00:00:00-00|%d: %.2x/%.2x/%.2x/%.2x\n",
			iter->this_arg_index,
			*iter->this_arg, *(iter->this_arg + 1),
			*(iter->this_arg + 2), *(iter->this_arg + 3));
		break;
	default:
		printf("\tBOGUS DATA - vendor ns %d\n", iter->this_arg_index);
		break;
	}
}

static const struct radiotap_override overrides[] = {
	{ .field = 14, .align = 4, .size = 4, }
};


void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{	
	if(mac_in_network(packet)) return;

	static int count = 1;
    int err;
    int i;
	struct ieee80211_radiotap_iterator iter;

    printf("\n Packet (%d):\n", count);

    err = ieee80211_radiotap_iterator_init(&iter,(struct ieee80211_radiotap_header *)packet, 2014, &vns);
	if (err) {
		printf("malformed radiotap header (init returns %d)\n", err);
		return;
	}

    /**
     * Parsing captured data packet and print radiotap information.
     */
    while (!(err = ieee80211_radiotap_iterator_next(&iter))) {
		if (iter.this_arg_index == IEEE80211_RADIOTAP_VENDOR_NAMESPACE) {
			printf("\tvendor NS (%.2x-%.2x-%.2x:%d, %d bytes)\n",
				iter.this_arg[0], iter.this_arg[1],
				iter.this_arg[2], iter.this_arg[3],
				iter.this_arg_size - 6);
			for (i = 6; i < iter.this_arg_size; i++) {
				if (i % 8 == 6)
					printf("\t\t");
				else
					printf(" ");
				printf("%.2x", iter.this_arg[i]);
			}
			printf("\n");
		} else if (iter.is_radiotap_ns)
			print_radiotap_namespace(&iter);
		else if (iter.current_namespace == &vns_array[0])
			print_test_namespace(&iter);
	}

	if (err != -ENOENT) {
		printf("malformed radiotap data\n");
		return;
	}

    struct ieee80211_hdr *hdr =(struct ieee80211_hdr *)(packet + iter._max_length);
    printf("\tsource address: " MACSTR "\n", MAC2STR(hdr->addr2));
    printf("\tdestination address: " MACSTR "\n", MAC2STR(hdr->addr1));
    printf("\tbssid: " MACSTR "\n", MAC2STR(hdr->addr3));
	count ++;
	
	return;
}


