#! /bin/csh -f

# Shell script to de-templify Templates for Overture

# Make some ArraySimple, vectorSimple
dt2.p template=ArraySimple className=ArraySimpleReal \
      template=VectorSimple className=VectorSimpleReal \
      T==real  \
      file=ArraySimple.h,ArraySimpleReal.h \
      file=VectorSimple.h,VectorSimpleReal.h \
      WAS_NOT_DETEMPLIFIED==WAS_DETEMPLIFIED \
      __OV_ArraySimple_H__==__OV_ArraySimpleReal_H__  \
      __VECTOR_SIMPLE_H__==__VECTOR_SIMPLE_REAL_H__

dt2.p template=ArraySimple className=ArraySimpleInt \
      template=VectorSimple className=VectorSimpleInt \
      T==int  \
      file=ArraySimple.h,ArraySimpleInt.h \
      file=VectorSimple.h,VectorSimpleInt.h \
      WAS_NOT_DETEMPLIFIED==WAS_DETEMPLIFIED \
      __OV_ArraySimple_H__==__OV_ArraySimpleInt_H__  \
      __VECTOR_SIMPLE_H__==__VECTOR_SIMPLE_INT_H__

      
exit
