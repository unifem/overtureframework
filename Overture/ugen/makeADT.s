#! /bin/csh -f

# Shell script to de-templify Templates for Overture

# Make a 3D ADT that holds an int      
dt2.p template=GeometricADT className=GeometricADT3dInt \
      template=__GeometricADTtraversor className=GeometricADTTraversor3dInt \
      template=__GeometricADTiterator  className=GeometricADTIterator3dInt \
      template=GeomADTTuple            className=GeomADTTuple3dInt \
      template=NTreeNode className=NTreeNode2GeomADTTuple3dInt \
      dataT==int dimension==6 dimension2==12 \
      file=GeometricADT2.h,GeometricADT3dInt.h file=GeometricADT2.C,GeometricADT3dInt.C \
      file=GeomADTTuple2.h,GeomADTTuple3dInt.h \
      debug_GeometricADT==debug_GeometricADT3dInt \
      notProcessedWithDT==processedWithDT \
      GeometricADT2==GeometricADT3dInt \
      GeomADTTuple2==GeomADTTuple3dInt \
      __KKC_GEOMETRIC2_SEARCH__==__GEOMETRIC_ADT_3D_INT_H__ \
      GEOM_ADT_TUPLE_H==GEOM_ADT_TUPLE_3D_INT_H
      
# dt2.p template=GeometricADT className=GeometricADT3dVoidPointer \
#       template=__GeometricADTtraversor className=GeometricADTTraversor3dVoidPointer \
#       template=__GeometricADTiterator  className=GeometricADTIterator3dVoidPointer \
#       template=GeomADTTuple            className=GeomADTTuple3dVoidPointer \
#       template=NTreeNode className=NTreeNode2GeomADTTuple3dVoidPointer \
#       dataT==void\* dimension==6 dimension2==12 \
#       file=GeometricADT2.h,GeometricADT3dVoidPointer.h file=GeometricADT2.C,GeometricADT3dVoidPointer.C \
#       file=GeomADTTuple.h,GeomADTTuple3dVoidPointer.h \
#       debug_GeometricADT==debug_GeometricADT3dVoidPointer \
#       notProcessedWithDT==processedWithDT \
#       GeometricADT2==GeometricADT3dVoidPointer \
#       __KKC_GEOMETRIC2_SEARCH__==__GEOMETRIC_ADT_3D_VOIDPOINTER_H__ \
#       GEOM_ADT_TUPLE_H==GEOM_ADT_TUPLE_3D_VOIDPOINTER_H
      
dt2.p template=NTreeNode className=NTreeNode2GeomADTTuple3dInt \
      degree==2 Data==GeomADTTuple3dInt \
      file=NTreeNode.h,NTreeNode2GeomADTTuple3dInt.h \
      include=\"GeomADTTuple3dInt.h\" \
       __KKC_NTREENODE__==__NTREENODE_GEOMETRIC_ADT_3D_INT__
exit



	
