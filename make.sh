#!/bin/sh
set -v
gcc -O2 -c bitcalc.c
gcc -s -o bitcalc.cgi bitcalc.o
sudo mv bitcalc.cgi /home/http/cgi-bin
sudo chown apache:apache /home/http/cgi-bin/bitcalc.cgi
