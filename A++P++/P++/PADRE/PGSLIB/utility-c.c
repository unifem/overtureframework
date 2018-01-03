/* These subroutines support the utilities in PGSLib.
   They are called by routines in the PGLSib_Utility_Module.
*/

/* $Id: utility-c.c,v 1.3 2000/09/26 23:32:04 dquinlan Exp $ */

#include <stdlib.h>
#include <stdio.h>
#include "pgslib-include-c.h"

#define USE_FILE_OUTPUT

#include "utility-c.h"

static int pgslib_fperr_opened=0;
static FILE *fperr;
static char ferrname[2048] = "";
static int pgslib_fpout_opened=0;
static FILE *fpout;
static char foutname[2048] = "";

static int been_initialized = FALSE;
static int pgslib_doesnt_init_mpi;

/* Quinlan (9/24/2000) Added here to avoid redeclaration of pgslib_state (prevents warning on SGI at link time) */
struct pgslib_state_struct pgslib_state;


void pgslib_initialize_c(nPE, thisPE, IO_ROOT_PE, File_Prefix)
     int *nPE, *thisPE, *IO_ROOT_PE;
     char *File_Prefix;
{ int ierror;
  char ProgName[] = "PGSLib_MPI";
  char ErrorString[] = "ERROR in initialize.";
  int zero = 0;
  char *blank = "";
  char **blankptr = &blank;

  if (! been_initialized)
    {	been_initialized = TRUE;


	MPI_Initialized(&pgslib_doesnt_init_mpi);
	if (! pgslib_doesnt_init_mpi) {
	  /* Need to call the fortran version of init, since main is F90 code. */
	  /*	  pgslib_mpi_init(&ierror);		*/
	  ierror = MPI_Init(&zero,&blankptr);

	  if ( ierror != MPI_SUCCESS ) 
	    fprintf(stderr, "ERROR: Could not initialize MPI");
	}	     
	MPI_Comm_size( MPI_COMM_WORLD, nPE);
	MPI_Comm_rank( MPI_COMM_WORLD, thisPE);

	/* Establish file names */
	sprintf(foutname, "%s-out.%04d", File_Prefix, *thisPE);
	sprintf(ferrname, "%s-err.%04d", File_Prefix, *thisPE);

	/* Use user request for IO_ROOT_PE, unless it is out of range */
	/* User request is 0 based */
	PGSLib_IO_ROOT_PE = *IO_ROOT_PE;
	if( *IO_ROOT_PE < 0) 
	  PGSLib_IO_ROOT_PE = DEFAULT_IO_ROOT_PE;
	if ( *IO_ROOT_PE > *nPE) 
	  PGSLib_IO_ROOT_PE = DEFAULT_IO_ROOT_PE;

	pgslib_state.initialized = TRUE;

	pgslib_state.nPE = *nPE;
	pgslib_state.thisPE = *thisPE;
	pgslib_state.io_pe = PGSLib_IO_ROOT_PE;
      }
  *nPE = pgslib_state.nPE;
  *thisPE = pgslib_state.thisPE;
  *IO_ROOT_PE = pgslib_state.io_pe;
}

void pgslib_finalize_c()
{ int ierror;
  pgslib_close_output_c();	
  been_initialized = FALSE;
  if (! pgslib_doesnt_init_mpi)
    MPI_Finalize();	
}


void pgslib_error_c(ErrorString)
     char *ErrorString;
{ int myrank;

  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

#ifdef USE_FILE_OUTPUT
  if( !pgslib_fperr_opened) {
    fperr = fopen(ferrname, "w");
    pgslib_fperr_opened = TRUE;
  }
  fprintf(fperr, " %3d: ERROR %s\n", myrank, ErrorString);
#else
  fprintf(stderr, " %3d: ERROR %s\n", myrank, ErrorString);
#endif
}


void pgslib_fatal_error_c(ErrorString)
     char *ErrorString;
{ int myrank;
  pgslib_error_c(ErrorString);
  pgslib_abort_c();
}

void pgslib_abort_c()
{
  pgslib_close_output_c();
  MPI_Abort( MPI_COMM_WORLD, 1);
  exit;
}

void pgslib_output_c(Message)
     char *Message;
{ int myrank;

  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

#ifdef USE_FILE_OUTPUT
  if( !pgslib_fpout_opened) {
    fpout = fopen(foutname, "w");
    if( fpout == NULL)
      fprintf(stderr, "fopen failed in pgslib_output");
    pgslib_fpout_opened = TRUE;
  }
  fprintf(fpout, " %3d: %s\n", myrank, Message);
#else
  fprintf(stdout, " %3d: %s\n", myrank, Message);
#endif
}

void pgslib_flush_output_c()
{
#ifdef USE_FILE_OUTPUT
  if ( fpout != NULL)
  {
    fflush(fpout);
  }
  if ( fperr != NULL)
  {
    fflush(fperr);
  }
#else
  fflush(stdout);
  fflush(stderr);
#endif
}

void pgslib_close_output_c()
{
#ifdef USE_FILE_OUTPUT
  if ( fpout != NULL)
  {
    fclose(fpout);
    pgslib_fpout_opened = FALSE;
  }
  if ( fperr != NULL)
  {
    fclose(fperr);
    pgslib_fperr_opened = FALSE;
  }
#endif
}

void pgslib_check_malloc_c(Pointer, ErrorString)
     void *Pointer;
     char *ErrorString;
{
  if(Pointer == NULL) {
    pgslib_error_c(ErrorString);
    exit;
  }
}

void pgslib_barrier_c()
{
  MPI_Barrier( MPI_COMM_WORLD );
}
