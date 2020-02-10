#ifndef MONITORINFO_H
#define MONITORINFO_H

/*********************************
 * monitor_info.h
 *
 * Struct for holding network monitoring information 
 * 
 * Author: Josh Rands
 * Date: 11/2/2019
 ********************************/

#include "defines.h"

struct local_data
{
    double batteryLevel;
};

struct pair_data
{
    IP_ADDR pairIp;
    double rss;
};

#endif