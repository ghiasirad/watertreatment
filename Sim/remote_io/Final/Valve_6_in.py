"""
MODIFIED Pymodbus Server With Updating Thread
--------------------------------------------------------------------------
This is an example of having a background thread updating the
context in an SQLite4 database while the server is operating.

This scrit generates a random address range (within 0 - 65000) and a random
value and stores it in a database. It then reads the same address to verify
that the process works as expected

This can also be done with a python thread::
    from threading import Thread
    thread = Thread(target=updating_writer, args=(context,))
    thread.start()
"""
import socket
import json
# --------------------------------------------------------------------------- #
# import the modbus libraries we need
# --------------------------------------------------------------------------- #
from pymodbus.server.async import StartTcpServer
from pymodbus.device import ModbusDeviceIdentification
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.datastore import ModbusServerContext, ModbusSlaveContext
from pymodbus.transaction import ModbusRtuFramer, ModbusAsciiFramer
import random

# --------------------------------------------------------------------------- #
# import the twisted libraries we need
# --------------------------------------------------------------------------- #
from twisted.internet.task import LoopingCall



# --------------------------------------------------------------------------- #
# define your callback process
# --------------------------------------------------------------------------- #

last_command = -1
def updating_writer(a):
    global last_command
    print 'updating'
    context  = a[0]
    
    slave_id = 0x01 # slave address
    count = 50
    s = a[1]
    
    current_command = context[slave_id].getValues(16, 1, 1)[0] / 65535.0

    s.send('{"request":"write","data":{"inputs":{"valve6_sp_in":'+repr(current_command)+'}}}\n')

    # import pdb; pdb.set_trace()
    #s.send('{"request":"read"}')
    data = json.loads(s.recv(1500))
    valve_6_in = int(data["state"]["valve_6_in"]*65535)
    level_6 = int(data["outputs"]["level_6"]/4*65535)
    print data

    # import pdb; pdb.set_trace()
    context[slave_id].setValues(4, 1, [valve_6_in,level_6])



def run_update_server():
    # ----------------------------------------------------------------------- #
    # initialize your data store
    # ----------------------------------------------------------------------- #


    store = ModbusSlaveContext(
        di=ModbusSequentialDataBlock(0,range(1,101)),
        co=ModbusSequentialDataBlock(0,range(101,201)),
        hr=ModbusSequentialDataBlock(0,range(201,301)),
        ir=ModbusSequentialDataBlock(0,range(301,401)))

    context = ModbusServerContext(slaves=store, single=True)

    # ----------------------------------------------------------------------- #
    # initialize the server information
    # ----------------------------------------------------------------------- #
    identity = ModbusDeviceIdentification()
    identity.VendorName = 'pymodbus'
    identity.ProductCode = 'PM'
    identity.VendorUrl = 'http://github.com/bashwork/pymodbus/'
    identity.ProductName = 'pymodbus Server'
    identity.ModelName = 'pymodbus Server'
    identity.MajorMinorRevision = '1.0'

    # connect to simulation
    HOST = '127.0.0.1'
    PORT = 55555
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))
    # ----------------------------------------------------------------------- #
    # run the server you want
    # ----------------------------------------------------------------------- #
    time = 1  # 5 seconds delay
    loop = LoopingCall(f=updating_writer, a=(context,sock))
    loop.start(time, now=False)  # initially delay by time
    StartTcpServer(context, identity=identity, address=("192.168.95.33", 502))


if __name__ == "__main__":
    run_update_server()
