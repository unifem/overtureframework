/* This routine moves data from  Supplement_Data to Duplicate_Data.
   gsTrace must be setup before this call.  That means that indexing
   and init_comm have been done.
   
The input is:
   nnodesThisPE
   Duplicate_Data  (already allocated, but not loaded)
   gsTrace (which knows how to do the communication)
   Supplement_Data 

The output is cmpl data at the elements
   Duplicate_Data

   
   
*/

/* $Id: scatter.c,v 1.1.1.1 1999/06/09 02:04:07 dquinlan Exp $ */

/* This is a generic routine which gets included in specific calls */
void PGSLIB_ROUTINE_NAME(pgslib_scatter_buf_)(Duplicate_Data, Supplement_Data, BlockSize_ptr, gsTrace_ptrptr)
     PGSLIB_DATA_TYPE *Supplement_Data;
     GS_TRACE_STRUCT **gsTrace_ptrptr;
     int              *BlockSize_ptr;
     PGSLIB_DATA_TYPE *Duplicate_Data;
{
  COMM_SEND *SendS;
  COMM_RCV  *RcvR;
  SEND_BUFFER *SBuff;
  RCV_BUFFER *RBuff;
  CmplOwnerPE *CmplOwnPE;
  CmplReceiverPE *ComplRcvrPE;
  GS_TRACE_STRUCT  *gsTrace_ptr;
  int i, cmpl, N_Rcvd, BlockSize;
  char estr[512];

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "\n");
    pgslib_output_c(estr);
    sprintf(estr, "Top of Scatter: BlockSize = %d", *BlockSize_ptr);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif

  gsTrace_ptr = *gsTrace_ptrptr;
  BlockSize   = *BlockSize_ptr;

/* Setup SendS for the send */
/* Only needs to be done on the first call with each trace. */
  if (gsTrace_ptr -> ScatterSBuf == NULL) {
    gsTrace_ptr -> ScatterSBuf = (COMM_SEND *) malloc(sizeof(COMM_SEND));
    pgslib_check_malloc_c((gsTrace_ptr -> ScatterSBuf), "malloc failed for ScatterSBuf in scatter");
    
    SendS = gsTrace_ptr -> ScatterSBuf;
    SendS->N_Send = gsTrace_ptr->N_CmplOwnerPEs;
  

/* S_Buffers is accessed through double indirection, so that
   we can change the order of the sends if desired. */

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "Set-up in Scatter: SendS->N_Send = %d", SendS->N_Send);
    pgslib_output_c(estr);
#endif
  
    if (SendS->N_Send > 0) {
      SendS->S_Buffers = (SEND_BUFFER **)malloc(sizeof(SEND_BUFFER *)*SendS->N_Send);
      pgslib_check_malloc_c(SendS->S_Buffers, "malloc failed in scatter");
      (SendS->S_Buffers)[0] = (SEND_BUFFER *)malloc(sizeof(SEND_BUFFER)*SendS->N_Send);
      pgslib_check_malloc_c((SendS->S_Buffers)[0], "malloc failed in scatter");

      SendS->send_request = (MPI_Request *)malloc(SendS->N_Send*sizeof(MPI_Request));
      pgslib_check_malloc_c(SendS->send_request, "malloc of SendS->send_request failed in scatter.");
    
      SendS->send_status = (MPI_Status *)malloc(SendS->N_Send*sizeof(MPI_Status));
      pgslib_check_malloc_c(SendS->send_status, "malloc of SendS->send_status failed in scatter.");
    }
    else {
      SendS->S_Buffers = NULL;
      SendS->send_request = NULL;
      SendS->send_status = NULL;
    }
/* For now, just walk through the send buffer in linear order. */
    for(i=0;i<SendS->N_Send;i++)
      *(SendS->S_Buffers + i) = (*SendS->S_Buffers) + i;

/* Move all data except data buffer pointers into SendS.  Data buffer changes on each call */

    for(i=0;i<SendS->N_Send;i++) {
      SBuff = *(SendS->S_Buffers + i);
      CmplOwnPE = gsTrace_ptr->CmplOwnerPEs + i;
      SBuff->dest_PE.PE = CmplOwnPE->PE.PE;
      SBuff->send_TAG = CmplOwnPE->Tag;
      SBuff->n_send_items = CmplOwnPE->ncmpls;
    } /* Finished initialized ScatterSBuf */
  }
  else {
    SendS = gsTrace_ptr -> ScatterSBuf;
  }

