eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl

# perl program to create float double and int gridFunction's


# These files contain the "generic" code
@filenames=("genericGridFunction.gf.h",
            "genericGridCollectionFunction.gf.h","genericGridCollectionFunction.C",
            "mappedGridFunction.gf.h","mappedGridFunction.C",
            "gridCollectionFunction.gf.h","gridCollectionFunction.C");

@classTypes=("double","float","int");  # make these types of grid functions

foreach $file ( @filenames )
{

  open(FILE,"$file") || die "cannot open file $file!" ;
  $cgfile = $file;
  $cgfile =~ s/^g/G/;   # capitalize 
  $cgfile =~ s/^m/M/;   # capitalize 
  $cgfile =~ s/\.gf//;  # remove .gf suffix *wdh* 090410
  open(FILEF,">float$cgfile");
  open(FILED,">double$cgfile");
  open(FILEI,">int$cgfile");
  %fileout=("double","FILED","float","FILEF","int","FILEI");  # associative array

  while( <FILE> )
  {
    foreach $type ( @classTypes )
    {
      $line = $_;   
      $line =~ s/\bdouble\b/$type/g;
      $line =~ s/\bdoubleArray\b/${type}SerialArray/g;
      $line =~ s/\bdoubleSerialArray\b/${type}SerialArray/g;
      $line =~ s/\bdoubleDistributedArray\b/${type}DistributedArray/g;
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
      $line =~ s/\b(MappedGridFunction)\b/${type}\1/g;
      $line =~ s/\b(MappedGridFunctionRCData)\b/${type}\1/g;
      $line =~ s/(MAPPED_GRID_FUNCTION)/\U${type}_\1/g;
      $line =~ s/\b(GridCollectionFunction)\b/${type}\1/g;
      $line =~ s/(GRID_COLLECTION_FUNCTION)/\U${type}_\1/g;

      $line =~ s/(TYPE_COLLECTION_FUNCTION)/\U${type}_COLLECTION_FUNCTION/g;

      $line =~ s/\b(ListOfGenericGridFunction)\b/ListOf\u${type}GenericGridFunction/g;
      $line =~ s/\b(ListOfMappedGridFunction)\b/ListOf\u${type}MappedGridFunction/g;

      $line =~ s/\b(ListOfGridCollectionFunction)\b/ListOf\u${type}GridCollectionFunction/g;

      $line =~ s/\b(nullMappedGridFunction)\b/null\u${type}MappedGridFunction/g;
      $line =~ s/\b(nullGridCollectionFunction)\b/null\u${type}GridCollectionFunction/g;

      $file = $fileout{$type};
      # print "$type $file $line";
      print $file $line;
    }
   } 
  close(FILE);

}


# Now make the CompositeGridFunction from compositeGridFunction
@filenames=("compositeGridFunction.gf.h","compositeGridFunction.C");


foreach $file ( @filenames )
{
  $cgfile = $file;
  $cgfile =~ s/^c/C/;
  $cgfile =~ s/\.gf//;  # remove .gf suffix *wdh* 090410
  open(FILE,"$file") || die "cannot open file $file!" ;
  open(FILEF,">float$cgfile");
  open(FILED,">double$cgfile");
  open(FILEI,">int$cgfile");

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

      $line =~ s/\b(MappedGridFunction)\b/${type}\1/g;
      $line =~ s/\b(GridCollectionFunction)\b/${type}GridCollectionFunction/g;
      $line =~ s/\b(CompositeGridFunction)\b/${type}CompositeGridFunction/g;
      $line =~ s/\b(COMPOSITE_GRID_FUNCTION)\b/\U${type}CompositeGridFunction/g;

      $line =~ s/(TYPE_COLLECTION_FUNCTION)/\U${type}_COLLECTION_FUNCTION/g;

      $line =~ s/\b(ListOfGridCollectionFunction)\b/ListOf\u${type}GridCollectionFunction/g;
      $line =~ s/\b(ListOfCompositeGridFunction)\b/ListOf\u${type}CompositeGridFunction/g;

      $file = $fileout{$type};
      # print "$type $file $line";
      print $file $line;
    }
   } 
  close(FILE);

}



# 
# 
# # Now make:
# #  GenericCompositeGridOperators from GenericGridCollectionOperators 
# #  CompositeGridOperators from GridCollectionOperators 
# @filenames=("GenericGridCollectionOperators.h","GenericGridCollectionOperators.C", 
#             "GridCollectionOperators.h","GridCollectionOperators.C");
# 
# foreach $file ( @filenames )
# {
#   $cgfile = $file;
#   $cgfile =~ s/GridCollectionOperators/CompositeGridOperators/;
#   open(FILE,"$file") || die "cannot open file $file!" ;
#   open(FILEF,">$cgfile");
# 
#   while( <FILE> )
#   {
#       $line = $_;   
#       $line =~ s/(GenericGridCollectionOperators)/GenericCompositeGridOperators/g;
#       $line =~ s/(GridCollectionOperators)/CompositeGridOperators/g;
#       $line =~ s/(GRID_COLLECTION_OPERATORS)/COMPOSITE_GRID_OPERATORS/g;
#       $line =~ s/\b(realGridCollectionFunction)\b/realCompositeGridFunction/g;
#       $line =~ s/\b(GridCollection)\b/CompositeGrid/g;
#       $line =~ s/\b(GridCollectionData)\b/CompositeGridData/g;
# 
#       $line =~ s/\b(realGridCollectionFunctionReally)\b/realGridCollectionFunction/g;
#       $line =~ s/\b(GridCollectionReally)\b/GridCollection/g;
# 
#       $line =~ s/^\/\/ define COMPOSITE_GRID_OPERATORS/\#define COMPOSITE_GRID_OPERATORS/g;
# 
#       $file = FILEF;
#       # print "$type $file $line";
#       print $file $line;
#    } 
#   close(FILE);
# 
# }
# 
# 
# 
# 
