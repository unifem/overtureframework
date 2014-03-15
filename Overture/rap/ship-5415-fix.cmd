new/append model 
browse file 
ship_5415.igs 
read file 
set view:0 0.352018 0.179372 0 3.71667 0.930839 0.35347 0.0927158 -0.0409101 0.352922 -0.934757 -0.363131 0.866316 0.342975 
heal model 
HEAL:selection function Hide Surface 
hide surfaces 1 0 
HEAL:selection function Surface Edge 
surface edge 1 6.611932e-01 1.743872e-01 3.016495e-01 
HEAL:selection function Select Curve 
select curve 0 
HEAL:unhide all sub-surfaces 
HEAL:selection function Project Curve 
project edge 0 0 
HEAL:hide valid sub-surfaces 
HEAL:selection function Edit Trimcurve 
edit trim curve 0 2 
Mouse Mode Snap To Intersection
snap to intersection 4 2 1 1 2.331050e-01 9.971033e-01 1.939001e-01 9.668682e-01 
snap to intersection 3 0 0 0 3.084990e-01 -3.882191e-03 2.511995e-01 3.150657e-02 
Mouse Mode Hide SubCurve
hide curve 1
auto assemble
HEAL:show valid sub-surfaces
set view:0 0.352018 0.179372 0 3.25012 0.932743 0.348414 0.0927158 -0.0389946 0.353139 -0.934757 -0.358425 0.868273 0.342975
HEAL:plot curves (toggle) 0
count broken
close
force redraw, wait 1
save model
ship-5415-fix.hdf
exit
