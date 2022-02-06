import struct

def createCmdPacket(name, arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0):
    data = b""
    data += name.ljust(0x20, "\x00").encode('ascii')
    data += arg1.to_bytes(8, "little")
    data += arg2.to_bytes(8, "little")
    data += arg3.to_bytes(8, "little")
    data += arg4.to_bytes(8, "little")
    return data

def convertResponse(response_buffer):
    response_msg = response_buffer[0:0x40]
    response_msg_end = response_msg.find(b"\x00")
    return {
        "msg": response_msg[:response_msg_end].decode(),
        "type": struct.unpack_from("<l", response_buffer, 0x40)[0],
        "errorCode1": struct.unpack_from("<l", response_buffer, 0x44)[0],
        "errorCode2": struct.unpack_from("<l", response_buffer, 0x48)[0]
    }
def readResponse(response_buffer):
    response_msg = response_buffer[0:0x40]
    response_msg_end = response_msg.find(b"\x00")
    print("Msg:", response_msg[:response_msg_end].decode())
    print("Response Type:", struct.unpack_from("<l", response_buffer, 0x40)[0])
    print("Error Code 1:", hex(struct.unpack_from("<L", response_buffer, 0x44)[0]))
    print("Error Code 2:", hex(struct.unpack_from("<L", response_buffer, 0x48)[0]))

