#!/usr/bin/python

# MapObject categories:
# 1: Party
# 2: Village
# 3: Dungeon

import os
import sys
from PIL import Image

OCEAN = (0, 174, 240)
LAND = (57, 181, 74)
FORREST = (0, 114, 54)
MOUNTAIN = (149, 149, 149)
VILLAGE = (198, 156, 109)
FORTRESS = (255, 0, 0)
CAVEMOUTH = (96, 57, 19)

value = {}
value[LAND] = 0
value[FORREST] = 1
value[MOUNTAIN] = 2
value[OCEAN] = 3
value[VILLAGE] = 0
value[FORTRESS] = 0
value[CAVEMOUTH] = 2

image = Image.open(sys.argv[1])
width,height = image.size
print '{"context":{"partyId1":0,"partyId2":0},'
print '"map":{"height":%d,"width":%d,"scale":1.50,"hexes":[' % (height,width)

id = 0
mapData = []
mapObjects = []
for y in range(height):
    row = []
    for x in range(width):
        pixel = image.getpixel((x,y))
        if pixel == VILLAGE:
            mapObjects.append('{"id":%d,"x":%d,"y":%d,"visualType":%d,"category":2,"layer":1,"actions":null,"members":null,"items":null,"units":null,"texts":null}' % (id, x, y, 4))
            id += 1
        elif pixel == CAVEMOUTH:
            mapObjects.append('{"id":%d,"x":%d,"y":%d,"visualType":%d,"category":3,"layer":1,"actions":null,"members":null,"items":null,"units":null,"texts":null}' % (id, x, y, 5))
            id += 1
        elif pixel == FORTRESS:
            mapObjects.append('{"id":%d,"x":%d,"y":%d,"visualType":%d,"category":3,"layer":1,"actions":null,"members":null,"items":null,"units":null,"texts":null}' % (id, x, y, 4))
            id += 1
        tileData = '{"v":%d,"x":%d,"y":%d}' % (value[pixel],x,y)
        row.append(tileData)
    mapData.append(','.join(row))

print ',\n'.join(mapData)
print ']},'
    
print '"mapObjects":['
try:
    f = open(os.path.splitext(sys.argv[1])[0] + ".txt", 'r')
except:
    f = 0

if (f):
    print f.read()
    f.close()

print ',\n'.join(mapObjects)
print '],'
print '"units":null}'