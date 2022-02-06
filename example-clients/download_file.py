#!/usr/bin/python3

from cmd_helper import createCmdPacket, readResponse, convertResponse
from file_helper import pipe_read_in
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
targetFile = "/data/IV0000-DEVP00001_00-CECIE00000000000.pkg"
client.sendall(createCmdPacket("DownloadFile", len(targetFile)))
client.sendall(targetFile.encode("ascii"))
response = client.recv(0x4C)
status = convertResponse(response)
if len(status["msg"]) > 0:
    print(status);
    client.close()
    sys.exit(-1)

fileSize = int.from_bytes(client.recv(8), "little")

pipe_read_in(client, "sample.pkg", fileSize)

response = client.recv(0x4C)
readResponse(response)
client.close()
