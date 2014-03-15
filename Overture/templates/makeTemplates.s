#! /bin/csh -f

# Shell script to de-templify Templates for Overture


# First create List's made from ListOfReferenceCountedObjects
set LORCO = "ListOfReferenceCountedObjects"

# for OverBlown:
dt.p $LORCO OB_MappedGridFunction         ListOfOB_MappedGridFunction          OB_MappedGridFunction.h

# for Overture
dt.p $LORCO floatGenericGridFunction      ListOfFloatGenericGridFunction       floatGenericGridFunction.h
dt.p $LORCO doubleGenericGridFunction     ListOfDoubleGenericGridFunction      doubleGenericGridFunction.h
dt.p $LORCO intGenericGridFunction        ListOfIntGenericGridFunction         intGenericGridFunction.h
dt.p $LORCO floatMappedGridFunction       ListOfFloatMappedGridFunction        floatMappedGridFunction.h
dt.p $LORCO doubleMappedGridFunction      ListOfDoubleMappedGridFunction       doubleMappedGridFunction.h
dt.p $LORCO intMappedGridFunction         ListOfIntMappedGridFunction          intMappedGridFunction.h

dt.p $LORCO floatCompositeGridFunction    ListOfFloatCompositeGridFunction     floatCompositeGridFunction.h
dt.p $LORCO doubleCompositeGridFunction   ListOfDoubleCompositeGridFunction    doubleCompositeGridFunction.h
dt.p $LORCO intCompositeGridFunction      ListOfIntCompositeGridFunction       intCompositeGridFunction.h

dt.p $LORCO floatGridCollectionFunction   ListOfFloatGridCollectionFunction    floatGridCollectionFunction.h
dt.p $LORCO doubleGridCollectionFunction  ListOfDoubleGridCollectionFunction   doubleGridCollectionFunction.h
dt.p $LORCO intGridCollectionFunction     ListOfIntGridCollectionFunction      intGridCollectionFunction.h

dt.p $LORCO intArray                      ListOfIntArray                       A++.h
dt.p $LORCO floatArray                    ListOfFloatArray                     A++.h
dt.p $LORCO doubleArray                   ListOfDoubleArray                    A++.h

dt.p $LORCO intSerialArray                ListOfIntSerialArray                 A++.h
dt.p $LORCO floatSerialArray              ListOfFloatSerialArray               A++.h
dt.p $LORCO doubleSerialArray             ListOfDoubleSerialArray              A++.h

dt.p $LORCO intArray                      ListOfIntDistributedArray            A++.h
dt.p $LORCO floatArray                    ListOfFloatDistributedArray          A++.h
dt.p $LORCO doubleArray                   ListOfDoubleDistributedArray         A++.h

dt.p $LORCO ListOfIntArray                ListOfListOfIntArray                 ListOfIntArray.h
dt.p $LORCO ListOfFloatArray              ListOfListOfFloatArray               ListOfFloatArray.h
dt.p $LORCO ListOfDoubleArray             ListOfListOfDoubleArray              ListOfDoubleArray.h
dt.p $LORCO GenericGrid                   ListOfGenericGrid                    GenericGrid.h
dt.p $LORCO MappedGrid                    ListOfMappedGrid                     MappedGrid.h
dt.p $LORCO GenericGridCollection         ListOfGenericGridCollection          GenericGridCollection.h
dt.p $LORCO GridCollection                ListOfGridCollection                 GridCollection.h
dt.p $LORCO CompositeGrid                 ListOfCompositeGrid                  CompositeGrid.h
dt.p $LORCO MappingRC                     ListOfMappingRC                      MappingRC.h

# now make lists from tlist

dt.p tlist BoundingBox                    ListOfBoundingBox                    BoundingBox.h
dt.p tlist MappedGridOperators            ListOfMappedGridOperators            MappedGridOperators.h
dt.p tlist GenericMappedGridOperators     ListOfGenericMappedGridOperators     GenericMappedGridOperators.h
dt.p tlist CompositeGridOperators         ListOfCompositeGridOperators         CompositeGridOperators.h
dt.p tlist GenericCompositeGridOperators  ListOfGenericCompositeGridOperators  GenericCompositeGridOperators.h
dt.p tlist GenericGridCollectionOperators ListOfGenericGridCollectionOperators GenericGridCollectionOperators.h

# for the data-base routines:
dt.p tlist ADataBaseRCData                ListOfADataBaseRCData                ADataBase.h
