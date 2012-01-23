#!/usr/bin/env python
# set air data rate

import serial, sys, optparse, time, fdpexpect

parser = optparse.OptionParser("show_regs")
parser.add_option("--baudrate", type='int', default=57600, help='baud rate')

opts, args = parser.parse_args()

if len(args) == 0:
    print("usage: show_regs.py <DEVICE...>")
    sys.exit(1)


def show_regs(device):
    '''show S registers'''
    port = serial.Serial(device, opts.baudrate, timeout=0,
                         dsrdtr=False, rtscts=False, xonxoff=False)
    ser = fdpexpect.fdspawn(port.fileno(), logfile=sys.stdout)
    ser.send('+++')
    time.sleep(1)
    ser.send('\r\nATI\r\n')
    try:
        ser.expect(['OK','SiK .* on HM-TRP'], timeout=2)
    except fdpexpect.TIMEOUT:
        print("timeout")
        return
    ser.send('ATI5\r\n')
    time.sleep(1)
    ser.read_nonblocking(300)
    ser.send('ATO\r\n')
    print("")
    time.sleep(1)
    port.close()

for d in args:
    print("showing registers on %s" % (d))
    show_regs(d)
    