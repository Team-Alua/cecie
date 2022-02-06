#!/usr/bin/python3

from cmd_helper import createCmdPacket, readResponse
import socket
import time

def createTouchSavePacket(options):
    packet = b""
    packet += options.get("dirName", "").encode("ascii").ljust(0x20, b'\x00')
    packet += options.get("titleId", "").encode("ascii").ljust(0x10, b"\x00")
    packet += options.get("fingerprint", "").encode("ascii").ljust(0x50, b"\x00")
    packet += options.get("saveBlocks", 0).to_bytes(8, 'little')
    return packet

IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, 8766))
print("Connected!")
client.sendall(createCmdPacket("DownloadFile", 0))

response = client.recv(0x4C)
readResponse(response)
client.close()
