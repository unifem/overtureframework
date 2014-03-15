*
*		Assuming the rocket cross section mappings work as expected, this ogen command file
*		will create a rocket consisting of a star grain section on top which blends to annular
*               grain section in the middle, which then blends to a nozzel.  I have not tested this
*               command file with a version of ogen including the rocket cross sections, so it may
*               have errors
*
create mappings
*
*		The first step is to create a series of cross sections describing the mesh at
*               transition points.  All cross sections must be parameterized in an equivalent way.
*		therefore, all cross sections should have the same number of lines.  Additonally it
*		is benifical to make the number of lines divisible by the number of vertices
*
*		For this example, evenly spaced cross sections will be produced.  There will
*               exist 1 cross section per length unit in the z direction
*
*
  rocket (3D)
    star
    set number of vertices
     7
    set z value
      20
    lines
      350
    mappingName
      star_top0
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      19
    lines
      350
    mappingName
      star_top1
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      18
    lines
      350
    mappingName
      star_top2
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      17
    lines
      350
    mappingName
      star_top3
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      16
    lines
      350
    mappingName
      star_top4
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      15
    lines
      350
    mappingName
      star_top5
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      14
    lines
      350
    mappingName
      star_top6
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      13
    lines
      350
    mappingName
      star_top7
    set bounding radii
      1.0 3.0
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      12
    lines
      350
    mappingName
      star_top8
    set bounding radii
      1.0 3.0
  exit
*
*		Create some cross sections in the transition region.  The outer radius of the star
*               is 3.0 the inner radius of the star is 1.0.  the transition will be obtained to 
*               a circle of radius 2.0 by blending both outer and inner radi to 2.0
*
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      11
    lines
      350
    mappingName
      star_trans0
    set bounding radii
      1.2 2.8
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      10
    lines
      350
    mappingName
      star_trans1
    set bounding radii
      1.5 2.5
  exit
  rocket (3D)
    star
    set number of vertices
      7
    set z value
      9
    lines
      350
    mappingName
      star_trans2
    set bounding radii
      1.8 2.2
  exit
*
*		Create some sections for the annular grain
*
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      8
    lines
      350
    mappingName
      circ0
    set radius
      2.0
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      7
    lines
      350
    mappingName
      circ1
    set radius
      2.0
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      6
    lines
      350
    mappingName
      circ2
    set radius
      2.0
  exit
*
*		Create the nozzel by creating cross sections which decrease in radius towards
*               the throat of the nozzel, and then increase in radius for the nozzel exit
*
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      5
    lines
      350
    mappingName
      noz0
    set radius
      2.0
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      4
    lines
      350
    mappingName
      noz1
    set radius
      1.5
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      3
    lines
      350
    mappingName
      noz2
    set radius
      1.0
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      2
    lines
      350
    mappingName
      noz3
    set radius
      1.5
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      1
    lines
      350
    mappingName
      noz4
    set radius
      1.7
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      0
    lines
      350
    mappingName
      noz5
    set radius
      1.7
  exit
  rocket (3D)
    circle
    set number of vertices
      7
    set z value
      -1
    lines
      350
    mappingName
      noz6
    set radius
      1.7
  exit
*
*		Now that all the required cross sections have been defined the next step
* 		is to link all these cross sections together into a shell on the outside surface.
*		This is done by creating a cross section mapping passing through all the defined
*               cross sections
*
crossSection   
      index
      cubic
      mappingName
        rocket_shell
      general
        22
        star_top0
        star_top1
        star_top2
        star_top3
        star_top4
        star_top5
        star_top6
        star_top7
        star_top8
        star_trans0
        star_trans1
        star_trans2
        circ0
        circ1
        circ2
        noz0
        noz1
        noz2
        noz3
        noz4
        noz5
        noz6
      lines
        350 45       
  exit
*
*		The next step is to transform the boundary shell into a boundary volume this is done
*               by performing a mapping from normals from the rocket_shell.  In order to avoid mesh 
*               entaglement, and to prevent bad aspect ratio cells, it is best to keep the distance
*               of normal mapping less than both 0.5*outer_fillet_radius, and
*               1.0*inner_fillet_radius
*
  mapping from normals
    mappingName  
      rocket_volume
    extend normals from which mapping
      rocket_shell
    normal distance
      .125 -.125 -0.075
    lines
      350 91 6  * 350 45 4  
    boundary conditions
      -1 -1 1 1 1 0
    share
      0 0 2 3 0 0
  exit
*
*		   A back ground grid is created for this grid star volume grid to overlap with
*
*   box 
*     set corners
*       -3.0 3.0 -3.0 3.0 -1 20
*     lines
*       90 90 160   90 90 120  125 125  90  100 100 80  80 80 60 60 60 45
*     boundary conditions
*       0 0 0 0 1 1
*     share
*       0 0 0 0 3 2 
*     mappingName
*       background
*   exit
  box 
    set corners
      -2.0 2.0 -2.0 2.0 -1 8.
    lines
      90 90 120  * 60 60 80
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 3 0
    mappingName
      background1
  exit
  box 
    set corners
      -2.9 2.9 -2.9 2.9  8. 20 -3.0 3.0 -3.0 3.0  8. 20
    lines
      137 137 120  * 90 90 80 
    boundary conditions
      0 0 0 0 0 1
    share
      0 0 0 0 0 2 
    mappingName
      background2
  exit
*
* build a DataPointMapping since this will be faster to evaluate and invert
*
     DataPointMapping
     build from a mapping
       rocket_volume
     mappingName
       rocket_volume-dp
  exit
exit
*
*			Now the overlap can be done.  The overlap will likley be extradorinarly
*                       computationally and memory expensive since the full grid has on the order
*                       of 200,000 grid points
*
generate an overlapping grid
    background1
    background2
    rocket_volume-dp
  done
  change parameters
  ghost points
    all
    2 2 2 2
  exit
* pause
*   display intermediate
  compute overlap
*   continue
*   continue
*   pause
exit
save an overlapping grid
rocket.hdf
rocket
exit
