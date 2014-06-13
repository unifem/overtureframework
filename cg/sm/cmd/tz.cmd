#
# cgsm -- test the twilightzone
#
# Usage:
#   
#  cgsm [-noplot] tz -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> -ts=[me|fe|ie]...
#                    -bcn=[dirichlet|d|sf|slip|mixed] -diss=<> -order=<2/4> -debug=<num> -bg=<backGround> -cons=[0/1] ...
#                    -pv=[nc|c|g|h] -godunovOrder=[1|2] -mu=<> -lambda=<> -rho=<> -filter=[0|1] ...
#                    -bc1=[dirichlet|d|t|s|sym] -bc2=[..] -bc3=[...] ... -bc6=[...] 
#                    -checkGhostErr=[0|1|2] -stressRelaxation=[0|2|4] -relaxAlpha=<> -relaxDelta=<> ...
#                    -godunovType=[0|2] -varMat=[0|1]  -go=[run/halt/og]
# 
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -cons : 1= conservative difference 
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -godunovType : 0=linear, 1=LE with HMS code, 2=SVK 3=Rotated-linear, 4=MHS+NeoHookean
#  -ts : time-stepping method, me=modified-equation, fe=forward-Euler, ie=improved-Euler, ab=adams-bashforth
#  -checkGhostErr : optionally check the error on this many ghost points
#  -stressRelaxation : turn on stress-strain relaxation, 2=2nd-order approx, 4=4th order
#  -varMat : 1=variable material properties, 0=constant
#  -bc1 = [dirichlet|d|t|s|sym] : set boundaries with bc=1 to be dirichlet, displacement, traction, slipwWall or symmetry
#  -bcn : OLD dirichlet=dirichlet, d=displacement, sf=stress-free, slip=slipWall
# 
# Examples:
# 
# -- dirichlet BC: 
#  cgsm tz -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=2. -go=halt              [exact]
#  cgsm tz -g=nonSquare10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt           [exact]
#  cgsm tz -g=rotatedSquare10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt       [exact]
#  cgsm noplot tz -g=nonSquare10.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go      [exact]
#  cgsm noplot tz -g=sise2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go     [exact]
#  cgsm noplot tz -g=rsis2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go            [exact]
#  cgsm tz -g=cice1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt
#  cgsm tz noplot -g=cice1.order2.hdf -diss=0.1 -tf=.5 -tz=trig -go=go
#  cgsm tz noplot -g=cice2.order2.hdf -diss=0.1 -tf=.5 -tz=trig -go=go
#  -- cons:
#    cgsm tz -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=2. -pv=c -go=halt  [exact]
#    cgsm tz -g=square10.hdf -degreex=1 -degreet=1 -diss=0. -tf=2. -pv=c -bc=sf -go=halt   [exact]
#    cgsm tz -g=box10.hdf -degreex=1 -degreet=1 -diss=0. -tf=2. -pv=c -bc=sf -go=halt   [exact]
#    cgsm tz -g=rbibe1.order2 -degreex=1 -degreet=1 -diss=0. -tf=2. -pv=c -bc=sf -go=halt   [exact]
#    cgsm tz -g=wiggley4.order2 -pv=c -tf=1. -tp=0.1 -tz=trig -bcn=sf -diss=0 -filter=1 -filterOrder=6 -filterStages=2 -go=halt
#  -- pulse: 
#  cgsm tz -g=square20.hdf -degreex=2 -degreet=2 -diss=0. -tf=2. -tz=pulse -go=halt 
#   -- fourth-order --
#    cgsm tz -g=square16.order4 -degreex=4 -degreet=3 -diss=0. -tf=2. -go=halt      [exact]
#    cgsm tz -g=square16.order4 -degreex=4 -degreet=4 -diss=0. -tf=2. -go=halt      [not exact: why?]
#   -- 3d --
#  cgsm tz -g=box10.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=halt                 [exact]
#  cgsm tz -g=nonBox5.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=halt               [exact]
#  cgsm tz -g=rotatedBox10.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=halt          [exact]
#  cgsm tz -g=bib2e.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=halt                 [exact]
#  cgsm tz -g=rbibe2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=halt         [exact]
#  cgsm tz -g=spheree2.order2 -tz=trig -diss=0.5 -tf=1. -go=halt     
#   -- Cons: track down a bug with cons + edges + Cartesian ... fixed 
#   cgsm tz -g=bib1 -pv=c -tz=poly -diss=0. -degreex=1 -degreet=2 -tp=.02 -bcn=sf -go=halt  [exact]
#   cgsm tz -g=sibe1.order2 -pv=c -tz=trig -diss=0.5 -tf=1. -tp=.02 -bcn=sf -go=halt  
#   cgsm tz -g=bib2e -pv=c -tz=trig -diss=0.5 -tf=1. -tp=.02 -bcn=sf -fx=2 -fy=2 -fz=2 -ft=2 -go=halt  [C=ok, NC=ok]
#   cgsm tz -g=rbibe2.order2 -pv=c -tz=trig -diss=0.5 -tf=1. -tp=.02 -bcn=sf -fx=2 -fy=2 -fz=2 -ft=2 -go=halt  [trouble with SOS-C]
#   cgsm tz -g=box20 -pv=c -tz=trig -diss=0.5 -tf=1. -tp=.02 -bcn=sf -fx=4 -fy=4 -fz=4 -ft=4 -checkGhostErr=1 -go=halt  [ OK ] 
#   cgsm tz -g=rotatedBox4.order2 -pv=c -tz=trig -diss=0.5 -tf=1. -tp=.02 -bcn=sf -fx=4 -fy=4 -fz=4 -ft=4 -go=halt  [ OK ] 
# 
# -variable material properties
#  cgsm -noplot tz -g=square10.hdf -degreex=1 -degreet=1 -diss=0. -tf=2. -pv=c -varMat=1 -go=go [exact]
#
# -variable material properties (Godunov)
#  cgsm -noplot tz -g=square20.hdf -degreex=1 -degreet=1 -tf=2. -pv=g -varMat=1 -go=go 
#  cgsm -noplot tz -g=square40.hdf -degreex=1 -degreet=1 -tf=2. -pv=g -varMat=1 -go=go  [not exact, but second order]
#  cgsm -noplot tz -g=square40.hdf -degreex=2 -degreet=2 -tf=2. -pv=g -varMat=1 -bc=sf -go=go  [second order]
#  cgsm -noplot tz -g=annl40.hdf -degreex=2 -degreet=2 -tf=2. -pv=g -varMat=1 -bc=d -go=go  [second order]
#
# --- add filter:
#  cgsm tz -g=rsise4.order2.hdf -tz=trig -diss=0. -tf=10. -tp=1. -filter=1 -go=halt
# 
#  cgsm tz -g=sice4.order2 -tz=trig -pv=c -diss=1. -tf=10. -tp=.1 -lambda=100 -bc=sf -go=halt
#  cgsm tz -g=sice4.order2 -tz=trig -pv=c -diss=0 -filter=1 -tf=10. -tp=.1 -lambda=100 -bc=sf -go=halt
# 
# -- stress free BC:
#  cgsm tz -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=sf -go=halt
#  cgsm tz -g=cice1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=sf -go=halt
# 
# -- slip wall BC:
#  cgsm tz -g=nonSquare5.hdf -degreex=2 -degreet=2 -diss=0. -tp=.01  -pv=nc -bcn=slip -debug=3 -go=halt  [exact]
#  cgsm tz -g=nonSquare5.hdf -degreex=2 -degreet=2 -diss=0. -tp=.01  -pv=c -bcn=slip -debug=3 -go=halt  [to-do]
#  cgsm tz -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=2. -pv=nc -bcn=slip -go=halt   [exact]
#  cgsm tz -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=2. -pv=c -bcn=slip -go=halt   [exact]
# 
# -- godunov: 
#  cgsm tz -g=square10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt      [exact]
#  cgsm tz -g=square10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -ad=1. -ad4=.5 -go=halt   [exact, artificial diffusion]
#  cgsm tz -g=square10.hdf -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -go=halt      [exact]
#  cgsm tz -g=nonSquare10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt    [exact]
#  cgsm tz -g=rhombus -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -go=halt   [exact]
#  cgsm tz -g=rhombus -pv=g -tz=trig -tf=.5 -tp=.1 -go=halt  
#  cgsm tz -g=rhomboid -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -go=halt   [exact]
#    trouble: t=.3 -- bug fixed (negative Jacobian problem)
#  cgsm tz -g=rhomboid -pv=g -tz=trig -tf=1. -tp=.1 -bc=dirichlet -go=halt  
#  cgsm tz -g=rhomboid -pv=g -degreex=2 -degreet=0 -tf=1. -tp=.1 -bc=d -go=halt  [exact]
#  cgsm tz -g=rhombus20 -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -go=halt   
#  cgsm tz -g=rotatedSquare10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt    [exact]
#  cgsm tz -g=sis.hdf -pv=g -degreex=1 -degreet=1 -tf=1. -tp=.01 -go=halt      [exact]
#  cgsm tz -g=rsis2.hdf -pv=g -degreex=1 -degreet=1 -tf=1. -tp=.05 -go=halt
#  cgsm tz -g=cice2.order2 -pv=g -tz=trig -tf=1. -tp=.05 -go=halt
#    -- square beside a square: (for testing the case when grids overlap on a boundary)
#  cgsm tz -g=sbs.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=1   [exact]
#  cgsm tz -g=rotatedBox1.order2 -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -go=halt -checkGhostErr=1 [exact]
#  cgsm tz -g=rotatedBox1.order2 -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -bcn=sf -go=halt -checkGhostErr=1 [exact]
#    ** trouble: 
#  cgsm tz -g=wiggley3d1.order2 -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -bc=dirichlet -go=halt -checkGhostErr=1
#    -- box beside a box: 
#  cgsm tz -g=bbb.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=1   [exact]
#  cgsm tz -g=rotatedBoxBesideBoxi1.order2 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=1   [exact]
# 
#     --- slip-wall BC
#  cgsm tz -g=square10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=slip -go=halt 
#     -- traction-BC
#  cgsm tz -g=square10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=sf -go=halt      [exact]
#  cgsm tz -g=nonSquare10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=sf -go=halt      [exact]
#  cgsm tz -g=rotatedSquare10.hdf -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -bcn=sf -go=halt    [exact]
#  cgsm tz -g=rotatedSquare10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=sf -go=halt      [exact]
#  cgsm tz -g=annulus1.order2 -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.05 -bcn=sf -go=halt 
#  cgsm tz -g=quarterAnnulus -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.05 -bcn=sf -go=halt   ** trouble **
#  cgsm tz -g=quarterAnnulus -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.05 -bcn=mixed -bg=quarter -go=halt  OK
#  cgsm tz -g=quarterAnnulus -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.05 -bcn=mixed2 -bg=quarter -go=halt  OK
# 
#  cgsm tz -g=sbs.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=sf -go=halt -checkGhostErr=1 [ exact ]
#  cgsm tz -g=bbb.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=sf -go=halt -checkGhostErr=1 [ exact ]
#  cgsm tz -g=rotatedBoxBesideBoxi1.order2 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=sf -go=halt -checkGhostErr=1   [exact]
# 
#     -- mixed BC's : traction + displacement 
#  cgsm tz -g=square10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -bcn=mixed -go=halt   [exact]
#  cgsm tz -g=square10.hdf -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -bcn=mixed -go=halt   [exact]
#  cgsm tz -g=nonSquare10.hdf -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -bcn=mixed -go=halt   [exact]
#  cgsm tz -g=rotatedSquare10.hdf -pv=g -degreex=2 -degreet=0 -tf=.5 -tp=.01 -bcn=mixed -go=halt   [exact] *NO* 
#     -- 3D
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=dirichlet -bg=box   [exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=d -bg=box  [exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=sf -bg=box [exact]
#  cgsm tz -g=rotatedBox10 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=d -bg=box  [exact]
#  cgsm tz -g=rotatedBox10 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=sf -bg=box [exact]
#  cgsm tz -g=rbibe2.order2 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=d -bg=box  [exact]
#  cgsm tz -g=rbibe2.order2 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=sf -bg=box [exact]
#  cgsm tz -g=sibe1.order2 -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=d -bg=box  
#  cgsm tz -g=stretchedBox1.order2.ng2.hdf -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=dirichlet -bg=box -tangentialStressDissipation=1. -godunovType=1 -godunovOrder=1
#  cgsm tz -g=stretchedBox1.order2.ng2.hdf -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=dirichlet -bg=box -tangentialStressDissipation=1. -godunovType=1 -godunovOrder=2
#  cgsm tz -g=sphereFixede4.order2.hdf -pv=g -degreex=2 -degreet=2 -tf=.5 -tp=.01 -go=halt -checkGhostErr=2 -bcn=dirichlet -bg=box -tangentialStressDissipation=1. -godunovType=1 -godunovOrder=2
# 
#   -- test displacement BC on one face at a time: (jwb)
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D00 -bg=box	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D01 -bg=box	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D02 -bg=box	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D10 -bg=box	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D11 -bg=box	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D12 -bg=box	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D00 -bg=box	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D01 -bg=box	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D02 -bg=box	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D10 -bg=box	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D11 -bg=box	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D12 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D00 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D01 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D02 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D10 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D11 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=dis3D12 -bg=box	[exact]
#
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D00 -bg=box	 	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D01 -bg=box	 	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D02 -bg=box	 	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D10 -bg=box	 	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D11 -bg=box	 	[exact]
#  cgsm tz -g=box10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D12 -bg=box	 	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D00 -bg=box  	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D01 -bg=box  	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D02 -bg=box  	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D10 -bg=box  	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D11 -bg=box  	[exact]
#  cgsm tz -g=nonBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D12 -bg=box  	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D00 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D01 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D02 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D10 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D11 -bg=box	[exact]
#  cgsm tz -g=rotatedBox10.hdf -pv=g -degreex=1 -degreet=1 -tf=.5 -tp=.01 -go=halt -bcn=sf3D12 -bg=box	[exact]
#
#  cgsm tz -g=sib1.hdf -degreex=1 -degreet=1 -diss=0. -tf=1. -go=halt -pv=g
#  cgsm tz -g=spheree1.order2 -tz=trig -diss=0. -tf=1. -tp=.02 -pv=g -go=halt     
# 
#*** MOL time-stepping:
#  cgsm tz -g=square5.hdf -degreex=2 -degreet=2 -diss=0. -tf=2. -tp=.01 -ts=fe  -debug=1 -go=halt [exact]
#  cgsm tz -g=square5.hdf -degreex=2 -degreet=1 -diss=0. -tf=2. -tp=.01 -ts=ie  -debug=1 -go=halt -pv=g [exact]
# 
# --- AMR:
#  cgsm tz -g=rsise4.order2 -pv=g -amr=1 -diss=0. -filter=1 -tp=.05 -tf=1. -tol=.01 -tz=pulse -x0=-.5 -y0=-.5 -bcn=dirichlet -go=halt
#  cgsm tz -g=rsise8.order2 -pv=g -amr=1 -diss=0. -filter=1 -tp=.05 -tf=1. -tol=.001 -tz=pulse -x0=-.5 -y0=-.5 -bcn=dirichlet -go=halt
# 
# parallel: 
#  mpirun -np 1 $cgsmp tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt 
#  mpirun -np 1 $cgsmp noplot tz.cmd -g=square5.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go -debug=15
#  mpirun -np 2 $cgsmp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go 
#  srun -N1 -n1 -ppdebug $cgsmp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt 
#  totalview srun -a -N1 -n4 -ppdebug $cgsmp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
#  srun -ppdebug -N2 -n2 memcheck_all $cgsmp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 
#  srun -N1 -n4 -ppdebug $cgsmp tz.cmd -g=sibFixedSmalle2.order2 -tz=trig -filter=1 -tf=.5 -tp=.1 -go=halt 
# 
#  mpirun -np 2 $cgsmp noplot tz -g=sise2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go
#  mpirun -np 4 $cgsmp noplot tz -g=nonBox8.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
#  mpirun -np 1 $cgsmp noplot tz -g=rbibe1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go
#  totalview srun -a -N1 -n2 -ppdebug $cgsmp noplot tz -g=rbibe1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go
#  mpirun -np 4 $cgsmp noplot tz -g=rbibe2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
# 
#  -- track down bug with sibFixed4 and -N8 -n64
# bug: 
# srun -N4 -n32 -ppdebug $cgsmp noplot tz -g=rsise2.order2 -pv=nc -degreex=0 -degreet=0 -tf=.04 -tp=0.02 -tz=poly -bcn=d -diss=0 -filter=1 -debug=3 -go=go -numberOfParallelGhost=2 > ! junk
#  ok: 
# srun -N2 -n16 -ppdebug $cgsmp noplot tz -g=rsise2.order2 -pv=nc -degreex=2 -degreet=2 -tf=.5 -tp=0.1 -tz=poly -bcn=d -diss=0 -filter=1 -go=go
#  bug: also bug with filter=0, ad=1.; 
# srun -N4 -n32 -ppdebug $cgsmp noplot tz -g=rsise2.order2 -pv=nc -degreex=2 -degreet=2 -tf=.5 -tp=0.1 -tz=poly -bcn=d -diss=0 -filter=1 -go=go
#  ok with filter=0
# srun -N4 -n32 -ppdebug $cgsmp noplot tz -g=rsise2.order2 -pv=nc -degreex=2 -degreet=2 -tf=.5 -tp=0.1 -tz=poly -bcn=d -diss=0 -filter=0 -go=go
# ok:
# srun -N2 -n16 -ppdebug $cgsmp noplot tz -g=rsise1.order2 -pv=nc -degreex=2 -degreet=2 -tf=.5 -tp=0.1 -tz=poly -bcn=d -diss=0 -filter=1 -go=go
# ok:
# srun -N3 -n24 -ppdebug $cgsmp noplot tz -g=rsise1.order2 -pv=nc -degreex=2 -degreet=2 -tf=.5 -tp=0.1 -tz=poly -bcn=d -diss=0 -filter=1 -go=go
# ok: 
# srun -N4 -n32 -ppdebug $cgsmp noplot tz -g=rsise1.order2 -pv=nc -degreex=2 -degreet=2 -tf=.5 -tp=0.1 -tz=poly -bcn=d -diss=0 -filter=1 -go=go
# 
# --- set default values for parameters ---
# 
$noplot=""; $backGround="square"; $grid="square10"; $rho=1.; $mu=1.; $lambda=1.; $pv="nc"; $en="max"; $show=" ";
$debug = 0;  $tPlot=.1; $diss=.1; $dissOrder=2; $bcn="d"; $cons=0; $godunovOrder=2;
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=$fx; $fz=$fx; $ft=$fx;  $ts="me"; 
$order = 2; $go="run"; $checkGhostErr=0; 
$tFinal=1.; $cfl=.9; $dsf=.1; $filter=0; $filterOrder=6; $filterStages=2; $filterFrequency=1; $filterIterations=1; 
$ad=0.; $ad4=0.; # art. diss for Godunov
$amr=0; $useTopHat=0; $xTopHat=.25; $yTopHat=.25; $zTopHat=0.; $x0=.5; $y0=.5; $z0=.5;
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tol=.001;  $nbz=2;   # amr tol and number-of-buffer-zones
$stressRelaxation=0; $relaxAlpha=0.1; $relaxDelta=0.1; $varMat=0;
$tangentialStressDissipation=1.;
$bc1=""; $bc2=""; $bc3=""; $bc4=""; $bc5=""; $bc6=""; 
$godunovType=0;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"varMat=f"=>\$varMat,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"rho=f"=>\$rho,"godunovType=i"=>\$godunovType,\
  "dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"pv=s"=>\$pv,\
  "godunovOrder=f"=>\$godunovOrder,"ts=s"=>\$ts,"en=s"=>\$en,"fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"ft=f"=>\$ft,\
  "dissOrder=i"=>\$dissOrder,"filter=i"=>\$filter,"filterOrder=i"=>\$filterOrder,"filterStages=i"=>\$filterStages,\
  "filterFrequency=i"=>\$filterFrequency,"filterIterations=i"=>\$filterIterations,"checkGhostErr=i"=>\$checkGhostErr,\
  "ad=f"=>\$ad,"ad4=f"=>\$ad4,"stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,\
  "relaxDelta=f"=>\$relaxDelta,"tangentialStressDissipation=f"=>\$tangentialStressDissipation,\
  "bc1=s"=>\$bc1,"bc2=s"=>\$bc2,"bc3=s"=>\$bc3,"bc4=s"=>\$bc4,"bc5=s"=>\$bc5,"bc6=s"=>\$bc6,\
  "amr=i"=>\$amr,"tol=f"=>\$tol,"nrl=i"=>\$nrl,"nbz=i"=>\$nbz,"ratio=i"=>\$ratio,"useTopHat=i"=>\$useTopHat,\
  "xTopHat=f"=>\$xTopHat,"yTopHat=f"=>\$yTopHat,"zTopHat=f"=>\$zTopHat,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0 );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }
if( $tz eq "trig" ){ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $pv eq "nc" ){ $pv = "non-conservative"; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
#
if( $ts eq "me" ){ $ts = "modifiedEquationTimeStepping"; }
if( $ts eq "fe" ){ $ts = "forwardEuler"; }
if( $ts eq "ie" ){ $ts = "improvedEuler"; }
if( $ts eq "ab" ){ $ts = "adamsBashforth2"; }
# 
if( $bcn eq "d" ){ $bcn = "all=displacementBC"; }
if( $bcn eq "dirichlet" ){ $bcn = "all=dirichletBoundaryCondition"; }
if( $bcn eq "sf" ){ $bcn = "all=tractionBC\n"; }
if( $bcn eq "slip" ){ $bcn = "all=slipWall\n"; }
if( $bcn eq "mixed" ){ $bcn = "all=displacementBC\n $backGround(0,0)=tractionBC\n $backGround(1,0)=tractionBC"; }
if( $bcn eq "mixed2" ){ $bcn = "all=displacementBC\n $backGround(0,1)=tractionBC\n $backGround(1,1)=tractionBC"; }
if( $bcn eq "mixed3" ){ $bcn = "all=tractionBC\n $backGround(0,0)=displacementBC\n $backGround(1,0)=displacementBC"; }
if( $bcn eq "dis3D00" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(0,0)=displacementBC\n"; }
if( $bcn eq "dis3D01" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(0,1)=displacementBC\n"; }
if( $bcn eq "dis3D02" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(0,2)=displacementBC\n"; }
if( $bcn eq "dis3D10" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(1,0)=displacementBC\n"; }
if( $bcn eq "dis3D11" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(1,1)=displacementBC\n"; }
if( $bcn eq "dis3D12" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(1,2)=displacementBC\n"; }
if( $bcn eq "sf3D00" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(0,0)=tractionBC\n"; }
if( $bcn eq "sf3D01" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(0,1)=tractionBC\n"; }
if( $bcn eq "sf3D02" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(0,2)=tractionBC\n"; }
if( $bcn eq "sf3D10" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(1,0)=tractionBC\n"; }
if( $bcn eq "sf3D11" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(1,1)=tractionBC\n"; }
if( $bcn eq "sf3D12" ){ $bcn = "all=dirichletBoundaryCondition\n $backGround(1,2)=tractionBC\n"; }
#
# NEW way for BC's
if( $bc1 eq "dirichlet" ){ $bc1="bcNumber1=dirichletBoundaryCondition"; }\
     elsif( $bc1 eq "d" ){ $bc1="bcNumber1=displacementBC"; }\
     elsif( $bc1 eq "t" ){ $bc1="bcNumber1=tractionBC"; }\
     elsif( $bc1 eq "s" ){ $bc1="bcNumber1=slipWall"; }\
     elsif( $bc1 eq "sym" ){ $bc1="bcNumber1=symmetry"; }\
     else{ $bc1="#"; }
if( $bc2 eq "dirichlet" ){ $bc2="bcNumber2=dirichletBoundaryCondition"; }\
     elsif( $bc2 eq "d" ){ $bc2="bcNumber2=displacementBC"; }\
     elsif( $bc2 eq "t" ){ $bc2="bcNumber2=tractionBC"; }\
     elsif( $bc2 eq "s" ){ $bc2="bcNumber2=slipWall"; }\
     elsif( $bc2 eq "sym" ){ $bc2="bcNumber2=symmetry"; }\
     else{ $bc2="#"; }
if( $bc3 eq "dirichlet" ){ $bc3="bcNumber3=dirichletBoundaryCondition"; }\
     elsif( $bc3 eq "d" ){ $bc3="bcNumber3=displacementBC"; }\
     elsif( $bc3 eq "t" ){ $bc3="bcNumber3=tractionBC"; }\
     elsif( $bc3 eq "s" ){ $bc3="bcNumber3=slipWall"; }\
     elsif( $bc3 eq "sym" ){ $bc3="bcNumber3=symmetry"; }\
     else{ $bc3="#"; }
if( $bc4 eq "dirichlet" ){ $bc4="bcNumber4=dirichletBoundaryCondition"; }\
     elsif( $bc4 eq "d" ){ $bc4="bcNumber4=displacementBC"; }\
     elsif( $bc4 eq "t" ){ $bc4="bcNumber4=tractionBC"; }\
     elsif( $bc4 eq "s" ){ $bc4="bcNumber4=slipWall"; }\
     elsif( $bc4 eq "sym" ){ $bc4="bcNumber4=symmetry"; }\
     else{ $bc4="#"; }
if( $bc5 eq "dirichlet" ){ $bc5="bcNumber5=dirichletBoundaryCondition"; }\
     elsif( $bc5 eq "d" ){ $bc5="bcNumber5=displacementBC"; }\
     elsif( $bc5 eq "t" ){ $bc5="bcNumber5=tractionBC"; }\
     elsif( $bc5 eq "s" ){ $bc5="bcNumber5=slipWall"; }\
     elsif( $bc5 eq "sym" ){ $bc5="bcNumber5=symmetry"; }\
     else{ $bc5="#"; }
if( $bc6 eq "dirichlet" ){ $bc6="bcNumber6=dirichletBoundaryCondition"; }\
     elsif( $bc6 eq "d" ){ $bc6="bcNumber6=displacementBC"; }\
     elsif( $bc6 eq "t" ){ $bc6="bcNumber6=tractionBC"; }\
     elsif( $bc6 eq "s" ){ $bc6="bcNumber6=slipWall"; }\
     elsif( $bc6 eq "sym" ){ $bc6="bcNumber6=symmetry"; }\
     else{ $bc6="#"; }
#
if( $en eq "max" ){ $errorNorm="maximum norm"; }
if( $en eq "l1" ){ $errorNorm="l1 norm"; }
if( $en eq "l2" ){ $errorNorm="l2 norm"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
# -new: set-up stage: 
linear elasticity
variable material properties $varMat
$pv
 continue
#  -- time stepping method : 
$ts
# 
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations $filterIterations \n filter coefficient 1. \n  filter stages $filterStages \n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
# 
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:rho $rho
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:stressRelaxation $stressRelaxation
SMPDE:relaxAlpha $relaxAlpha
SMPDE:relaxDelta $relaxDelta
SMPDE:tangential stress dissipation $tangentialStressDissipation
#
# SVK and other nonlinear models:
SMPDE:PDE type for Godunov $godunovType
# 
OBTZ:$tz
OBTZ:twilight zone flow 1
OBTZ:degree in space $degreex
OBTZ:degree in time $degreet
OBTZ:frequencies (x,y,z,t) $fx $fy $fz $ft
$pulseAmp=1.; $pulsePower=1; $pulseExponent=30.; 
OBTZ:pulse amplitude, exponent, power $pulseAmp $pulseExponent $pulsePower
OBTZ:pulse center $x0 $y0 $z0
OBTZ:pulse velocity 1 1 1
OBTZ:$errorNorm
# 
#
final time $tFinal
times to plot $tPlot
dissipation $diss
order of dissipation $dissOrder
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad 
SMPDE:fourth-order artificial diffusion $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 
cfl $cfl
use conservative difference $cons
# Adjust TZ coeff's for nonlinear models
if( $godunovType eq 2 ){ $tzCmds = \
  "OBTZ:assign polynomial coefficients\n " . \
   "ct(0,0)=1.1e-3\n " . \
   "ct(1,0)=3.2e-3\n " . \
   "ct(2,0)=2.4e-3\n " . \
   "ct(0,1)=1.7e-3\n " . \
   "ct(1,1)=2.2e-3\n " . \
   "ct(2,1)=2.3e-3\n " . \
   "ct(0,2)=1.8e-3\n " . \
   "ct(1,2)=3.5e-3\n " . \
   "ct(2,2)=1.3e-3\n " . \
   "ct(0,3)=2.4e-3\n " . \
   "ct(1,3)=3.6e-3\n " . \
   "ct(2,3)=1.9e-3\n " . \
   "ct(0,4)=2.1e-3\n " . \
   "ct(1,4)=3.2e-3\n " . \
   "ct(2,4)=1.5e-3\n " . \
   "ct(0,5)=1.4e-3\n " . \
   "ct(1,5)=2.6e-3\n " . \
   "ct(2,5)=2.3e-3\n " . \
   "ct(0,6)=2.4e-3\n " . \
   "ct(1,6)=3.8e-3\n " . \
   "ct(2,6)=3.6e-3\n " . \
   "ct(0,7)=1.5e-3\n " . \
   "ct(1,7)=3.3e-3\n " . \
   "ct(2,7)=1.4e-3\n " . \
   "done"; }else{ $tzCmds ="*"; }
$tzCmds
# 
#
# bc: $backGround=stressFree
#  -- for now we don't handle a corner where two stree free Bc's meet: 
#  bc: all=dirichlet
#  bc: $backGround(0,1)=stressFree
#  bc: $backGround(1,1)=stressFree
boundary conditions
  $bcn
$bc1
$bc2
$bc3
$bc4
$bc5
$bc6
# bcNumber1=symmetry
# bcNumber2=symmetry
# bcNumber3=tractionBC
# bcNumber4=tractionBC
#
#  all=dirichletBoundaryCondition
#  square(0,1)=tractionBC
#  box(0,1)=tractionBC
done
#
# -------------------Start AMR commands------------------------------
if( $amr eq "1" ){ $cmd =" turn on adaptive grids"; }else{ $cmd =" turn off adaptive grids"; }
$cmd
#   save error function to the show file
  show amr error function 0
#
if( $useTopHat eq 1 ){ $cmds = "use top-hat for error function"; }else{ $cmds="#"; }
$cmds
#
  top hat parameters
    $xTopHat $yTopHat $zTopHat
    .125
    1. 1. 0.
#
  $amrInterpOrder=3; # =2; 
  order of AMR interpolation $amrInterpOrder
  error threshold
     $tol 
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
  change error estimator parameters
    weight for first difference
      1.
    weight for second difference
      1.
    default number of smooths
      1
    set scale factors     
      1 1 1 1 1 1 1 1 1 1 1
    done
    exit
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
      $nbz
#    width of proper nesting 
#      1
    grid efficiency
      .7 
  exit
# -----------------End AMR commands---------------------------------------------
displacement scale factor $dsf
debug $debug
check errors 1
plot errors 1
# optionally check the error on this many ghost points:
check error on ghost
   $checkGhostErr
# 
if( $pv eq "conservative" || $pv eq "non-conservative" ){ $cmds="plot velocity 1\n plot stress 1"; }else{ $cmds="#"; }
$cmds
#*********************************
show file options...
  OBPSF:compressed
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush 20
exit
#**********************************
# 
continue
$go

#
# set view:0 0 0 0 1 0.976179 0.0166626 -0.216324 0.0226901 0.983739 0.178164 0.215775 -0.178829 0.959928
  erase
  contour
    ghost lines $checkGhostErr
    erase
    exit
  contour
    exit
  plot:v1-error
