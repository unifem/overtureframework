#ifndef hdf_stuff_h
#define hdf_stuff_h

#ifdef HDF_DUMMY
  #include <stdio.h>
  typedef int int32;
#else
  #include <hdf.h>
  #include <mfhdf.h>
#endif

#include "c_array.h"
/* kkc causes compiler errors #include "stupid_compiler.h" */

#ifndef TRUE
#define TRUE                       1
#endif

/* define the HDF data type corresponding to real */
#ifdef SINGLE
#define DFNT_REAL DFNT_FLOAT32
#else
#define DFNT_REAL DFNT_FLOAT64
#endif

/* prototypes */

#ifdef HDF_DUMMY
void
Vdetach(int32 directory);
#endif

#ifndef NO_REAL
int
hput_real( real x, char *name, int vgroup_id );
int
hget_real(real *x, char *name, int vgroup_id );
int
hput_real_array_1d( real_array_1d * x, char *name, int vgroup_id );
int
hput_real_array_2d( real_array_2d * x, char *name, int vgroup_id );
int
hput_real_array_3d( real_array_3d * x, char *name, int vgroup_id );
int
hput_real_array_4d( real_array_4d * x, char *name, int vgroup_id );
real_array_1d * 
hget_real_array_1d( char * name, int vgroup_id );
real_array_2d * 
hget_real_array_2d( char * name, int vgroup_id );
real_array_3d * 
hget_real_array_3d( char * name, int vgroup_id );
real_array_4d * 
hget_real_array_4d( char * name, int vgroup_id );
#endif

int32
open_hdf_file(char * name, char access);
int32
get_file_id(void);
int32
get_sd_id(void);
void
close_hdf_file(int32 root);
int32
create_dir(char * name, char * class_name, int vgroup_id);
int32
locate_dir(char * name, int vgroup_id);
int
hput_int( int x, char *name, int vgroup_id );
int
hput_float( float x, char *name, int vgroup_id );
int
hput_double( double x, char *name, int vgroup_id );
int
hget_int(int *x, char *name, int vgroup_id );
int
hget_float(float *x, char *name, int vgroup_id );
int
hget_double(double *x, char *name, int vgroup_id );
int
hput_int_array_1d( int_array_1d * x, char *name, int vgroup_id );
int
hput_int_array_2d( int_array_2d * x, char *name, int vgroup_id );
int
hput_int_array_3d( int_array_3d * x, char *name, int vgroup_id );
int
hput_int_array_4d( int_array_4d * x, char *name, int vgroup_id );
int
hput_float_array_1d( float_array_1d * x, char *name, int vgroup_id );
int
hput_float_array_2d( float_array_2d * x, char *name, int vgroup_id );
int
hput_float_array_3d( float_array_3d * x, char *name, int vgroup_id );
int
hput_float_array_4d( float_array_4d * x, char *name, int vgroup_id );
int
hput_double_array_1d( double_array_1d * x, char *name, int vgroup_id );
int
hput_double_array_2d( double_array_2d * x, char *name, int vgroup_id );
int
hput_double_array_3d( double_array_3d * x, char *name, int vgroup_id );
int
hput_double_array_4d( double_array_4d * x, char *name, int vgroup_id );
int_array_1d * 
hget_int_array_1d( char * name, int vgroup_id );
int_array_2d * 
hget_int_array_2d( char * name, int vgroup_id );
int_array_3d * 
hget_int_array_3d( char * name, int vgroup_id );
int_array_4d * 
hget_int_array_4d( char * name, int vgroup_id );
float_array_1d * 
hget_float_array_1d( char * name, int vgroup_id );
float_array_2d * 
hget_float_array_2d( char * name, int vgroup_id );
float_array_3d * 
hget_float_array_3d( char * name, int vgroup_id );
float_array_4d * 
hget_float_array_4d( char * name, int vgroup_id );
double_array_1d * 
hget_double_array_1d( char * name, int vgroup_id );
double_array_2d * 
hget_double_array_2d( char * name, int vgroup_id );
double_array_3d * 
hget_double_array_3d( char * name, int vgroup_id );
double_array_4d * 
hget_double_array_4d( char * name, int vgroup_id );
int   
hput_string( char * x, char * name, int vgroup_id );
char *   
hget_string(char * name, int vgroup_id );


/* end prototypes */

#endif
