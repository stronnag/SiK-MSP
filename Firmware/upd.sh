#!/bin/sh

## SiK 1.9-MSP on HM-TRP

python2 tools/uploader.py --port /dev/ttyUSB0 \
 --baudrate 19200 \
 obj/hm_trp/radio~hm_trp/radio~hm_trp.ihx
