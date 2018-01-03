/* This routine moves data from Duplicate_Data to Supplement_Data.
   gsTrace must be setup before this call.  That means that indexing
   and init_comm have been done.
   
The input is:
   nnodesThisPE
   Duplicate_Data
   gsTrace (which knows how to do the communication)
   Supplement_Data (already allocated, but not loaded)

The output is cmpl data at the elements
   Supplement_Data

   
   
*/

/* This is the generic procedure, to be included by the specific calls*/

/* $Id: gather.c,v 1.1.1.1 1999/06/09 02:04:06 dquinlan Exp $ */

void PGSLIB_ROUTINE_NAME(pgslib_gather_buf_)(Supplement_Data, Duplicate_Data, BlockSize_ptr, gsTrace_ptrptr)
     PGSLIB_DATA_TYPE *Duplicate_Data;
     PGSLIB_DATA_TYPE *Supplement_Data;
     int              *BlockSize_ptr;
     GS_TRACE_STRUCT **gsTrace_ptrptr;
{
  COMM_SEND *SendS;
  COMM_RCV  *RcvR;
  SEND_BUFFER *SBuff;
  RCV_BUFFER *RBuff;
  CmplOwnerPE *CmplOwnPE;
  CmplReceiverPE *CmplRcvrPE;
  GS_TRACE_STRUCT  *gsTrace_ptr;
  int i, cmpl, N_Rcvd, BlockSize;
  char estr[512];

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "\n");
    pgslib_output_c(estr);
    sprintf(estr, "Top of Gather: BlockSize = %d", *BlockSize_ptr);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif

  gsTrace_ptr = *gsTrace_ptrptr;
  BlockSize   = *BlockSize_ptr;


/* Setup SendS for the send */
/* Only need to do this on first call with this particular trace*/
  if (gsTrace_ptr -> GatherSBuf == NULL) {
    gsTrace_ptr -> GatherSBuf = (COMM_SEND *) malloc(sizeof(COMM_SEND));
    pgslib_check_malloc_c((gsTrace_ptr -> GatherSBuf), "malloc failed for GatherSBuf in gather");
    
    SendS = gsTrace_ptr -> GatherSBuf;
    SendS->N_Send = gsTrace_ptr->N_CmplReceiverPEs;
  
/* S_Buffers is accessed through double indirection, so that
   we can change the order of the sends if desired. */

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "Set-up in Gather: SendS->N_Send = %d", SendS->N_Send);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
    if (SendS->N_Send > 0) {
      SendS->S_Buffers = (SEND_BUFFER **)malloc(sizeof(SEND_BUFFER *)*SendS->N_Send);
      pgslib_check_malloc_c(SendS->S_Buffers, "malloc of S_Buffers failed in gather");
      (SendS->S_Buffers)[0] = (SEND_BUFFER *)malloc(sizeof(SEND_BUFFER)*SendS->N_Send);
      pgslib_check_malloc_c((SendS->S_Buffers)[0], "malloc of S_Buffers[0] failed in gather");

      SendS->send_request = (MPI_Request *)malloc(SendS->N_Send*sizeof(MPI_Request));
      pgslib_check_malloc_c(SendS->send_request, "malloc of SendS->send_request failed in gather.");
    
      SendS->send_status = (MPI_Status *)malloc(SendS->N_Send*sizeof(MPI_Status));
      pgslib_check_malloc_c(SendS->send_status, "malloc of SendS->send_status failed in gather.");
    }
    else {
      SendS->S_Buffers = NULL;
      SendS->send_request = NULL;
      SendS->send_status = NULL;
    }

/* For now, just walk through the send buffer in linear order. */
    for(i=0;i<SendS->N_Send;i++)
      *(SendS->S_Buffers + i) = (*SendS->S_Buffers) + i;

/* Move all data except data buffer into send buffer.  data buffer changes on each call */

    for(i=0;i<SendS->N_Send;i++) {
      SBuff = *(SendS->S_Buffers + i);
      CmplRcvrPE = gsTrace_ptr->CmplReceiverPEs + i;
      SBuff->dest_PE.PE = CmplRcvrPE->PE.PE;
      SBuff->send_TAG = CmplRcvrPE->Tag;
      SBuff->n_send_items = CmplRcvrPE->ncmpls;
    }	
  }  /* Finished initializing send buffer*/
  else	{
    /* If Send buffer already setup, then just point to it*/
    SendS = gsTrace_ptr -> GatherSBuf;
  }

