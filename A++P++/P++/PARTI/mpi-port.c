/*
   We have included the PARTI_MPI_numprocs function so that something in this file
   could be compiled and so we could avoid a warning about "empty translation unit"
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if 0
#ifdef USE_MPI
#define PARTI_ENABLE_MP_INTERFACE_MPI
/* error "MPI defined in mpi-port.c" */
#else
#error "MPI NOT defined in mpi-port.c"
#endif
#endif

#if 0
#include <Parti_config.h>

#if defined(PARTI_ENABLE_MP_INTERFACE_MPI)
#include <mpi.h>
#include "mpi-port.h"
#include "port.h"
#endif
#else
#include "port.h"
#endif

int
PARTI_MPI_numprocs ()
   {
  /* Use a static variable to call the MPI function only once and hold it locally. */
     static int Size = 0;

#if defined(PARTI_ENABLE_MP_INTERFACE_MPI)

     int flag;
     int return_status;

  /* D. Quinlan: Rewrote this function to avoid letting any MPI function from being called
     more than once (at the beginning, instead of at the end where it is an
     error since some internal data is removed in the cleanup process.  This error
     effects the use of MPI on the Blue Pacific machine only. */

     if (Size == 0)
        {
          return_status = MPI_Comm_compare( MPI_COMM_WORLD,
	                                    Global_PARTI_PADRE_Interface_PADRE_Comm_World,
                                            &flag);
          if (return_status != MPI_SUCCESS) 
             {
               printf("ERROR: in PARTI_MPI_numprocs()\n");
               printf("ERROR: tried to compare Global_PARTI_PADRE_Interface_PADRE_Comm_World\n");
               printf("ERROR: with MPI_COMM_WORLD ... didn't work!\n");
               printf("ERROR: return_status  ==  %d\n",return_status);
               printf("ERROR: exiting parallel machine with MPI_Abort()\n");
               MPI_Abort(MPI_COMM_WORLD,99);
             }
          if (flag != MPI_CONGRUENT)
             {
               printf("ERROR: in PARTI_MPI_numprocs()\n");
               printf("ERROR: Global_PARTI_PADRE_Interface_PADRE_Comm_World is supposed to\n");
               printf("ERROR: be identical to MPI_COMM_WORLD, but it isn't!\n");
               printf("ERROR: exiting parallel machine with MPI_Abort()\n");
               MPI_Abort(MPI_COMM_WORLD,99);
             }

       /* finally we get the number of processors! */
          return_status = MPI_Comm_size (Global_PARTI_PADRE_Interface_PADRE_Comm_World, &Size);

       /* This MPI_Comm_size is only called once we can do the error checking (the first time) */
          if (return_status != MPI_SUCCESS)
             {
               printf("ERROR: in PARTI_MPI_numprocs() after calling MPI_Comm_size(...)\n");
               printf("ERROR: return_status == %d\n",return_status);
               printf("ERROR: Size          == %d\n",Size);
               printf("ERROR: exiting parallel machine with MPI_Abort()\n");
               MPI_Abort(MPI_COMM_WORLD,99);
             }
          assert(Size > 0);
        }

/* endif for defined(PARTI_ENABLE_MP_INTERFACE_MPI) */
#endif
     return Size;
   }

