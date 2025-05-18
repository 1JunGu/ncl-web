#
#  File:
#    TRANS_contour_fill_on_map.py
#
#  Synopsis:
#    Illustrates how to create a contour fill plot on a map
#
#  Categories:
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
#    This example shows how to create a contour fill plot on a map.
#    xarray is used to read the NetCDF file, but PyNIO can also
#    be used. See the lines commented with ###.
#
#  Effects illustrated:
#    o  Drawing a contour fill plot
#    o  Drawing a map
# 
#  Output:
#    A single visualization is produced.
#
#  Notes: The data for this example can be downloaded from 
#    http://www.ncl.ucar.edu/Document/Manuals/NCL_User_Guide/Data/
#   
'''
  Transition Guide Python Example: 	TRANS_contour_fill_on_map.py

  - drawing contour fill plot
  - drawing a map
  
  18-09-04  kmf
'''
import numpy as np
import Ngl
import xarray as xr
###import Nio
import os

#--  open file and read variables
f    = xr.open_dataset("rectilinear_grid_3D.nc")
var  = f.t[0,0,:,:].values
lat  = f.lat.values
lon  = f.lon.values

### PyNIO
###f    = Nio.open_file("rectilinear_grid_3D.nc", "r")
###var  = f.variables["t"][0,0,:,:]
###lat  = f.variables["lat"][:]
###lon  = f.variables["lon"][:]

#-- start the graphics
wks = Ngl.open_wks("png",os.path.basename(__file__).split('.')[0])

#-- resource settings
res                 =  Ngl.Resources()
res.nglFrame        =  False

res.cnFillOn        =  True
res.cnFillPalette   = "NCL_default"
res.cnLineLabelsOn  =  False

res.lbOrientation   = "horizontal"          #-- horizontal labelbar

res.sfXArray        =  lon
res.sfYArray        =  lat

#-- create the contour plot
plot = Ngl.contour_map(wks,var,res)

#-- Retrieve some resources from map for adding labels
vpx  = Ngl.get_float(plot.map,'vpXF')
vpy  = Ngl.get_float(plot.map,'vpYF')
vpw  = Ngl.get_float(plot.map,'vpWidthF')
fnth = Ngl.get_float(plot.map,'tmXBLabelFontHeightF')

#-- write variable long_name and units to the plot
txres               = Ngl.Resources()
txres.txFontHeightF = fnth

txres.txJust  = "CenterLeft"
Ngl.text_ndc(wks,f.t.long_name,vpx,vpy+0.02,txres)
###Ngl.text_ndc(wks,f.variables["t"].attributes['long_name'],vpx,vpy+0.02,txres)

txres.txJust  = "CenterRight"
Ngl.text_ndc(wks,f.t.units,vpx+vpw,vpy+0.02,txres)
###Ngl.text_ndc(wks,f.variables["t"].attributes['units'],vpx,vpy+0.02,txres)

#-- advance the frame
Ngl.frame(wks)

Ngl.end()
