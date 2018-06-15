#!/usr/bin/python
# generation of PowerAPI XML config files for taurus
# Bernhard Hering 2015
# last change: 2015-10-07


def print_devices(nodename,openString):
  print('\t<devices>\n\t\t<dev name="hdeemdev" device="HDEEM-' + nodename + '" openString="'+openString+'" />\n\t</devices>\n')
  
def print_attributes():
  print('\t<attributes>')
  print('\t\t<attr name="ENERGY" op="SUM">\n\t\t\t<src type="device" name="hdeemdev" />\n\t\t</attr>')
  print('\t\t<attr name="POWER" op="SUM">\n\t\t\t<src type="device" name="hdeemdev" />\n\t\t</attr>')
  print('\t\t<attr name="AVG_POWER" op="SUM">\n\t\t\t<src type="device" name="hdeemdev" />\n\t\t</attr>')
  print('\t\t<attr name="MIN_POWER" op="SUM">\n\t\t\t<src type="device" name="hdeemdev" />\n\t\t</attr>')
  print('\t\t<attr name="MAX_POWER" op="SUM">\n\t\t\t<src type="device" name="hdeemdev" />\n\t\t</attr>')
  print('\t</attributes>')
  
def print_node(nodename):
  print('<obj name="plat.' + nodename + '" type="Node">')
  print_devices(nodename,'Blade')
  print_attributes()
  print('\t<children>\n\t\t<child name="board" />\n\t</children>\n</obj>')
  
def print_board(nodename):
  print('<obj name="plat.' + nodename + '.board" type="Board">')
  print('\t<attributes>')
  print('\t\t<attr name="ENERGY" op="SUM">\n\t\t\t<src type="child" name="socket0" />\n\t\t\t<src type="child" name="socket1" />\n\t\t\t<src type="child" name="mem0" />\n\t\t\t<src type="child" name="mem1" />\n\t\t\t<src type="child" name="mem2" />\n\t\t\t<src type="child" name="mem3" />\n\t\t</attr>')
  print('\t\t<attr name="POWER" op="SUM">\n\t\t\t<src type="child" name="socket0" />\n\t\t\t<src type="child" name="socket1" />\n\t\t\t<src type="child" name="mem0" />\n\t\t\t<src type="child" name="mem1" />\n\t\t\t<src type="child" name="mem2" />\n\t\t\t<src type="child" name="mem3" />\n\t\t</attr>')
  print('\t\t<attr name="AVG_POWER" op="SUM">\n\t\t\t<src type="child" name="socket0" />\n\t\t\t<src type="child" name="socket1" />\n\t\t\t<src type="child" name="mem0" />\n\t\t\t<src type="child" name="mem1" />\n\t\t\t<src type="child" name="mem2" />\n\t\t\t<src type="child" name="mem3" />\n\t\t</attr>')
  print('\t\t<attr name="MIN_POWER" op="SUM">\n\t\t\t<src type="child" name="socket0" />\n\t\t\t<src type="child" name="socket1" />\n\t\t\t<src type="child" name="mem0" />\n\t\t\t<src type="child" name="mem1" />\n\t\t\t<src type="child" name="mem2" />\n\t\t\t<src type="child" name="mem3" />\n\t\t</attr>')
  print('\t\t<attr name="MAX_POWER" op="SUM">\n\t\t\t<src type="child" name="socket0" />\n\t\t\t<src type="child" name="socket1" />\n\t\t\t<src type="child" name="mem0" />\n\t\t\t<src type="child" name="mem1" />\n\t\t\t<src type="child" name="mem2" />\n\t\t\t<src type="child" name="mem3" />\n\t\t</attr>')
  print('\t</attributes>\n\t<children>\n\t\t<child name="socket0" />\n\t\t<child name="socket1" />\n\t\t<child name="mem0" />\n\t\t<child name="mem1" />\n\t\t<child name="mem2" />\n\t\t<child name="mem3" />\n\t</children>\n</obj>')
  
def print_cpumem(nodename):
  print('<obj name="plat.'+nodename+'.board.socket0" type="Socket">')
  print_devices(nodename,'CPU0')
  print_attributes()
  print('</obj>\n<obj name="plat.'+nodename+'.board.socket1" type="Socket">')
  print_devices(nodename,'CPU1')
  print_attributes()
  print('</obj>\n<obj name="plat.'+nodename+'.board.mem0" type="Memory">')
  print_devices(nodename,'DDR AB')
  print_attributes()
  print('</obj>\n<obj name="plat.'+nodename+'.board.mem1" type="Memory">')
  print_devices(nodename,'DDR CD')
  print_attributes()
  print('</obj>\n<obj name="plat.'+nodename+'.board.mem2" type="Memory">')
  print_devices(nodename,'DDR EF')
  print_attributes()
  print('</obj>\n<obj name="plat.'+nodename+'.board.mem3" type="Memory">')
  print_devices(nodename,'DDR GH')
  print_attributes()
  print('</obj>\n\n')

taurus = [];

#for i in range(6339,6350):
#  node = 'taurusi' + str(i)
#  taurus.append(node)

for i in range(4001,4200):
  node = 'taurusi' + str(i)
  taurus.append(node)
#for i in range(5001,5613):
#  node = 'taurusi' + str(i)
#  taurus.append(node)
#for i in range(6001,6613):
#  node = 'taurusi' + str(i)
#  taurus.append(node)
  
print('<?xml version="1.0"?>\n')
print('<System>\n')
print('<Plugins>')
print('\t<plugin name="HDEEM" lib="libpwr_hdeemdev.so" />')
print('</Plugins>\n')


print('<Devices>')

for node in taurus:
  print('\t<device name="HDEEM-' + node +'" plugin="HDEEM" initString="' + node + '-bmc;x;x" />')
  
print('</Devices>\n\n<Objects>\n<obj name="plat" type="Platform">')
print('\t<attributes>')
print('\t\t<attr name="ENERGY" op="SUM">')

for node in taurus:
  print('\t\t\t<src type="child" name="' + node +'" />')
print('\t\t</attr>')

print('\t\t<attr name="POWER" op="SUM">')

for node in taurus:
  print('\t\t\t<src type="child" name="' + node +'" />')
print('\t\t</attr>')

print('\t\t<attr name="AVG_POWER" op="SUM">')

for node in taurus:
  print('\t\t\t<src type="child" name="' + node +'" />')
print('\t\t</attr>')

print('\t\t<attr name="MIN_POWER" op="SUM">')

for node in taurus:
  print('\t\t\t<src type="child" name="' + node +'" />')
print('\t\t</attr>')

print('\t\t<attr name="MAX_POWER" op="SUM">')

for node in taurus:
  print('\t\t\t<src type="child" name="' + node +'" />')
print('\t\t</attr>\n')

print('\t</attributes>\n\t<children>')

for node in taurus:
  print('\t\t<child name="' + node +'" />')
print('\t</children>\n</obj>\n')


for node in taurus:
  print_node(node)
 # print_board(node)
 # print_cpumem(node)

print('</Objects>')
print('</System>')
