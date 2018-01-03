/*
   This is the generic procedure which gets included in the type specific files.
   void pgslib_constrained_send_rcv(SendS, RcvR)

This routines takes two inputs, a COMM_SEND and a COMM_RCV structure.
  COMM_SEND contains the data for the messages to be send.
  COMM_RCV  holds the received data.  Depending on the particular
            need, COMM_RCV may be pre-allocated, or parts of it
	    may be allocated on the fly.

A barrier at the start of the routine insures that all PEs are ready for this
communication step.

*/

/* $Id: constrained-send-rcv.c,v 1.1.1.1 1999/06/09 02:04:06 dquinlan Exp $ */



void PGSLIB_ROUTINE_NAME(pgslib_cnstd_send_rcv_)(SendS, RcvR)
COMM_SEND *SendS;
COMM_RCV  *RcvR;
{ 
  int nproc, myrank;
  int N_Send, N_Rcvd;
  int s, l, mpi_err;
  SEND_BUFFER *this_S_Buff;
  MPI_Request *send_request;
  MPI_Status  *send_status;
  char estr[512];

  MPI_Comm_size( MPI_COMM_WORLD, &nproc );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank);
	
#ifdef DEBUG_SEND_RCV
  pgslib_output_c("Calling initial barrier in CSR");
#endif

  mpi_err = MPI_Barrier( MPI_COMM_WORLD );

#ifdef DEBUG_SEND_RCV
  pgslib_output_c("Passed initial barrier in CSR");
  pgslib_flush_output_c();
#endif

  N_Send = SendS->N_Send;
  N_Rcvd = 0;

/* Do the sends, and do some attempts some receives */

  send_request = SendS->send_request;
  send_status  = SendS->send_status;
    
  for(s=0; s<N_Send; s++) {
    this_S_Buff = SendS->S_Buffers[s];
#ifdef DEBUG_SEND_RCV
    sprintf(estr,"CSR, Sending message %d: n_send=%d, PE=%d, TAG=%d, first word=%d", s,
	    this_S_Buff->n_send_words, this_S_Buff->dest_PE.PE, this_S_Buff->send_TAG,
	    (this_S_Buff->PGSLIB_TYPE_NAME(send_data_))[0]);
    pgslib_output_c(estr);
  pgslib_flush_output_c();
#endif
    MPI_Isend(&((this_S_Buff->PGSLIB_TYPE_NAME(send_data_))[0]),
	      this_S_Buff->n_send_words,
	      PGSLIB_MPI_DATA_TYPE,
	      this_S_Buff->dest_PE.PE,
	      this_S_Buff->send_TAG,
	      MPI_COMM_WORLD,
	      &(send_request[s]) );
#ifdef DEBUG_SEND_RCV
    sprintf(estr, "sent message %d, attempting receive", s);
    pgslib_output_c(estr);
  pgslib_flush_output_c();
#endif
    PGSLIB_ROUTINE_NAME(pgslib_attempt_receive_)(RcvR, myrank);
  }
  
/* All the sends are complete, so now count how many receives have
   been made? */
  N_Rcvd = RcvR->N_Rcvd;

  while(N_Rcvd < RcvR->N_Rcv_Buffers) {
    PGSLIB_ROUTINE_NAME(pgslib_attempt_receive_)(RcvR, myrank);
    N_Rcvd = RcvR->N_Rcvd;
  }

/* Finally, finish up the send requests.*/
  if (N_Send > 0) {
    MPI_Waitall(N_Send, &send_request[0], &send_status[0]);

  }

#ifdef DEBUG_SEND_RCV
  pgslib_output_c("Calling final barrier in CSR");
#endif

  mpi_err = MPI_Barrier( MPI_COMM_WORLD );

#ifdef DEBUG_SEND_RCV
  pgslib_output_c("Passed final barrier in CSR");
  pgslib_flush_output_c();
#endif

return;
} /* end of pgslib_constrained_send_receive */

