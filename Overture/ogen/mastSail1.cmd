**********************
* 2D sail and mast section grid
***********************
*
create mappings
SmoothedPolygon
*
      vertices
             13
            0.4   0.15       
            0.7   0.1      
            1.0   0.0 
	    1.01  0.015  
 	    1.0   0.03
	    0.7   0.11
	    0.4   0.16
	    0.2   0.11
	    0.0   0.03
 	   -0.01  0.015
	    0.0   0.0
            0.2   0.1       
            0.4   0.15       
*
       sharpness
  9.
  9.
  50.
  9.
  9.
  9.
  9.
  9.
  9.
  50.
  9.
  9.
  9.
*
    n-dist
       fixed normal distance
	 -.15
   t-stretch
 .15 20
 .15 20
 .15 50
 .15 40
 .15 50
 .15 20
 .15 20
 .15 20
 .15 50
 .15 50
 .15 50
 .15 20
 .15 20
   lines
    100 20
   boundary conditions
     -1 -1 5 0
   share
      -1 -1 5 0
  exit
    rotate/scale/shift
      rotate
	-15.
	0. 0.  
      scale 
	10. 10.
     mappingName
       mySail
   exit
*************************
* now design the mast with two ellipses
  Circle or ellipse
  specify axes of the ellipse
    0.07 0.05
  mappingName
    ellipse1
  exit
  Circle or ellipse
  specify axes of the ellipse
    .20 .20
  mappingName
    ellipse2
  exit
  tfi
  choose left curve
    ellipse1
  choose right curve
    ellipse2
  boundary conditions
    1 0 -1 -1
  share 
    5 0 -1 -1
  lines
    15 60
*  mappingName
*    mast1
  exit
  rotate/scale/shift  
  scale 
    10. 10.
  shift 
  .9 .6
  mappingName
    mast1
  exit
**********************
* now design the surrounding airbox   
  rectangle
    specify corners
  * xmin  ymin   xmax   ymax
    -10.    -10.   20.   10. 
    lines
    40 40 
  mappingName 
    airBox
    boundary conditions
    1 2 3 4
  exit
  view mappings
    mySail
    mast1
    airBox
exit
exit
*
  generate an overlapping grid
    airBox
    mySail
    mast1
    done
    pause
  change parameters
    prevent hole cutting 
      airBox
      * do not interpolate ghost
    * choose implicit or explicit interpolation
    interpolation type
    implicit for all grids
    explicit 
      airBox
      done
   * ghost points
   *   all
   *   2 2 2 2 2 2
  exit
compute overlap
pause
exit
save an overlapping grid
mySail1.hdf
mySail1
