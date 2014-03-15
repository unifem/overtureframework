*
* Rap command file:
*   Remove the wheels from the asmo automobile
*
new/append model 
filename asmo.igs 
all mappings 
read file 
x-r:0 90 
heal model 
* Remove duplicate surface
delete surfaces 3 232 160 91 
pause
* 
* Remove a wheel
*
HEAL:selection function Hide Surface 
set view:0 0.468278 0.111783 0 4.84722 1 0 0 0 6.12323e-17 -1 0 1 6.12323e-17 
hide surfaces 8 90 12 54 88 53 55 89 11 
HEAL:delete hidden sub-surfaces 
x-r:0
x-r:0 
* front wheel removed
pause
Pick rotation point:0 
157.323 -134.696 7.14671 
x-r:0 
* 
* Remove the hole in the body by deleting trim curves
*
HEAL:selection function Edit Trimcurve 
HEAL:selection function Delete Trimcurve 
delete trim curve 0 1 
delete trim curve 0 0 
HEAL:selection function Delete Surface 
set view:0 0.464018 0.207328 0 16.7674 0.999732 -0.00138761 -0.0231087 -0.0195885 -0.582696 -0.812454 -0.0123379 0.812689 -0.582567 
delete surfaces 1 2 
HEAL:selection function Delete Trimcurve 
delete trim curve 0 2 
smaller:0 1.7
* hole for front wheel removed
pause
set view:0 -0.528024 0.0973451 0 6.27778 0.975702 0.0531785 0.212551 0.213213 -0.00703667 -0.97698 -0.0504587 0.99856 -0.018204 
*
* Remove the back wheel and  the hole in the body at the back wheel
*
HEAL:selection function Delete Surface 
delete surfaces 8 17 10 14 12 11 12 10 10 
x-r:0
* back wheel removed
pause
*
HEAL:selection function Delete Trimcurve 
Pick rotation point:0 
706.889 -126.986 9.97931 
mogl-select:0 1 
115 1067975872 1071743296 
mogl-coordinates 1.887906e-01 1.887906e-01 3.247863e-01 3.247863e-01 1.067976e+09 6.442863e+02 -1.125803e+02 -1.702143e+00 
set view:0 -0.530843 0.0510552 0 6.27778 0.978363 0.172206 0.114677 0.197884 -0.617055 -0.761633 -0.0603956 0.767846 -0.637781 
delete trim curve 0 4 
delete trim curve 0 3 
HEAL:selection function Delete Surface 
delete surfaces 1 6 
HEAL:selection function Delete Trimcurve 
delete trim curve 0 5 
* hole for back wheel removed
pause
set view:0 0 0 0 1 0.53603 0.280344 -0.796291 -0.843884 0.203706 -0.49635 0.0230603 0.938036 0.34577 
close 
*
* Build the connectivity and a global triangulation
topology 
merge tolerance 0.65 
split tolerance factor 3.23
deltaS 10 
build edge curves 
merge edge curves 
* the connectivity is determined
bigger:0 1.5
pause
*
* triangulate
*
triangulate
* global triangulation
pause
exit
*
* Save the asmo with no wheels
*
save model
asmoWithNoWheels.hdf
* model has been saved
pause
exit

