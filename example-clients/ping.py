#!/usr/bin/python3

import socket
import time

IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, 8765))

count = 0
while True:
  if count < 5:
      print("Sending ping")
      client.sendall((1735289200).to_bytes(4, "little"))
  else:
      print("Sending termination")
      client.sendall((5).to_bytes(4, "little"))
  statusCode = int.from_bytes(client.recv(4, socket.MSG_WAITALL), "little")
  print("Response", statusCode)
  if count == 5:
      break
  time.sleep(1)
  count += 1
client.close()
