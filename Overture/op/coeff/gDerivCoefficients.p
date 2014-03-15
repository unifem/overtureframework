#!/usr/local/bin/perl

# perl program to create derivative COEFFICIENT routines for different derivatives , different 
#   orders of accuracy and different space dimensions
#
# create files xDerivative22.C, yDerivative22.C, zDerivative22.C, xxDerivative22.C, ...
#              xDerivative23.C, yDerivative23.C, zDerivative23.C, xxDerivative23.C, ...
#              etc. etc.

# ===================================================================================
# ===============First do standard spatial derivatives===============================
# ===================================================================================

# Here is the file containing the generic derivative
$genericFileName = "gDerivCoefficients";

# make these types of derivatives:
#@filenames=("x","y","z","xx","xy","xz","yy","yz","zz","laplace");
#
#foreach $file ( @filenames )
#{
#
#  open(FILE,"${genericFileName}.C") || die "cannot open file $file!" ;
#  open(FILEOUT,">${file}FDerivCoefficients.C");
#
#  while( <FILE> )
#  {
#    $line = $_;   
#    $derivative = $file;
#    $line =~ s/xxFDerivCoefficients/${derivative}FDerivCoefficients/g;
#    $line =~ s/UXX/U\U${derivative}/g;
#    $line =~ s/ULAPLACE/LAPLACIAN/g;
#
#    print FILEOUT $line;
#   } 
#  close(FILE);
#
#}

# ===================================================================================
# ===================Now do r,s,t derivatives ======================================
# ===================================================================================



# Here is the file containing the generic derivative
$genericFileName = "pDerivCoefficients";

# make these types of derivatives:
@filenames=("r","s","t","rr","rs","rt","ss","st","tt");

foreach $file ( @filenames )
{

  open(FILE,"${genericFileName}.C") || die "cannot open file $file!" ;
  open(FILEOUT,">${file}DerivCoefficients.C");

  while( <FILE> )
  {
    $line = $_;   
    $derivative = $file;
    $line =~ s/rrDerivCoefficients/${derivative}DerivCoefficients/g;
    $line =~ s/URR/U\U${derivative}/g;

    print FILEOUT $line;
   } 
  close(FILE);

}
