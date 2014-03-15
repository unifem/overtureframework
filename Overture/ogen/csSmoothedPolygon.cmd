*
*  Demonstrate the use of the CrossSection Mapping -- build a pipe with
*  cross-sections defined by smoothed polygons
*
create mappings
*
SmoothedPolygon
  vertices
    6
     0. .25
     .125 .25
    .125 -.25
    -.125 -.25
    -.125 .25
     0. .25
  periodicity
    2
  n-dist
    fixed normal distance
      .075
  t-stretch
    0.  50.
    .15 50.
    .15 50.
    .15 50.
    .15 50.
    .0 50.
  make 3d (toggle)
    0.
  mappingName
    level0
exit
SmoothedPolygon
  vertices
    6
     0. .25
     .125 .25
    .125 -.25
    -.125 -.25
    -.125 .25
     0. .25
  periodicity
    2
  n-dist
    fixed normal distance
      .075
  t-stretch
    0.  50.
    .15 50.
    .15 50.
    .15 50.
    .15 50.
    .0 50.
  make 3d (toggle)
    .4
  mappingName
    level1
exit
SmoothedPolygon
  vertices
    6
     0.  .25
     .25 .25
    .25 -.25
    -.25 -.25
    -.25 .25
     0.  .25
  periodicity
    2
  n-dist
    fixed normal distance
      .075
  t-stretch
    0.  50.
    .15 50.
    .15 50.
    .15 50.
    .15 50.
    .0 50.
  make 3d (toggle)
    .6
  mappingName
    level2
exit
SmoothedPolygon
  vertices
    6
     0.  .25
     .25 .25
    .25 -.25
    -.25 -.25
    -.25 .25
     0.  .25
  periodicity
    2
  n-dist
    fixed normal distance
      .075
  t-stretch
    0.  50.
    .15 50.
    .15 50.
    .15 50.
    .15 50.
    .0 50.
  make 3d (toggle)
    1.
  mappingName
    level3
exit
*
* make a cross section mapping
CrossSection
  general
  4
  level0
  level1
  level2
  level3
  mappingName
   pipe
  exit
*
exit
*
* 
generate an overlapping grid
    pipe
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
* pause
  exit
*
save an overlapping grid
spPipe.hdf
pipe
exit