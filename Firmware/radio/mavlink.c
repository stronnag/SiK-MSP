// -*- Mode: C; c-basic-offset: 8; -*-
//
// Copyright (c) 2012 Andrew Tridgell, All Rights Reserved
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  o Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  o Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//

///
/// @file	mavlink.c
///
/// mavlink reporting code
///

#include <stdarg.h>
#include "radio.h"
#include "packet.h"
#include "timer.h"

extern __xdata uint8_t pbuf[MAX_PACKET_LENGTH];

static uint8_t msp_calc_crc(uint8_t *buf, int len)
{
        uint8_t crc=0;
        int n;

        for(n = 0; n < len; n++)
        {
                crc ^= buf[n];
        }
        return crc;
}

/// send a MAVLink status report packet
void MAVLink_report(void)
{
/***
public struct MSP_RADIO
{
    public uint16 rxerrors;
    public uint16 fixed_errors;
    public uint8 localrssi;
    public uint8 remrssi;
    public uint8 txbuf;
    public uint8 noise;
    public uint8 remnoise;
}
 **/
        uint16_t rxerr = errors.rx_errors;
        uint16_t fixed = errors.corrected_packets;
        pbuf[0] = '$';
        pbuf[1] = 'M';
        pbuf[2] = '>';
        pbuf[3] = 9;
        pbuf[4] = 199;
        pbuf[5] = (rxerr >> 8); // rxerr
        pbuf[6] = (rxerr & 0xff); // rxerr
        pbuf[7] = (fixed >> 8); // fixederr
        pbuf[8] = (fixed & 0xff); // fixederr
        pbuf[9] = statistics.average_rssi;
        pbuf[10] = remote_statistics.average_rssi;
        pbuf[11] = serial_read_space();
        pbuf[12] = statistics.average_noise;
        pbuf[13] = remote_statistics.average_noise;
        pbuf[14] = msp_calc_crc(pbuf+3, 11);  // CRC
        if (serial_write_space() > 14)
                serial_write_buf(pbuf, 15);
}
