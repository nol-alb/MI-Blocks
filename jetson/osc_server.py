from pythonosc.dispatcher import Dispatcher
from pythonosc.osc_server import BlockingOSCUDPServer

JETSON_IP = "10.42.0.1"
OSC_PORT = 5005

connected_xiaos = {}

def handle_xiao(address, *args):
    xiao_id = address.split("/")[2]
    if xiao_id not in connected_xiaos:
        connected_xiaos[xiao_id] = True
        print(f"[NEW] XIAO connected: {xiao_id}")
    print(f"[{xiao_id}] count → {args[0]}")

dispatcher = Dispatcher()
dispatcher.map("/xiao/*", handle_xiao)

print(f"OSC server listening on {JETSON_IP}:{OSC_PORT}")
server = BlockingOSCUDPServer((JETSON_IP, OSC_PORT), dispatcher)
server.serve_forever()