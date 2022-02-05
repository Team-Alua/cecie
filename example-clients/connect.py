#!/usr/bin/python3

import socket
import time

IP = "10.0.0.4"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, 8766))
print("Connected!")
client.close()
