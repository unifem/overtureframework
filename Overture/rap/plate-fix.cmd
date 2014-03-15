new/append model 
browse file 
/usr/casc/overture/Overture/sampleMappings/plate.igs 
read file 
set view:0 0 0 0 1 0.910655 -0.00397987 -0.413147 -0.00335015 0.99985 -0.017016 0.413153 0.0168798 0.910505 
heal model 
HEAL:hide valid sub-surfaces 
set view:0 0 0 0 1 0.829424 0.131716 -0.542869 0.0153686 0.966056 0.257874 0.558408 -0.22223 0.799246 
HEAL:selection function Edit Trimcurve 
edit trim curve 0 27 
set view:0 -0.157373 -0.74546 0 59.6987 1 0 0 0 1 0 0 0 1 
Mouse Mode Split 
split curve 1 9.780769e-01 
split curve 0 1.516608e-02 
Mouse Mode Hide SubCurve
hide curve 6
hide curve 6
Mouse Mode Snap To Intersection
snap to intersection 0 5 0 1 8.662065e-01 9.795733e-01 8.658066e-01 9.806601e-01 
snap to intersection 3 4 1 0 8.769049e-01 9.791706e-01 8.770549e-01 9.807578e-01 
auto assemble
HEAL:hide valid sub-surfaces
edit trim curve 0 31
set view:0 0.694134 -0.0377166 0 24.518 1 0 0 0 1 0 0 0 1
Mouse Mode Split
split curve 1 9.252456e-01 
set view:0 0.710283 -0.0356865 0 134.614 1 0 0 0 1 0 0 0 1
Mouse Mode Hide SubCurve
hide curve 7
Mouse Mode Snap To Intersection
snap to intersection 6 1 1 0 3.971926e-02 5.125892e-01 3.945322e-02 5.131803e-01 
auto assemble
HEAL:hide valid sub-surfaces
edit trim curve 0 29
set view:0 -0.723312 -0.029598 0 32.6367 1 0 0 0 1 0 0 0 1
Mouse Mode Split
split curve 2 7.268098e-02 
Mouse Mode Hide SubCurve
hide curve 6
Mouse Mode Snap To Intersection
snap to intersection 6 1 0 1 9.567948e-01 5.124980e-01 9.566119e-01 5.134029e-01 
auto assemble
HEAL:hide valid sub-surfaces
HEAL:unhide all sub-surfaces 
count broken
close
force redraw, wait 1
save model
plate.hdf
exit
