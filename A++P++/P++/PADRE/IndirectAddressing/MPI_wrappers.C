#include "mpi.h"


#if 0
template<class  X> class type_tag {};

int MPI_Irecv_wrap(const type_tag<int>& ,void *buf, int count, int source, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Irecv( buf, count, MPI_INT, source, tag, comm, request);
}

int MPI_Irecv_wrap(const type_tag<float>& ,void *buf, int count, int source, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Irecv( buf, count, MPI_FLOAT, source, tag, comm, request);
}

int MPI_Irecv_wrap(const type_tag<double>& ,void *buf, int count, int source, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Irecv( buf, count, MPI_DOUBLE, source, tag, comm, request);
}

int MPI_Isend_wrap(const type_tag<int>& ,void *buf, int count, int dest, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Isend( buf, count, MPI_INT, dest, tag, comm, request);
}

int MPI_Isend_wrap(const type_tag<float>& ,void *buf, int count, int dest, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Isend( buf, count, MPI_FLOAT, dest, tag, comm, request);
}

int MPI_Isend_wrap(const type_tag<double>& ,void *buf, int count, int dest, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Isend( buf, count, MPI_DOUBLE, dest, tag, comm, request);
}

#else
template<class dataT>
int MPI_Irecv_wrap(dataT inDataT, void *buf, int count, int source, 
                   int tag, MPI_Comm comm, MPI_Request *request );


template<class dataT>
int MPI_Isend_wrap(dataT inDataT, void *buf, int count, int dest, 
                   int tag, MPI_Comm comm, MPI_Request *request );


//
// specific specializations for int, float, and double
//
template <> int MPI_Irecv_wrap(int inDataT, void *buf, int count, int source, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Irecv( buf, count, MPI_INT, source, tag,comm,request);
}

template <> int MPI_Irecv_wrap(float inDataT, void *buf, int count, int source, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Irecv( buf, count, MPI_FLOAT, source, tag,comm,request);
}

template <> int MPI_Irecv_wrap(double inDataT, void *buf, int count, int source, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Irecv( buf, count, MPI_DOUBLE, source, tag,comm,request);

}

template <> int MPI_Isend_wrap(int inDataT, void *buf, int count, int dest, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Isend( buf, count, MPI_INT, dest, tag,comm,request);
}

template <> int MPI_Isend_wrap(float inDataT, void *buf, int count, int dest, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Isend( buf, count, MPI_FLOAT, dest, tag,comm,request);
}

template <> int MPI_Isend_wrap(double inDataT, void *buf, int count, int dest, 
                                int tag, MPI_Comm comm, MPI_Request *request )
{
  return MPI_Isend( buf, count, MPI_DOUBLE, dest, tag,comm,request);

}

#endif
