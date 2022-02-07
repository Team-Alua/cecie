#!/usr/bin/python3

from cmd_helper import createCmdPacket, readResponse
import socket
import time

def createDeleteSavePacket(options):
    packet = b""
    packet += options.get("dirName", "").encode("ascii").ljust(0x20, b'\x00')
    packet += options.get("titleId", "").encode("ascii").ljust(0x10, b"\x00")
    return packet

IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, 8766))                                                           
print("Connected!")
client.sendall(createCmdPacket("DeleteSave"))

client.sendall(createDeleteSavePacket({
    "dirName": "data0000\x00",
    "titleId": "CUSA03694\x00"
}))
response = client.recv(0x4C)
readResponse(response)
client.close()
