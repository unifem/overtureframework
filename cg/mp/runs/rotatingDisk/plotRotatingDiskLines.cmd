#
# Plot results from rotatingDisk.cmd
#  -- plot lines through the solution
#
#   plotStuff plotRotatingDiskLines -show=rotatingDisk4.show
#   plotStuff plotRotatingDiskLines -show=rotatingDisk4_scf10.show -name=rotatingDisk4_scf10
#
$show="rotatingDisk4.show"; $name="rotatingDisk4"; $solution=11; 
$sc="stressNorm"; $fc="p"; # component names
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution );
#
$show
#
# -- set displacement components for Godunov 
frame series:innerDomain
# 
# previous
# -- solution at t=1: (solution=11)
solution: $solution
contour
  adjust mapping for displacement 1
exit
contour
#  adjust grid for displacement 1
#  plot contour lines (toggle)
  vertical scale factor 0.
  # stress-Norm: 
  # min max 0 .54
  line plots
   set bogus value for points not interpolated
      -123456.
    specify lines
    1 201
    -1 0 1 0
      v1
      add v2
      add s11
      add s12
      add s21
      add s22
      add u
      add v
      add x0
      add x1
      add v1Err
      add v2Err
      add cs11
      add cs12
      add cs21
      add cs22
# 
      save results to a matlab file
      $matlabName=$name . "_Solid_HorizontalLine.m"; 
      $matlabName
      exit this menu
    exit this menu
#
  exit
# 
frame series:outerDomain
plot:p
contour
#  plot contour lines (toggle)
  vertical scale factor 0.
  # p: 
  # min max .5 1.0001 
  line plots
    clip to boundary
    set bogus value for points not interpolated
      -123456.
    specify lines
      1 301
     -2 0 2 0
      r
      add u
      add v
      add T
      add p
      add x0
      add x1
      add rhoErr
      add uErr
      add vErr
      add TErr
      save results to a matlab file
      $matlabName=$name . "_Fluid_HorizontalLine.m"; 
      $matlabName
      exit this menu
    exit this menu
 exit
# 



# 
frame series:outerDomain
plot:p
contour
#  plot contour lines (toggle)
  vertical scale factor 0.
  # p: 
  # min max .5 1.0001 
  line plots
    specify lines
    2 101
    -2 0 -1 0
     1 0 2 0
      rho_line_0
      add u_line_0
      add v_line_0
      add T_line_0
      add p_line_0
      add x0_line_0
      add x1_line_0
      add rho_line_1
      add u_line_1
      add v_line_1
      add T_line_1
      add p_line_1
      add x0_line_1
      add x1_line_1
      save results to a matlab file
      $matlabName=$name . "_Fluid_HorizontalLine.m"; 
      $matlabName
      exit this menu
    exit this menu
 exit
# 






DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
set view:0 -0.00598171 -0.0691154 0 1.10155 1 0 0 0 1 0 0 0 1
#
solution: 6
hardcopy file name:0 rotatingDisk4_sNorm_p_t0p5.ps
hardcopy save:0
pause
#
solution: 11
hardcopy file name:0 rotatingDisk4_sNorm_p_t1p0.ps
hardcopy save:0
pause
#
# -- now plot horizontal velocity 
frame series:innerDomain
plot:v1
frame series:outerDomain
plot:u
#
solution: 6
hardcopy file name:0 rotatingDisk4_v1_u_t0p5.ps
hardcopy save:0
pause
#
solution: 11
hardcopy file name:0 rotatingDisk4_v1_u_t1p0.ps
hardcopy save:0


frame series:solidDomain
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
bigger:0
y-:0
y-:0
hardcopy file name:0 dd16_p_sNorm.ps
hardcopy save:0




# 
pause
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 0.0054308 -0.0528746 0 1.22673 1 0 0 0 1 0 0 0 1

plot:v2
hardcopy file name:0 ss64nsv2rt1p0.ps

$plotName = $name . "v1rt0p5.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0