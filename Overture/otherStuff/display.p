eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl

# perl program to create float double and int display functions


# These files contain the "generic" code
@filenames=("display.C");

@classTypes=("double","float","int","doubleSerial","floatSerial","intSerial");  # make these types of grid functions

foreach $file ( @filenames )
{

  open(FILE,"$file") || die "cannot open file $file!" ;
  $displayFile = $file;
  $displayFile =~ s/^d/D/;   # capitalize 
  open(FILEF,">float$displayFile");
  open(FILED,">double$displayFile");
  open(FILEI,">int$displayFile"); 
  open(FILEFS,">floatSerial$displayFile");
  open(FILEDS,">doubleSerial$displayFile");
  open(FILEIS,">intSerial$displayFile");
  %fileout=("double","FILED","float","FILEF","int","FILEI",
            "doubleSerial","FILEDS","floatSerial","FILEFS","intSerial","FILEIS");  # associative array

  while( <FILE> )
  {
    foreach $type ( @classTypes )
    {
      $line = $_;   
      $line =~ s/\bfloat\b/$type/g;
      $line =~ s/\bfloatArray\b/${type}Array/g;

      if( $type eq "int" || $type eq "double" || $type eq "float" )
      {
        $line =~ s/\bfloatSerialArray\b/${type}SerialArray/g;
      }
      else
      {
        $line =~ s/\bfloatSerialArray\b/${type}Array/g;
      }
      if( $type eq "int" || $type eq "intSerial" )
      { 
        $line =~ s/\bDEFAULT_FORMAT\b/dp.iFormat/g; 
        $line =~ s/\bDEFAULT_WIDTH\b/7/g; 
      }
      elsif( $type eq "float" || $type eq "floatSerial" )
      { 
        $line =~ s/\bDEFAULT_FORMAT\b/dp.fFormat/g; 
        $line =~ s/\bDEFAULT_WIDTH\b/12/g; 
      }
      elsif( $type eq "double" || $type eq "doubleSerial" )
      {
        $line =~ s/\bDEFAULT_FORMAT\b/dp.dFormat/g; 
        $line =~ s/\bDEFAULT_WIDTH\b/12/g; 
      }
      if( $type eq "intSerial" || $type eq "floatSerial" || $type eq "doubleSerial" )
      {
        $line =~ s/^\/\/ ifdef USE_PPP/\#ifdef USE_PPP/;
        $line =~ s/^\/\/ endif USE_PPP/\#endif/;
      }
      else
      {
        $line =~ s/\#undef PARALLEL_DISPLAY/\#define PARALLEL_DISPLAY/;
      }
      $file = $fileout{$type};
      # print "$type $file $line";
      print $file $line;
    }
   } 
  close(FILE);

}
