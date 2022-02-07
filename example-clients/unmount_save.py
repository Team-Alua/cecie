#!/usr/bin/python3

from cmd_helper import createCmdPacket, readResponse
import socket
import time

IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, 8766))                                                           
print("Connected!")
# Replace with the sent mount path
mountPath = "/savedata0"
client.sendall(createCmdPacket("UnmountSave", len(mountPath)))
client.sendall(mountPath.encode('ascii').ljust(16, b"\x00"))

response = client.recv(0x4C)
readResponse(response)
client.close()