/* Move data into SendS->send_data_(type).  Data is not really moved, pointer
   is set to point to Supplement_Data[offset]*/

  for(i=0;i<SendS->N_Send;i++) {
    SBuff = *(SendS->S_Buffers + i);
    SBuff->n_send_words = (SBuff->n_send_items) * BlockSize;
    CmplOwnPE = gsTrace_ptr->CmplOwnerPEs + i;
    SBuff->PGSLIB_TYPE_NAME(send_data_) = &(Supplement_Data[(CmplOwnPE->Offset)*BlockSize]);

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "SendS[%d]->n_send_words = %d, dest = %d", i, SBuff->n_send_words, CmplOwnPE->PE.PE);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
  }

#ifdef DEBUG_GATH_SCATT
    pgslib_flush_output_c();
#endif

/* Set up receive buffer for incoming messages.*/
/* Only need to do this on first call with each trace. */
  if (gsTrace_ptr -> ScatterRBuf == NULL) {
    gsTrace_ptr -> ScatterRBuf = (COMM_RCV *)malloc(sizeof(COMM_RCV));
    pgslib_check_malloc_c((gsTrace_ptr -> ScatterRBuf), "malloc failed for ScatterRBuf in scatter");

    RcvR = gsTrace_ptr -> ScatterRBuf;

    /* We receive N_CmplReceiverPEs messages */
  
    RcvR->N_Rcv_Buffers = gsTrace_ptr->N_CmplReceiverPEs;
  
    if (RcvR->N_Rcv_Buffers > 0) {
      RcvR->R_Buffers = (RCV_BUFFER **)malloc(sizeof(RCV_BUFFER *)*RcvR->N_Rcv_Buffers);
      pgslib_check_malloc_c(RcvR->R_Buffers, "malloc failed for RcvR->R_Buffers in scatter");
      (RcvR->R_Buffers)[0] = (RCV_BUFFER *)malloc(sizeof(RCV_BUFFER)*RcvR->N_Rcv_Buffers);
      pgslib_check_malloc_c((RcvR->R_Buffers)[0], "malloc failed for (RcvR->R_Buffers)[0] in scatter");
    }
    else	
      RcvR->R_Buffers = NULL;

    /* set RcvR flags */
    RcvR->USE_TAG = 1;     /* Incoming messages go in a particular place */
    RcvR->KNOWN_SRC = 1;   /* We know which messages we expect */
    RcvR->ALLOW_RCV_BUFF_RESIZE = 0;  /* We pre-sized the buffer */
    RcvR->RCV_BUFF_PREALLOCATED = 1; /* But it doesn't matter, because of line above.*/
    RcvR->Element_Size = sizeof(PGSLIB_DATA_TYPE);

    /* Now prepare Rcv Buffs for incoming messages */
    /* We can setup all data except pointer to data buffer, since that changes with each call */
    for(i=0;i<RcvR->N_Rcv_Buffers;i++) {
      (RcvR->R_Buffers)[i] = (RcvR->R_Buffers)[0] + i;
      RBuff = (RcvR->R_Buffers)[i];
      ComplRcvrPE = (gsTrace_ptr->CmplReceiverPEs) + i;
      RBuff->src_PE.PE = ComplRcvrPE->PE.PE;
      RBuff->n_max_rcv_items = ComplRcvrPE->ncmpls;
      RBuff->n_rcvd_words = 0;
      RBuff->rcv_TAG = i;
    }
  } /* Finished initializing ScatterRBuf */
  else {
    RcvR = gsTrace_ptr -> ScatterRBuf;
    RcvR->Element_Size = sizeof(PGSLIB_DATA_TYPE);  /* May change on each call */
  }
    
  /* Reset this for each call */
  RcvR->N_Rcvd = 0;
#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "RcvR->N_Rcv_Buffers = %d", RcvR->N_Rcv_Buffers);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
/* Now point receive data buffer */
  for(i=0;i<RcvR->N_Rcv_Buffers;i++) {
    (RcvR->R_Buffers)[i] = (RcvR->R_Buffers)[0] + i;
    RBuff = (RcvR->R_Buffers)[i];
    RBuff->n_max_rcv_words = (RBuff->n_max_rcv_items) * BlockSize;
    ComplRcvrPE = (gsTrace_ptr->CmplReceiverPEs) + i;
    RBuff->PGSLIB_TYPE_NAME(rcv_data_) = &(Duplicate_Data[(ComplRcvrPE->Offset)*BlockSize]);	

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "RcvR[%d]->n_max_rcv_items = %d, src = %d", i, RBuff->n_max_rcv_items, RBuff->src_PE.PE);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
  }


/* Exchange data */
#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "Calling cnstd_send_rcv");
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
   PGSLIB_ROUTINE_NAME(pgslib_cnstd_send_rcv_)(SendS, RcvR);

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "Returned from  cnstd_send_rcv in scatter");
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
/* There is no unpacking to do, since the data went directly into Duplicate_Data */

/* Don't free ScatterSBuf nor ScatterRBuf since we will use them on subsequent calls */

  return;
}
