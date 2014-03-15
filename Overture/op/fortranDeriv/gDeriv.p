eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl

# perl program to create derivative routines for different derivatives , different 
#   orders of accuracy and different space dimensions
#
# create files xFDeriv.f, yFDeriv.f, etc.
#              etc. etc.
#
#
# ===================================================================================
# ===============First do standard spatial derivatives===============================
# ===================================================================================

# Here is the file containing the generic derivative
$genericFileName = "gDeriv";

# make these types of derivatives:
@filenames=("x","y","z","xx","xy","xz","yy","yz","zz","laplacian");

foreach $file ( @filenames )
{

  open(FILE,"${genericFileName}.f") || die "cannot open file $file!" ;
  open(FILEOUT,">${file}FDeriv.f");
  print "build ${file}FDeriv.f\n";

  while( <FILE> )
  {
    $line = $_;   
    $derivative = $file;
    $line =~ s/xxDeriv/${derivative}FDeriv/g;  # subroutine name
    $line =~ s/UXX/U\U${derivative}/g;
    $line =~ s/ULAPLACIAN/LAPLACIAN/g;  # remove U added in previous change

    print FILEOUT $line;
   } 
  close(FILE);

}