#if defined(PARTI_ENABLE_MP_INTERFACE_MPI)
int
PARTI_MPI_myproc ()
{
  int Rank;
  int return_status;

#ifdef PARTI_INTERNALDEBUG
  int flag;
  return_status = MPI_Comm_compare(MPI_COMM_WORLD,
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
				   &flag);
  if (return_status != MPI_SUCCESS) 
    {
      printf("ERROR: in PARTI_MPI_myproc()\n");
      printf("ERROR: tried to compare Global_PARTI_PADRE_Interface_PADRE_Comm_World\n");
      printf("ERROR: with MPI_COMM_WORLD ... didn't work!\n");
      printf("ERROR: return_status  ==  %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if (flag != MPI_CONGRUENT)
    {
      printf("ERROR: in PARTI_MPI_myproc()\n");
      printf("ERROR: Global_PARTI_PADRE_Interface_PADRE_Comm_World is supposed to\n");
      printf("ERROR: be identical to MPI_COMM_WORLD, but it isn't!\n");
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif

  return_status = MPI_Comm_rank (Global_PARTI_PADRE_Interface_PADRE_Comm_World, &Rank);

#ifdef PARTI_INTERNALDEBUG
  if ( (return_status != MPI_SUCCESS) || ( Rank < 0 ) )
     {
      printf("ERROR: in PARTI_MPI_myproc() after calling MPI_Comm_rank(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: Rank          == %d\n",Rank);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif
  return Rank;
}

void
PARTI_killall()
{
/*    MPI_Finalize(); */
    MPI_Abort(MPI_COMM_WORLD,99);
}


int
PARTI_MPI_csend (int tag, char* buf, int len, int node, MPI_Request* pid)
{
  int return_status;



#ifdef PARTI_INTERNALDEBUG
  int flag;
  return_status = MPI_Comm_compare(MPI_COMM_WORLD,
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
				   &flag);
  if (return_status != MPI_SUCCESS) 
    {
      printf("ERROR: in PARTI_MPI_csend()\n");
      printf("ERROR: tried to compare Global_PARTI_PADRE_Interface_PADRE_Comm_World\n");
      printf("ERROR: with MPI_COMM_WORLD ... didn't work!\n");
      printf("ERROR: return_status  ==  %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if (flag != MPI_CONGRUENT)
    {
      printf("ERROR: in PARTI_MPI_csend()\n");
      printf("ERROR: Global_PARTI_PADRE_Interface_PADRE_Comm_World is supposed to\n");
      printf("ERROR: be identical to MPI_COMM_WORLD, but it isn't!\n");
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if ( (len < 0) || (node < 0) )
    {
      printf("ERROR: PARTI_MPI_csend(...) called with arguments that don't make sense!\n");
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif

/* part of P++ diagnostics */
  PARTI_numberOfMessagesSent++;

  return_status = MPI_Send((void*)buf, len, MPI_CHAR, node, tag, 
			   Global_PARTI_PADRE_Interface_PADRE_Comm_World);

#ifdef PARTI_INTERNALDEBUG
  if  (return_status != MPI_SUCCESS)
    {
      printf("ERROR: in PARTI_MPI_csend(...) after calling MPI_Send(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: tag           == %d\n",tag);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif
  return 0;
}

int
PARTI_MPI_crecv (int tag, char* buf, int len, int node, MPI_Request* pid)
{
  MPI_Status status;
  int return_status;

#ifdef PARTI_INTERNALDEBUG
  int flag;
  return_status = MPI_Comm_compare(MPI_COMM_WORLD,
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
				   &flag);
  if (return_status != MPI_SUCCESS) 
    {
      printf("ERROR: in PARTI_MPI_crecv()\n");
      printf("ERROR: tried to compare Global_PARTI_PADRE_Interface_PADRE_Comm_World\n");
      printf("ERROR: with MPI_COMM_WORLD ... didn't work!\n");
      printf("ERROR: return_status  ==  %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if (flag != MPI_CONGRUENT)
    {
      printf("ERROR: in PARTI_MPI_crecv()\n");
      printf("ERROR: Global_PARTI_PADRE_Interface_PADRE_Comm_World is supposed to\n");
      printf("ERROR: be identical to MPI_COMM_WORLD, but it isn't!\n");
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if ( (len < 0) || (node < 0) )
    {
      printf("ERROR: PARTI_MPI_crecv(...) called with arguments that don't make sense!\n");
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif

/* part of P++ diagnostics */
  PARTI_numberOfMessagesRecieved++;
  
  return_status = MPI_Recv((void*)buf, len, MPI_CHAR, node, tag, 
			   Global_PARTI_PADRE_Interface_PADRE_Comm_World, 
			   &status);

#ifdef PARTI_INTERNALDEBUG
  if (return_status != MPI_SUCCESS) 
    {
      printf("ERROR: in PARTI_MPI_crecv(...) after calling MPI_Recv(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: tag           == %d\n",tag);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if ( (status.MPI_SOURCE != node) || (status.MPI_TAG != tag) )
    {
      printf("ERROR: inconsistency in PARTI_MPI_crecv(...) after calling MPI_Recv(...)\n");
      printf("ERROR: status.MPI_SOURCE == %d, node == %d\n",status.MPI_SOURCE,node);
      printf("ERROR: status.MPI_TAG    == %d,  tag == %d\n",status.MPI_TAG,tag);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
      
#endif
  return 0;
}

int
PARTI_MPI_isend (int tag, char* buf, int len, int node, MPI_Request* pid)
{
  int return_status;

#ifdef PARTI_INTERNALDEBUG
  int flag;
  return_status = MPI_Comm_compare(MPI_COMM_WORLD,
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
				   &flag);
  if (return_status != MPI_SUCCESS) 
    {
      printf("ERROR: in PARTI_MPI_isend()\n");
      printf("ERROR: tried to compare Global_PARTI_PADRE_Interface_PADRE_Comm_World\n");
      printf("ERROR: with MPI_COMM_WORLD ... didn't work!\n");
      printf("ERROR: return_status  ==  %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if (flag != MPI_CONGRUENT)
    {
      printf("ERROR: in PARTI_MPI_isend()\n");
      printf("ERROR: Global_PARTI_PADRE_Interface_PADRE_Comm_World is supposed to\n");
      printf("ERROR: be identical to MPI_COMM_WORLD, but it isn't!\n");
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if ( (len < 0) || (node < 0) )
    {
      printf("ERROR: PARTI_MPI_isend(...) called with arguments that don't make sense!\n");
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif

/*  printf ("Calling MPI_Isend() in PARTI on processor %d \n",PARTI_myproc()); */

/* part of P++ diagnostics */
  PARTI_numberOfMessagesSent++;

  assert (pid != NULL);
  assert (buf != NULL);
  return_status = MPI_Isend((void*)buf, len, MPI_CHAR, node, tag, 
			    Global_PARTI_PADRE_Interface_PADRE_Comm_World,
			    pid);

#ifdef PARTI_INTERNALDEBUG
  if (return_status != MPI_SUCCESS)
    {
      printf("ERROR: in PARTI_MPI_isend(...) after calling MPI_Isend(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: tag           == %d\n",tag);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif
  return 0;
}

int
PARTI_MPI_irecv (int tag, char* buf, int len, int node, MPI_Request* pid)
{
  int return_status;

#ifdef PARTI_INTERNALDEBUG
  int flag;
  return_status = MPI_Comm_compare(MPI_COMM_WORLD,
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
				   &flag);
  if (return_status != MPI_SUCCESS) 
    {
      printf("ERROR: in PARTI_MPI_irecv()\n");
      printf("ERROR: tried to compare Global_PARTI_PADRE_Interface_PADRE_Comm_World\n");
      printf("ERROR: with MPI_COMM_WORLD ... didn't work!\n");
      printf("ERROR: return_status  ==  %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if (flag != MPI_CONGRUENT)
    {
      printf("ERROR: in PARTI_MPI_irecv()\n");
      printf("ERROR: Global_PARTI_PADRE_Interface_PADRE_Comm_World is supposed to\n");
      printf("ERROR: be identical to MPI_COMM_WORLD, but it isn't!\n");
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
  if ( (len < 0) || (node < 0) )
    {
      printf("ERROR: PARTI_MPI_irecv(...) called with arguments that don't make sense!\n");
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif

/*  printf ("Calling MPI_Irecv() in PARTI on processor %d \n",PARTI_myproc()); */

/* part of P++ diagnostics */
  PARTI_numberOfMessagesRecieved++;
  
  assert (pid != NULL);
  assert (buf != NULL);
  return_status = MPI_Irecv((void*)buf, len, MPI_CHAR, node, tag, 
			    Global_PARTI_PADRE_Interface_PADRE_Comm_World, 
			    pid);

#ifdef PARTI_INTERNALDEBUG
  if (return_status != MPI_SUCCESS)
    {
      printf("ERROR: in PARTI_MPI_irecv(...) after calling MPI_Irecv(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: len           == %d\n",len);
      printf("ERROR: tag           == %d\n",tag);
      printf("ERROR: node          == %d\n",node);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif
  return 0;
}

int
PARTI_MPI_msgwait ( MPI_Request* gid )
{
  MPI_Status status;
  int return_status;

  return_status = MPI_Wait (gid, &status);
#ifdef PARTI_INTERNALDEBUG
  if (return_status != MPI_SUCCESS)
    {
      printf("ERROR: in PARTI_MPI_msgwait(...) after calling MPI_Wait(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
/* 
   here we should check for status.MPI_TAG and status.MPI_SOURCE, but
   there doesn't seem to be a way to figure out whtat these are supposed 
   to be, if we only know gid
*/
#endif
  return 0;
}

int
PARTI_MPI_msgdone( MPI_Request* gid )
{
  MPI_Status status;
  int flag;
  int return_status; 

  return_status = MPI_Test (gid, &flag, &status);
#ifdef PARTI_INTERNALDEBUG
  if (return_status != MPI_SUCCESS)
    {
      printf("ERROR: in PARTI_MPI_msgdone(...) after calling MPI_Test(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
/*
   here we should check for status.MPI_TAG and status.MPI_SOURCE, but
   there doesn't seem to be a way to figure out whtat these are supposed 
   to be, if we only know gid
*/
#endif
  return flag;
}

int
PARTI_MPI_gsync ()
{
   int return_status = MPI_Barrier(Global_PARTI_PADRE_Interface_PADRE_Comm_World);
#ifdef PARTI_INTERNALDEBUG
  if (return_status != MPI_SUCCESS)
    {
      printf("ERROR: in PARTI_MPI_gsync(...) after calling MPI_Barrier(...)\n");
      printf("ERROR: return_status == %d\n",return_status);
      printf("ERROR: exiting parallel machine with MPI_Abort()\n");
      MPI_Abort(MPI_COMM_WORLD,99);
    }
#endif
}

#endif


