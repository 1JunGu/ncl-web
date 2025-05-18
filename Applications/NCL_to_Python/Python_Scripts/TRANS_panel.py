#
#  File:
#    TRANS_panel.py
#
#  Synopsis:
#    Illustrates how to create a panel plot
#
#  Categories:
#    contour plot
#    panel plot
#
#  Author:
#    Karin Meier-Fleischer, based on NCL example
#  
#  Date of initial publication:
#    September 2018
#
#  Description:
#    This example shows how to create a panel plot.
#    xarray is used to read the NetCDF file, but PyNIO can also
#    be used. See the lines commented with ###.
#
#  Effects illustrated:
#    o  Read netCDF data
#    o  Drawing a contour fill plot
#    o  Creating a panel plot
# 
#  Output:
#    A single visualization is produced.
#
#  Notes: The data for this example can be downloaded from 
#    http://www.ncl.ucar.edu/Document/Manuals/NCL_User_Guide/Data/
#   
'''
  Transition Guide Python Example: TRANS_panel.py
  
  -  Drawing a contour fill plot
  -  Creating a panel plot

  18-09-10  kmf
'''
import Ngl
import xarray as xr
###import Nio
import os

#--  open file and read variables
f    = xr.open_dataset("rectilinear_grid_2D.nc")
var  = f.tsurf
lat  = f.lat.values
lon  = f.lon.values

###f   = Nio.open_file("rectilinear_grid_2D.nc", "r")
###var = f.variables["tsurf"]
###lat = f.variables["lat"][:]
###lon = f.variables["lon"][:]

#-- start the graphics
wks = Ngl.open_wks("png",os.path.basename(__file__).split('.')[0])

#-- resource settings
res                 =  Ngl.Resources()
res.nglDraw         =  False            #-- don't draw plots
res.nglFrame        =  False            #-- don't advance the frame

res.cnFillOn        =  True             #-- contour fill
res.cnFillPalette   = "cmp_b2r"         #-- choose color map
res.cnLineLabelsOn  =  False            #-- no line labels

res.lbLabelBarOn    =  False            #-- don't draw a labelbar

res.sfXArray        =  lon              #-- coordinates for the x-axis
res.sfYArray        =  lat              #-- coordinates for the y-axis
	
#-- create the contour plots
plot = []
for i in range(0,4):
	p = Ngl.contour_map(wks,var[i,:,:],res)
	plot.append(p)

#-- panel resources
pnlres = Ngl.Resources()
#pnlres.nglDraw          = False
pnlres.nglFrame         = False
pnlres.nglPanelLabelBar = True          #-- common labelbar
pnlres.txString    = "TRANS: panel example" #-- panel title
pnlres.txFontHeightF    =  0.02 #-- text font size

Ngl.panel(wks,plot[0:4],[2,2],pnlres)  

#-- add title string,long_name and units string to panel
txres = Ngl.Resources()
txres.txFontHeightF = 0.020
Ngl.text_ndc(wks,"TRANS: panel example",0.5,0.825,txres)

txres.txFontHeightF = 0.012
Ngl.text_ndc(wks,var.long_name,0.12,0.79,txres)
Ngl.text_ndc(wks,var.units,    0.975,0.79,txres)

###Ngl.text_ndc(wks,f.variables["tsurf"].attributes['long_name'],0.12,0.79,txres)
###Ngl.text_ndc(wks,f.variables["tsurf"].attributes['units'],    0.975,0.79,txres)

#-- advance the frame
Ngl.frame(wks)

Ngl.end()