/* Move data into SendS->send_data_(type)  Data is not really moved, pointer
   is set to point to Duplicate_Data[offset]*/
    for(i=0;i<SendS->N_Send;i++) {
      SBuff = *(SendS->S_Buffers + i);
      SBuff->n_send_words = (SBuff->n_send_items) * BlockSize;
      CmplRcvrPE = gsTrace_ptr->CmplReceiverPEs + i;
      SBuff->PGSLIB_TYPE_NAME(send_data_) = &(Duplicate_Data[(CmplRcvrPE->Offset)*BlockSize]);

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "SendS[%d]->n_send_words = %d", i, SBuff->n_send_words);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif

    }	



/* Set up receive buffer for incoming messages.*/
/* Only need to do thi son first call with this particular trace */
  if (gsTrace_ptr -> GatherRBuf == NULL) {
    gsTrace_ptr -> GatherRBuf = (COMM_RCV *) malloc(sizeof(COMM_RCV));
    pgslib_check_malloc_c((gsTrace_ptr -> GatherRBuf), "malloc failed for GatherRBuf in gather");

    RcvR = gsTrace_ptr -> GatherRBuf;

    /* We receive N_CmplOwnerPEs messages */
    RcvR->N_Rcv_Buffers = gsTrace_ptr->N_CmplOwnerPEs;
  
    if (RcvR->N_Rcv_Buffers > 0) {
      RcvR->R_Buffers = (RCV_BUFFER **)malloc(sizeof(RCV_BUFFER *)*RcvR->N_Rcv_Buffers);
      pgslib_check_malloc_c(RcvR->R_Buffers, "malloc failed for RcvR->R_Buffers in gather");
      (RcvR->R_Buffers)[0] = (RCV_BUFFER *)malloc(sizeof(RCV_BUFFER)*RcvR->N_Rcv_Buffers);
      pgslib_check_malloc_c((RcvR->R_Buffers)[0], "malloc failed for (RcvR->R_Buffers)[0] in gather");
    }	
    else {
      RcvR->R_Buffers = NULL;
    }

    /* set RcvR flags */
    RcvR->USE_TAG = 1;     /* Incoming messages go in a particular place */
    RcvR->KNOWN_SRC = 1;   /* We know which messages we expect */
    RcvR->ALLOW_RCV_BUFF_RESIZE = 0;  /* We pre-sized the buffer */
    RcvR->RCV_BUFF_PREALLOCATED = 1; /* But it doesn't matter, because of line above.*/
    RcvR->Element_Size = sizeof(PGSLIB_DATA_TYPE);

    /* Now prepare Rcv Buffs for incoming messages */
    /* At setup we can do everything except point to the data, since data buffer changes with each call*/
    for(i=0;i<RcvR->N_Rcv_Buffers;i++) {
      (RcvR->R_Buffers)[i] = (RcvR->R_Buffers)[0] + i;
      RBuff = (RcvR->R_Buffers)[i];
      CmplOwnPE = (gsTrace_ptr->CmplOwnerPEs) + i;
      RBuff->src_PE.PE = CmplOwnPE->PE.PE;
      RBuff->n_max_rcv_items = CmplOwnPE->ncmpls;
      RBuff->n_rcvd_words = 0;
      RBuff->rcv_TAG = i;
    }
  }
  else {
    RcvR = gsTrace_ptr -> GatherRBuf;
  }

  /* Point recv buffer to the data buffer */
  RcvR->N_Rcvd = 0;
  for(i=0;i<RcvR->N_Rcv_Buffers;i++) {
    (RcvR->R_Buffers)[i] = (RcvR->R_Buffers)[0] + i;
    RBuff = (RcvR->R_Buffers)[i];
    RBuff->n_max_rcv_words = (RBuff->n_max_rcv_items) * BlockSize;
    CmplOwnPE = (gsTrace_ptr->CmplOwnerPEs) + i;
    RBuff->PGSLIB_TYPE_NAME(rcv_data_) = &(Supplement_Data[(CmplOwnPE->Offset)*BlockSize]);

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "RcvR[%d]->n_max_rcv_items = %d", i, RBuff->n_max_rcv_items);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif
  }


/* Exchange data */
   PGSLIB_ROUTINE_NAME(pgslib_cnstd_send_rcv_)(SendS, RcvR);

/* There is no unpacking to do since the data went directly into Supplement_Data */

#ifdef DEBUG_GATH_SCATT
    sprintf(estr, "RcvR->N_Rcvd = %d", RcvR->N_Rcvd);
    pgslib_output_c(estr);
    pgslib_flush_output_c();
#endif

/* Don't deallocate GatherSBuf or GatherRBuf, since we want to use them again next time
   a gather is done with this trace.*/

  return;
}
