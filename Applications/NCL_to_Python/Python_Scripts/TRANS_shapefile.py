#
#  File:
#    TRANS_shapefile.py
#
#  Synopsis:
#    Illustrates how to use shapefiles
#
#  Categories:
#    map plot
#    contour plot
#    shapefiles
#
#  Author:
#    Karin Meier-Fleischer, based on NCL example
#  
#  Date of initial publication:
#    September 2018
#
#  Description:
#    This example shows how to use shapefiles.
#    PyNIO is used to read the NetCDF and shapefile.
#
#  Effects illustrated:
#    o  Reading netCDF data
#    o  Drawing contours on a map
#    o  Using manual levels
#    o  Reading shapefile content
#    o  Drawing shapefile polylines
# 
#  Output:
#    A single visualization is produced.
#
#  Notes: The data for this example can be downloaded from 
#    http://www.ncl.ucar.edu/Document/Manuals/NCL_User_Guide/Data/
#   
'''
  Transition Guide Python Example: TRANS_shapefile.py

  -  Reading netCDF data
  -  Drawing contours on a map
  -  Using manual levels
  -  Reading shapefile content
  -  Drawing shapefile polylines
	
  18-09-11  kmf
'''
import numpy as np
import Ngl, Nio
import os

def BYR_03():
  return(np.array([\
         [  0,  0, 30],\
         [  4,  4, 37],\
         [  8,  8, 44],\
         [ 12, 12, 51],\
         [ 16, 16, 58],\
         [ 20, 20, 65],\
         [ 24, 24, 72],\
         [ 28, 28, 79],\
         [ 32, 32, 86],\
         [ 36, 36, 93],\
         [ 40, 40,100],\
         [ 44, 44,107],\
         [ 48, 48,114],\
         [ 52, 52,121],\
         [ 56, 56,128],\
         [ 60, 60,135],\
         [ 70, 70,142],\
         [ 80, 80,149],\
         [ 90, 90,156],\
         [100,100,163],\
         [110,110,170],\
         [120,120,177],\
         [130,130,184],\
         [140,140,191],\
         [150,150,198],\
         [160,160,205],\
         [170,170,212],\
         [180,180,219],\
         [190,190,226],\
         [200,200,233],\
         [210,210,240],\
         [220,220,247],\
         [255,250,205],\
         [255,247,185],\
         [255,244,165],\
         [255,241,145],\
         [255,238,125],\
         [255,226,113],\
         [255,214,101],\
         [255,202, 89],\
         [255,190, 77],\
         [255,178, 65],\
         [255,166, 53],\
         [255,154, 41],\
         [255,142, 29],\
         [255,130, 17],\
         [255,118,  5],\
         [255,106,  0],\
         [255, 94,  0],\
         [255, 82,  0],\
         [255, 70,  0],\
         [255, 58,  0],\
         [255, 46,  0],\
         [255, 34,  0],\
         [235, 24,  0],\
         [215, 14,  0],\
         [195,  4,  0],\
         [175,  0,  0],\
         [155,  0,  0],\
         [135,  0,  0],\
         [115,  0,  0],\
         [ 95,  0,  0],\
         [ 75,  0,  0],\
         [ 55,  0,  0],\
         [ 30,  0,  0],\
         [ 10,  0,  0]])/255.)

#--  open file and read variables
f    = Nio.open_file("tas_AFR-44_CNRM-CM5_rcp45_r1i1p1_CCLM_4-8-17_ym_20060101-20981231.nc", "r")
var  = f.variables["tas"][0,0,:,:]
lat  = f.variables["rlat"][:]
lon  = f.variables["rlon"][:]

#-- start the graphics
wks = Ngl.open_wks("png",os.path.basename(__file__).split('.')[0])

#-- resource settings
res                 =  Ngl.Resources()
res.nglFrame        =  False                #-- don't advance frame
res.nglDraw         =  False                #-- don't draw plot

res.cnFillOn        =  True
res.cnFillPalette   = BYR_03()              #-- choose color map
res.cnLinesOn       =  False
res.cnLineLabelsOn  =  False
res.cnLevelSelectionMode = "ManualLevels"   #-- set levels
res.cnMinLevelValF  =  240.0                #-- minimum contour level
res.cnMaxLevelValF  =  310.0                #-- maximum contour level
res.cnLevelSpacingF =  0.5                  #-- contour level spacing
res.cnFillMode      = "RasterFill"          #-- turn on contour fill

res.lbBoxLinesOn    =  False                #-- turn off labelbar box lines
res.lbLabelStride   =  10                   #-- skip every other label
res.lbBoxMinorExtentF =  0.24               #-- decrease height of labelbar box
res.pmLabelBarOrthogonalPosF = -0.05        #-- move labelbar upward

res.mpLimitMode     = "LatLon"
res.mpMinLatF       = -36.0
res.mpMaxLatF       =  42.6
res.mpMinLonF       = -23.0
res.mpMaxLonF       =  60.3
res.mpGridAndLimbOn =  False                #-- don't draw grid lines

res.sfXArray        =  lon
res.sfYArray        =  lat

#-- create the contour plot
plot = Ngl.contour_map(wks,var[:],res)

#-- read shapefile contents
shpf     = Nio.open_file("country.shp", "r") #-- open shapefile
lon      = np.ravel(shpf.variables["x"][:])
lat      = np.ravel(shpf.variables["y"][:])
segments = shpf.variables["segments"][:,0]

#-- polyline resource settings
plres             =  Ngl.Resources()        #-- resources for polylines
plres.gsLineColor = "black"
plres.gsSegments  =  segments

#-- add shapefile polylines to the plot
id = Ngl.add_polyline(wks, plot, lon, lat, plres)

#-- write variable long_name and units to the plot
txres               = Ngl.Resources()
txres.txFontHeightF = 0.022

Ngl.text_ndc(wks,f.variables["tas"].attributes['long_name'],0.30,0.88,txres)
Ngl.text_ndc(wks,'deg%s'%f.variables["tas"].attributes['units'],0.75,0.88,txres)

#-- advance the frame
Ngl.draw(plot)
Ngl.frame(wks)

Ngl.end()
