#!/usr/local/bin/perl

# perl program to create derivative routines for different derivatives , different 
#   orders of accuracy and different space dimensions
#
# create files xFDerivative.C, yFDerivative.C, zFDerivative.C, xxFDerivative.C, ...
#              etc. etc.
#
#  NOTE: laplace derivatives are done their own way because they are so complicated
#
# ===================================================================================
# ===============First do standard spatial derivatives===============================
# ===================================================================================

# Here is the file containing the generic derivative
$genericFileName = "gDerivative";

# make these types of derivatives:
# @filenames=("x","y","z","xy","xz","yz");
@filenames=("x","y","z","xx","xy","xz","yy","yz","zz"); # don't do : ,"laplace");

foreach $file ( @filenames )
{

  open(FILE,"${genericFileName}.C") || die "cannot open file $file!" ;
  open(FILEOUT,">${file}FDerivative.C");
  print "build ${file}FDerivative.C\n";

  while( <FILE> )
  {
    $line = $_;   
    $derivative = $file;
    $line =~ s/xxFDerivative/${derivative}FDerivative/g;
    $line =~ s/UXX/U\U${derivative}/g;
    $line =~ s/ULAPLACE/LAPLACIAN/g;

    print FILEOUT $line;
   } 
  close(FILE);

}

# # Here is the file containing the generic derivative
# $genericFileName = "ggDerivative";
# 
# #===================================================================================
# # make these types of derivatives (that require special code so the optimizing
# #    compiler doesn't choke)
# #===================================================================================
# @filenames=("xx","yy","zz");
# 
# foreach $file ( @filenames )
# {
# 
#   open(FILE,"${genericFileName}.C") || die "cannot open file $file!" ;
#   open(FILEOUT,">${file}FDerivative.C");
# 
#   while( <FILE> )
#   {
#     $line = $_;   
#     $derivative = $file;
#     $line =~ s/xxFDerivative/${derivative}FDerivative/g;
#     $line =~ s/UXX/U\U${derivative}/g;
#     $line =~ s/ULAPLACE/LAPLACIAN/g;
# 
#     print FILEOUT $line;
#    } 
#   close(FILE);
# 
# }


# ===================================================================================
# ===================Now do r,s,t derivatives ======================================
# ===================================================================================



# Here is the file containing the generic derivative
$genericFileName = "pDerivative";

# make these types of derivatives:
@filenames=("r","s","t","rr","rs","rt","ss","st","tt");

foreach $file ( @filenames )
{

  open(FILE,"${genericFileName}.C") || die "cannot open file $file!" ;
  open(FILEOUT,">${file}Derivative.C");

  print "build ${file}Derivative.C\n";
  while( <FILE> )
  {
    $line = $_;   
    $derivative = $file;
    $line =~ s/rrDerivative/${derivative}Derivative/g;
    $line =~ s/URR/U\U${derivative}/g;

    print FILEOUT $line;
   } 
  close(FILE);

}
