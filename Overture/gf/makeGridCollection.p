#!/n/c3servet/henshaw/bin/perl

#
# perl program to create a new type of GridCollection Function
#
#   Usage:
#      makeGridCollection.p className
#
# where className is the prefix to the new type of MappedGridFunction.
#
# example:
#  if the new class is FaceCenteredMappedGridFunction then className = FaceCentered
#

$className          = $ARGV[0];

if( $className eq "" )
{
  printf("usage:\n makeGridCollection.p className \n");
  printf("example: className=FaceCentered if you want to make a \n");
  printf("FaceCenteredGridCollectionFunction from a FaceCenteredMappedGridFunction \n");
  exit;
}
else
{
  printf("Creating files {float,int,double}${className}GridCollectionFunction.{h,C} \n");
}   

# These files contain the "generic" code
@filenames=("gridCollectionFunction.h",
            "gridCollectionFunction.C");

@classTypes=("double","float","int");  # make these types of grid functions

# $className="FaceCentered";

foreach $file ( @filenames )
{

  open(FILE,"/n/c3servet/henshaw/res/gf/$file") || die "cannot open file $file!" ;
  $cgfile = $file;
  $cgfile =~ s/^g/G/;   # capitalize 
  $cgfile =~ s/^m/M/;   # capitalize 
  open(FILEF,">float${className}$cgfile");
  open(FILED,">double$className$cgfile");
  open(FILEI,">int$className$cgfile");
  %fileout=("double","FILED","float","FILEF","int","FILEI");  # associative array

  while( <FILE> )
  {
    foreach $type ( @classTypes )
    {
      $line = $_;   
      $line =~ s/\bdouble\b/$type/g;
      $line =~ s/\bdoubleArray\b/${type}Array/g;
      # convert FABS into fabs or abs:
      if( $type eq "int" )
      { $line =~ s/\bFABS\b/abs/g; 
        $line =~ s/\bif_int\b//g;
      }
      elsif( $type eq "float" )
      { $line =~ s/\bFABS\b/fabs/g; 
        $line =~ s/\bif_float\b//g;
      }
      elsif( $type eq "double" )
      { $line =~ s/\bFABS\b/fabs/g; 
        $line =~ s/\bif_double\b//g;
      }
      $line =~ s;\bif_int\b(.*)\\$;   \\;g;  # this fixes the case of macros
      $line =~ s;\bif_float\b(.*)\\$;   \\;g;  # this fixes the case of macros
      $line =~ s;\bif_double\b(.*)\\$;   \\;g;  # this fixes the case of macros

      $line =~ s/\bif_int\b/\/\//g;
      $line =~ s/\bif_float\b/\/\//g;
      $line =~ s/\bif_double\b/\/\//g;

      $line =~ s/\b(GenericGridFunction)\b/${type}\1/g;
      $line =~ s/(GENERIC_GRID_FUNCTION)/\U${type}_\1/g;         
      $line =~ s/\b(GenericGridCollectionFunction)\b/${type}\1/g;
      $line =~ s/(GENERIC_GRID_COLLECTION_FUNCTION)\b/\U${type}\1/g;
      $line =~ s/\b(MappedGridFunction)\b/${type}${className}\1/g;
      $line =~ s/\b(MappedGridFunctionRCData)\b/${type}\1/g;
      $line =~ s/(MAPPED_GRID_FUNCTION)/\U${type}_${className}_\1/g;
      $line =~ s/\b(GridCollectionFunction)\b/${type}${className}\1/g;
      $line =~ s/(GRID_COLLECTION_FUNCTION)/\U${type}_${className}_\1/g;

      $line =~ s/\b(ListOfGenericGridFunction)\b/ListOf\u${type}GenericGridFunction/g;
      $line =~ s/\b(ListOfMappedGridFunction)\b/ListOf\u${type}${className}MappedGridFunction/g;

      $line =~ s/\b(nullMappedGridFunction)\b/null\u${type}MappedGridFunction/g;
      $line =~ s/\b(nullGridCollectionFunction)\b/null\u${type}GridCollectionFunction/g;

# remove derivatives for a derived class

      $line =~ s/^DERIVATIVE.*//;

# cast for boundary conditions
      $line =~ s/(->[Aa]pplyBoundaryConditions\()/\1(${type}GridCollectionFunction \&)/;

# cast for interpolate
      $line =~ s/(interpolant.interpolate\()/\1(${type}GridCollectionFunction \&)/;

      $file = $fileout{$type};
      # print "$type $file $line";
      print $file $line;
    }
   } 
  close(FILE);

}
