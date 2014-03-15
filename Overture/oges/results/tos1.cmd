* 
* Input parameters for tos
*
$debug=0; $bc="dirichlet"; 
* $bc="mixed";
*
* $grid="square5";
* $grid="square32";
* $grid="square128";
* $grid="square256";
* $grid="square512";
* $grid="cice";
* $grid="cic2e";
* $grid="cic3e";
* $grid="cic4e";
$grid="cic5e";
* $grid="cic6e";
* $grid="twoBumpe";
* $grid="twoBump3";
* $grid="twoBump4
* $grid="twoBump5"";;
$grid="box40"; 
* $grid="cylinderInAShortChannel1"; 
* $grid="cylinderInAChannel1"; 
* $grid="cylinderInAChannel2"; 
*
$iluLevels=2; $asmOverlap=1; 
* 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $resultsFile="$grid.$ksp.$pc.$subksp.$subpc$iluLevels.$bc.results";
** $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="lu"; $resultsFile="$grid.$ksp.$pc.$subksp.$subpc.$bc.results";
$ksp="bcgs"; $pc="hypre"; $resultsFile="$grid.$ksp.$pc.amg.$bc.results"; 
*
* $ksp="bcgs"; $pc="asm"; $subksp="preonly"; $subpc="ilu"; $resultsFile="$grid.$ksp.$pc$asmOverlap.$subksp.$subpc$iluLevels.$bc.results";
* 
* --- assign options ---
grid=$grid.hdf
results=$resultsFile
solveAgainWithConvergedSolution 1 
debug $debug 
bc=$bc 
done
*
* ---- oges parameters: 
parallel bi-conjugate gradient stabilized
number of incomplete LU levels
  $iluLevels 
*  parallel generalized minimal residual
  define petscOption -ksp_monitor stdout
*  define petscOption -log_summary hh
* 
   define petscOption -ksp_type $ksp
   define petscOption -pc_type $pc
   define petscOption -sub_ksp_type $subksp
   define petscOption -sub_pc_type $subpc
*
*  For additive schwartz method (ASM)
   define petscOption -pc_asm_overlap $asmOverlap 
*
   * these next don't seem to work:
* --old:
*    define petscOption -sub_pc_ilu_levels $iluLevels 
*    define petscOption -pc_ilu_levels $iluLevels
*   -- new way: but these do not over-ride the above!
   define petscOption -pc_factor_levels $iluLevels
   define petscOption -sub_pc_factor_levels $iluLevels
*
* hypre amg:
**     define petscOption -pc_type hypre
     define petscOption -pc_hypre_type boomeramg
     define petscOption -pc_hypre_boomeramg_strong_threshold .5
     * -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
     define petscOption -pc_hypre_boomeramg_coarsen_type Falgout
     define petscOption -pc_hypre_boomeramg_print_statistics 0 
 exit
