#!/usr/bin/python3

from cmd_helper import createCmdPacket, readResponse
import socket
import time

IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, 8766))
print("Connected!")
client.sendall(createCmdPacket("TouchSave", 0))
response = client.recv(0x4C)
readResponse(response)

client.close()
