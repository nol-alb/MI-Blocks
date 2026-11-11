# Setting Up The Network

The jetson needs to be in *access point mode*, it becomes a router and broadcasts its own WiFi network that other devices can connect to. 

switching back and forth includes:

# back to GTvisitor
nmcli con up GTvisitor

# back to hotspot
nmcli con up MI-Blocks-AP

# setup server
cd github/MI-Blocks
source legoenv/bin/activate
cd jetson
python osc_server.py

# Audio setup
- first run jackd -d alsa -d hw:USB -r 48000 -p 512 -n 3
- then pd Media -> Jack
