*
* Square hole in a square - poor man's version
*
*
* usage: ogen [noplot] squareHole -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot squareHole -factor=1 -order=2 -interp=i
*     ogen noplot squareHole -factor=2 -order=2 -interp=e
*     ogen noplot squareHole -factor=2 -order=4 -interp=e
*
$xas=-1.; $xbs=1.; $yas=-1.; $ybs=1.;      # dimensions of the outer square
$xah=-.5; $xbh=.5; $yah=-.5; $ybh=.5;  # dimensions of the hole
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=5; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$name = "squareHole" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
* 
create mappings
  rectangle
    set corners
     $xa=$xas; $xb=$xbs; $ya=$ybh; $yb=$ybs;
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    mappingName
      topSquare
    exit
*
  rectangle
    set corners
     $xa=$xas; $xb=$xbs; $ya=$yas; $yb=$yah;
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    mappingName
      bottomSquare
    exit
*
  rectangle
    set corners
     $xa=$xas; $xb=$xah; $ya=$yah; $yb=$ybh;
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    mappingName
      leftSquare
    exit
*
  rectangle
    set corners
     $xa=$xbh; $xb=$xbs; $ya=$yah; $yb=$ybh;
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    mappingName
      rightSquare
    exit
exit
*
generate an overlapping grid
  bottomSquare
  topSquare
  leftSquare
  rightSquare
  done
* 
  change parameters
    mixed boundary 
      bottomSquare 
        top    (side=1,axis=1) 
        leftSquare 
          determine mixed boundary points 
          done 
      bottomSquare 
        top    (side=1,axis=1) 
        rightSquare 
          determine mixed boundary points 
          done 
      topSquare
        bottom (side=0,axis=1)
        leftSquare
          determine mixed boundary points
          done
      topSquare
        bottom (side=0,axis=1)
        rightSquare
          determine mixed boundary points
          done
      leftSquare
        bottom (side=0,axis=1)
        bottomSquare
          determine mixed boundary points
          done
      leftSquare
        top    (side=1,axis=1)
        topSquare
          determine mixed boundary points
          done
      rightSquare
        bottom (side=0,axis=1)
        bottomSquare
          determine mixed boundary points
          done
      rightSquare
        top    (side=1,axis=1)
        topSquare
          determine mixed boundary points
          done
      done
    ghost points
      all
       $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
  exit
  compute overlap
exit
save a grid (compressed)
$name
squareHole
exit
