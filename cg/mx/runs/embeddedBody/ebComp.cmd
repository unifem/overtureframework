#
#  Use the "comp" code to compute the differences bwetween two show files.
# G4: 
#    comp -noplot ebComp.cmd -show1=ebG4Theta45.show -show2=enbG4Theta45.show -output=ebG4Theta45diff.show
# G4, periodic:
#    comp -noplot ebComp.cmd -show1=ebG4pTheta60.show -show2=enbG4pTheta60.show -output=ebG4pTheta60diff.show
$show1=""; $show2=""; $output=""; 
GetOptions( "show1=s"=>\$show1,"show2=s"=>\$show2,"output=s"=>\$output );
specify files
  # put solution with body first  ("coarse" grdi will stor the differences)
  $show1
  $show2
exit
# choose a solution
# -1
# compute errors
#
output show file: $output
save differences to show file
exit