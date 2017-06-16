#
#  Plot the heart valve grid
#
#  plotStuff plotHeartValveGrid.cmd -grid=heartValveGridi1.order2.hdf 
#
$grid="heartValveGridi1.order2.hdf";
GetOptions( "grid=s"=>\$grid );
#
$grid
# 
  grid colour 2 GRAY40 #BRASS
  grid colour 3 GRAY40 #BRASS
  grid colour 4 GRAY40 #BRASS
  grid colour 5 GRAY40 #BRASS
  grid colour 6 GRAY40 #BRASS
  grid colour 7 GRAY40 #BRASS
#
toggle boundary 1 0 0 0
  toggle boundary 1 1 1 0
  plot block boundaries 0
  toggle grid lines on boundary 1 0 0 0
  toggle grid lines on boundary 1 1 1 0
  #toggle grid lines on boundary 2 0 0 2
  set view:0 0.010181 0.00226244 0 0.959826 0.508022 -0.262003 0.820529 -0.0593912 0.939693 0.336824 -0.859294 -0.219846 0.461824

