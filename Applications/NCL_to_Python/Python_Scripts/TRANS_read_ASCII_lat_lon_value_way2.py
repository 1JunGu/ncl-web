#
#  File:
#    TRANS_read_ASCII_lat_lon_value_way2.py
#
#  Synopsis:
#    Illustrates how to read an ASCII file and create a 
#    contour fill plot on a map
#
#  Categories:
#    I/O
#    contour plot
#    map plot
#
#  Author:
#    Karin Meier-Fleischer, based on NCL example
#  
#  Date of initial publication:
#    September 2018
#
#  Description:
#    This example shows how to read an ASCII file and 
#    create a contour fill plot on a map.
#
#  Effects illustrated:
#    o  Read ASCII data
#    o  Drawing contours
#    o  Drawing a map
# 
#  Output:
#    -
#
#  Notes: The data for this example can be downloaded from 
#    http://www.ncl.ucar.edu/Document/Manuals/NCL_User_Guide/Data/
#   
"""
  Transition Guide Python Example:   TRANS_read_ASCII_lat_lon_value_way2.py

   based on read_asc6.ncl: http://ncl.ucar.edu/Applications/Scripts/read_asc6.ncl
   
   - read ASCII file asc6.txt
   - retrieve variable informations
   - draw contours on a map

	asc6.txt
	
    Lat     Lon     Temp (C)
    33.3    76.5    20.3
    33.3    76.6    20.3
    33.3    76.7    21.5
    33.3    76.8    20.0
	.....
	
  2018-08-27  kmf
"""
import Ngl
import os
import numpy as np

print("")

#-- read the data

f      = open("asc6.txt",'r')
data   = f.readlines()                          #-- data: type list

nrows  = len(data)

#-- assign lists to append elements

lat0 = []
lon0 = []
vals = []

for i in data[1::]:
	line = i.strip()
#	print(line)
	cols = line.split()
	lat0.append(cols[0])
	lon0.append(cols[1])
	vals.append(cols[2])

#-- convert string to float
lat0   = np.array(lat0).astype(float)
lon0   = np.array(lon0).astype(float)
temp1d = np.array(vals).astype(float)

indeqlat = np.array(np.where(lat0 == lat0[0]))

nlons    = indeqlat.shape                       #-- number of longitudes
nlons    = nlons[1]                             #-- number of longitudes
nlats    = int(nrows / nlons)                   #-- number of latitude

lat = lat0[::nlons]
lon = lon0[0:nlons]

#-- rows by column

print("--> nlats:            {}".format(len(lat)))
print("--> nlons:            {}".format(len(lon)))
print("--> rank of vals:     {}".format(len(temp1d.shape)))
print("--> shape temp1d:     {}".format(temp1d.shape))

temp2d = np.reshape(temp1d,[nlats,nlons])

print("--> min(temp2d)       {}".format(np.min(temp2d)))
print("--> max(temp2d)       {}".format(np.max(temp2d)))
print("--> min(lat)          {}".format(min(lat)))
print("--> max(lat)          {}".format(max(lat)))
print("--> min(lon)          {}".format(min(lon)))
print("--> max(lon)          {}".format(max(lon)))

print("--> shape temp2d:     {}".format(temp2d.shape))

#-- start the graphics
wks = Ngl.open_wks("png",os.path.basename(__file__).split('.')[0])

#-- set resources
res                      =  Ngl.Resources()

res.cnFillOn             =  True
res.cnLinesOn            =  False
res.cnLineLabelsOn       =  False
res.cnLevelSelectionMode = "ManualLevels"
res.cnMinLevelValF       =  np.min(temp1d)
res.cnMaxLevelValF       =  np.max(temp1d)
res.cnLevelSpacingF      =  2
res.cnFillPalette        = 'ncl_default'

res.tiMainString         = "temperature (degC)"

res.lbLabelFontHeightF   = 0.015
res.lbOrientation        = 'horizontal'
#res.pmLabelBarOrthogonalPosF = -0.1
res.pmLabelBarHeightF    = 0.07
res.pmLabelBarWidthF     = 0.6

res.sfXArray             = lon
res.sfYArray             = lat

res.mpLimitMode          = 'LatLon'
res.mpMinLonF            =  min(lon)
res.mpMaxLonF            =  max(lon)
res.mpMinLatF            =  min(lat)
res.mpMaxLatF            =  max(lat)
res.mpGridAndLimbOn      = False

#-- create the plot
plot = Ngl.contour_map(wks, temp2d, res)
Ngl.end()


