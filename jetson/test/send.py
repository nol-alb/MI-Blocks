from pythonosc.udp_client import SimpleUDPClient

import time

client = SimpleUDPClient('10.42.0.1',5005)
count = 0
while True:
	client.send_message('/xiao/slider01/value',count)
	print(f'sent: {count}')
	count += 1
	time.sleep(0.5)

