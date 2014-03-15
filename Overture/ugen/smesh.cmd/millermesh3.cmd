* 
* Kull test problem mesh 
* Geometry as specified by Doug Miller 
*   EXCEPTION: angles not quite 60 degrees, distance 141.24 was rounded to 141 
* 
* Initial Version: 
*    4/25/02 Kyle Chand 
* 
tb Plot Reference Grids 0
Create Curve 
Mouse Mode Build Point 
new point 0.   0. 
new point 602. 0. 
new point 0.   20. 
new point 141. 20. 
new point 261. 20. 
new point 461. 20. 
new point 602. 20. 
new point 0.   230. 
new point 20.  230. 
new point 382. 230. 
new point 582. 230. 
new point 602. 230. 
new point 0.   250. 
new point 602. 250. 
Mouse Mode Interpolate Curve 
point for interpolation 0 
point for interpolation 1 
stop picking 
point for interpolation 0 
point for interpolation 2 
stop picking 
point for interpolation 2 
point for interpolation 3 
stop picking 
point for interpolation 3 
point for interpolation 4 
stop picking 
point for interpolation 4 
point for interpolation 5 
stop picking 
point for interpolation 5 
point for interpolation 6 
stop picking 
point for interpolation 1 
point for interpolation 6 
stop picking 
point for interpolation 2 
point for interpolation 7 
stop picking 
point for interpolation 7 
point for interpolation 8 
stop picking 
point for interpolation 8 
point for interpolation 3 
stop picking 
point for interpolation 4 
point for interpolation 9 
stop picking 
point for interpolation 8 
point for interpolation 9 
stop picking 
point for interpolation 9 
point for interpolation 10 
stop picking 
point for interpolation 5 
point for interpolation 10 
stop picking 
point for interpolation 6 
point for interpolation 11 
stop picking 
point for interpolation 10 
point for interpolation 11 
stop picking 
point for interpolation 7 
point for interpolation 12 
stop picking 
point for interpolation 12 
point for interpolation 13 
stop picking 
point for interpolation 13 
point for interpolation 11 
stop picking 
exit 
*dxdy  12, 12
* serendipidous spacing : 1,1 
dxdy 1.,1.
Create TFI Region
select left 16
Done
select right 18
Done
select bottom 8
select bottom 11
select bottom 12
select bottom 15
Done
select top 17
Done
generate
Create Unstructured Region
select outer 7
select outer 3
select outer 9
select outer 8
Done
Done
generate
Create TFI Region
select left 4
Done
select right 6
Done
select bottom 5
Done
select top 3
select top 2
select top 1
select top 0
Done
generate
Create Unstructured Region
select outer 11
select outer 9
select outer 2
select outer 10
Done
Done
generate
Create Unstructured Region
select outer 10
select outer 1
select outer 13
select outer 12
Done
Done
generate
Create Unstructured Region
select outer 15
select outer 13
select outer 0
select outer 14
Done
Done
generate
Save Mesh
/home/chand/overture/2dmesh/test.msh
