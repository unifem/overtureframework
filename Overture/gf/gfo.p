#!/bin/perl


# Now make:
#  GenericCompositeGridOperators from GenericGridCollectionOperators 
#  CompositeGridOperators from GridCollectionOperators 
@filenames=("GenericGridCollectionOperators.h","GenericGridCollectionOperators.C", 
            "GridCollectionOperators.h","GridCollectionOperators.C");

foreach $file ( @filenames )
{
  $cgfile = $file;
  $cgfile =~ s/GridCollectionOperators/CompositeGridOperators/;
  open(FILE,"$file") || die "cannot open file $file!" ;
  open(FILEF,">$cgfile");

  while( <FILE> )
  {
      $line = $_;   
      $line =~ s/(GenericGridCollectionOperators)/GenericCompositeGridOperators/g;
      $line =~ s/(GridCollectionOperators)/CompositeGridOperators/g;
      $line =~ s/(GRID_COLLECTION_OPERATORS)/COMPOSITE_GRID_OPERATORS/g;
      $line =~ s/\b(realGridCollectionFunction)\b/realCompositeGridFunction/g;
      $line =~ s/\b(GridCollection)\b/CompositeGrid/g;
      $line =~ s/\b(GridCollectionData)\b/CompositeGridData/g;

      $file = FILEF;
      # print "$type $file $line";
      print $file $line;
   } 
  close(FILE);

}
