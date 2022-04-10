#!/usr/bin/python3
import socket
import struct
import time
def SendCommand(name):
    cmdNameBytes = name.encode()
    client.send(cmdNameBytes.ljust(32, b"\x00"))

def GetResponse():
    responseLength = struct.unpack("<i",client.recv(4))[0]
    return client.recv(responseLength)
    
def SendPacket(titleId, dirName, saveBlocks):
    data = b""
    data += titleId.encode().ljust(16, b"\x00")
    data += dirName.encode().ljust(32, b"\x00")
    data += struct.pack("<Q", saveBlocks)
    client.send(data)


def GetFileNames():
    fileCount = struct.unpack("<i",client.recv(4))[0]
    for i in range(fileCount):
        fileNameLength = struct.unpack("<i", client.recv(4))[0]
        if fileNameLength == 0:
            break
        relPath = client.recv(fileNameLength, socket.MSG_WAITALL)
        if len(relPath) == 0:
            break
        print("name: {}".format(relPath))
        
    pass
IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    client.connect((IP, 8766))
except ConnectionRefusedError:
    exit(0)
print("Connected!")
SendCommand("ReserveSaveContainer")
print(GetResponse())
SendPacket("CUSA03694", "data0016", 114)
print(GetResponse())
SendCommand("MountSaveContainer")
print(GetResponse())
print(GetResponse())
SendCommand("ListFilesInSaveContainer")
print(GetResponse())
print(GetResponse())

GetFileNames()
client.close()
