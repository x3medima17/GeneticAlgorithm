import pymongo
import socket
import json

client = pymongo.MongoClient()
db = client.tsmi

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind(('0.0.0.0',8989))


while True:
	msg, addr = server.recvfrom(2**13)
	data = json.loads(msg)
	# print(msg)
	uuid = data["uuid"]
	project = data["project"]
	X = data["X"]
	action = data["action"]
	if action == "read":
		res = db.fitting_cache.find_one({
				"project" : project,
				"X" : X
			})
		if not res:
			out = dict(
				status=1,
				message="Miss",
				uuid=uuid
				)
		else:
			out = dict(
				status=0,
				message="Ok",
				uuid=uuid,
				delta=res["delta"]
				)
		server.sendto(json.dumps(out), addr)

	elif action == "write":
		delta = data["delta"]
		# continue
		res = db.fitting_cache.find_one({
				"project" : project,
				"X" : X
			})
		if not res:
			db.fitting_cache.insert({
				"project" : project,
				"X" : X,
				"delta" : delta
				})

	
