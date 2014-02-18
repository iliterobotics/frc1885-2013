import usb.core

dev = usb.core.find(idVendor=0x045e, idProduct=0x02b0)

if dev is None:
  raise ValueError('Could not find Kinect Motor')

msg = ''
#ret = dev.ctrl_transfer(0x40, 6, 0x0001, 0, msg) # Don't need this???
ret = dev.ctrl_transfer(0xc0, 50, 0x0000, 0, 10)
print 'Accelerometer X   ', hex((ret[2]<<8)|ret[3])
print 'Accelerometer Y   ', hex((ret[4]<<8)|ret[5])
print 'Accelerometer Z   ', hex((ret[6]<<8)|ret[7])
print 'Tilt Angle  (Raw) ', hex(ret[8])
if ret[8] == 0x80:
  print 'Tilt Angle         MAX'
elif ret[8] & 0x80:
  print 'Tilt Angle        ', -1*(0xff-ret[8])/2.
else:
  print 'Tilt Angle        ', ret[8]/2.
print 'Tilt Status (Raw) ', hex(ret[9])
if ret[9] == 0x00:
  print 'Motor Stopped'
elif ret[9] == 0x01:
  print 'Motor Limit Reached'
elif ret[9] == 0x04:
  print 'Motor Moving'
