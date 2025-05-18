;----------------------------------------------------------------------
; panel_5x2_33.ncl
;
; Concepts illustrated:
;   - Combining two sets of paneled plots on one page
;   - Maximizing paneled plots after they've been created
;   - Drawing two labelbars in a combined panel plot
;   - Using lbBoxEndCapStyle to draw triangles at the end of a labelbar
;----------------------------------------------------------------------
; This script uses the same data and is similar to panel_33.ncl,
; except the logic for positioning the plots is a little cleaner,
; and 5 x 3 plots are drawn instead of 3 x 2.
;----------------------------------------------------------------------
; These files are loaded by default in NCL V6.2.0 and newer
; load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/gsn_code.ncl"
; load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/gsn_csm.ncl"
; load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/contributed.ncl"
load "./panel_two_sets.ncl"

begin
;---Open file
    fname = "TS.cam3.toga_ENS.1950-2000.nc"
    f     = addfile(fname,"r")

;---Convert "time" to an ntim x 6 array of year,mon,day,hour,min,sec
    newtime = cd_calendar(f->time,0)
    years   = newtime(:,0)
    months  = newtime(:,1)
    
;---Indicate start and end years of interest.
    month      = 1                  ; January
    smonth     = "Jan"
    start_years = (/1951,1961/)
    mid_years   = (/1971,1981/)
    end_years   = (/1991,2001/)
    nyears_cols = dimsizes(start_years)
    nyears_rows = 3

;---Read in temperature and convert to degrees C.
    t       = f->TS                 
    t       = t - 273.15
    t@units = "degC"

;---Start the graphics
    wks  = gsn_open_wks("png","panel_5x2")         ; send graphics to PNG file
    
    res                     = True
    res@gsnDraw             = False
    res@gsnFrame            = False
    res@mpFillOn            = False            ; no need   

    levels1 = ispan(-30,270,15) * 0.1
    labels1 = sprintf("%4.1f",levels1)   ; They both have length 4
    levels2 = ispan(-4,4,1)
    labels2 = sprinti("%4i",levels2)   ; They both have length 4

    res@cnLevelSelectionMode= "ExplicitLevels"   ; manual set levels
    res@cnLevels            = levels1
    res@lbLabelStrings      = labels1
    res@cnFillOn            = True             ; color fill plot
    res@cnLinesOn           = False
    res@cnLineLabelsOn      = False
    res@cnInfoLabelOn       = False
    res@cnFillPalette       = "BlAqGrYeOrRe"

    res@lbLabelBarOn        = False            ; turn off individual label bars

    res@gsnStringFontHeightF= 0.02
    res@gsnLeftString       = "TS"
    res@gsnRightString      = t@units

;---Resources for diff plots
    dres                    = res    
    dres@cnLevelSelectionMode= "ExplicitLevels"   ; manual set levels
    dres@cnLevels            := levels2
    dres@cnFillPalette       = "temp_diff_18lev"
    dres@lbLabelStrings      := labels2
    dres@lbBoxEndCapStyle    = "TriangleBothEnds"    ; Added in NCL V6.4.0

;---Create arrays to hold series of plots
    plots      = new(nyears_rows*nyears_cols,graphic)    
    diff_plots = new((nyears_rows-1)*nyears_cols,graphic)    

    do i=0,nyears_cols-1
;---Get data for start and end year of interest
      sy = ind(years.eq.start_years(i).and.months.eq.month)
      my = ind(years.eq.mid_years(i)  .and.months.eq.month)
      ey = ind(years.eq.end_years(i)  .and.months.eq.month)

      diff_mid = t(sy,:,:)                ; trick to copy  metadata
      diff_mid = t(my,:,:) - t(sy,:,:)    ; overwrite with diff values
      diff_end = t(my,:,:)                ; trick to copy  metadata
      diff_end = t(ey,:,:) - t(my,:,:)    ; overwrite with diff values

      diff_mid@long_name = mid_years(i) + "-" + start_years(i) + \
                       " TS field differences"
      diff_end@long_name = end_years(i) + "-" + mid_years(i) + \
                       " TS field differences"
;---Debug prints
      print("========================================")
      printMinMax(t(sy,:,:),0)
      printMinMax(t(my,:,:),0)
      printMinMax(t(ey,:,:),0)
      printMinMax(diff_mid,0)
      printMinMax(diff_end,0)
    
;---Create the start/mid/end year plots and the difference plots
      res@gsnCenterString = smonth + " " + start_years(i)
      plots(i) = gsn_csm_contour_map(wks,t(sy,:,:),res)

      res@gsnCenterString = smonth + " " + mid_years(i)
      plots(i+nyears_cols) = gsn_csm_contour_map(wks,t(my,:,:),res)

      res@gsnCenterString = smonth + " " + end_years(i)
      plots(i+2*nyears_cols) = gsn_csm_contour_map(wks,t(ey,:,:),res)

      dres@gsnCenterString = "Difference: " + smonth + " " + \
                             mid_years(i) + "-" + start_years(i)
      diff_plots(i) = gsn_csm_contour_map(wks, diff_mid ,dres)
      dres@gsnCenterString = "Difference: " + smonth + " " + \
                             end_years(i) + "-" + mid_years(i)
      diff_plots(i+nyears_cols) = gsn_csm_contour_map(wks, diff_end ,dres)
    end do

    dims1 = (/nyears_rows,nyears_cols/)
    dims2 = (/nyears_rows-1,nyears_cols/)

;---Resources for paneling
    pres  = True
    pres1 = True
    pres2 = True
    pres@gsnPanelTop         = 0.95    ; add room for title
    pres1@gsnPanelMainString = fname
    pres1@lbLabelFontHeightF = 0.01
    pres2@lbLabelFontHeightF = 0.01
;    drawNDCGrid(wks)
    panel_two_sets(wks,plots,diff_plots,dims1,dims2,pres1,pres2,pres)
end
    
