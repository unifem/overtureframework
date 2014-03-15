eval 'exec perl -S $0 ${1+"$@"}'
if 0;
# perl program to return the libraries needed by perl

system("perl -V > perl-V.temp");
open(PERLMV,"perl-V.temp") || die print "unable to open perl-V.temp\n";
$perllibs="";

while( <PERLMV> )
{
  if( /perllibs=/ )
  {
    $perllibs = $_;
    chop($perllibs);
    $perllibs =~ s/[ \t]*perllibs=//;
    $perllibs = "-lperl $perllibs";
    # printf(" perllibs=[$perllibs]\n");
  }
}
printf("$perllibs");

close(PERLMV);
unlink("perl-V.temp");

exit 0;
