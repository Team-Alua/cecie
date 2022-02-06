import struct

def createCmdPacket(name, size):
    data = b""
    data += name.ljust(0x20, "\x00").encode('ascii')
    data += size.to_bytes(8, "little")
    return data
def readResponse(response_buffer):
    response_msg = response_buffer[0:0x40]
    response_msg_end = response_msg.find(b"\x00")
    print("Msg:", response_msg[:response_msg_end].decode())
    print("Response Type:", struct.unpack_from("<l", response_buffer, 0x40)[0])
    print("Error Code 1:", struct.unpack_from("<l", response_buffer, 0x44)[0])
    print("Error Code 2:", struct.unpack_from("<l", response_buffer, 0x48)[0])

