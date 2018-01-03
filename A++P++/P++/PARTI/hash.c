#include <stdio.h>
#include "List.h"
#include "bsparti.h"
#include "port.h"
#include "utils.h"
#include "hash.h"

#define DA_TABLE_SIZE 256
static int da_done_init = 0;

static DARRAY *da_table[DA_TABLE_SIZE];
static int da_last = 0;  /* points to first open position in da_table */

#define SCHED_TABLE_SIZE 256

static List subarray_table[SCHED_TABLE_SIZE]; /* for subarray schedules */
static int sub_done_init = 0;

static List exch_table[SCHED_TABLE_SIZE];     /* for exchange schedules */
static int exch_done_init = 0;

typedef struct { SCHED* subarray_sched;
		 DARRAY *srcDA, *destDA;
		 int numDims;
		 int srcDims[MAX_DIM], sLos[MAX_DIM],
		     sHis[MAX_DIM], sStrides[MAX_DIM];
		 int destDims[MAX_DIM], dLos[MAX_DIM],
		     dHis[MAX_DIM], dStrides[MAX_DIM];
                 int referenceCount;  /* Added by Dan Quinlan */
	       } sub_cell;

typedef struct { SCHED* exch_sched;
		 DARRAY *dArrayPtr;
		 int ndims, fillVec[MAX_DIM];
                 int referenceCount;  /* Added by Dan Quinlan */
	       } exch_cell;

int da_hash(x)
int x;
{
  char* p = (char*) &x;
  
  return (p[0] ^ p[1] ^ p[2] ^ p[3]) & 0xff;
}


void init_da_table()
{
  int i;
  
  if (da_done_init) return;
  
  for (i = 0; i < DA_TABLE_SIZE; i++)
    da_table[i] = NULL;
  da_done_init = 1;
}

void destroy_da_table()
{
  return;
}

int insert_da_table(da)
DARRAY* da;
{
  if (!da_done_init)
    init_da_table();
  
  if (da_last >= DA_TABLE_SIZE) {
    fatal_error("No more room in darray table");
  }
  
  da_table[da_last] = da;
  da->referenceCount++;
  
  return da_last++;
}

DARRAY* lookupda(x)
int x;
{
  if (!da_done_init)
    init_da_table();
  
  if (x >= da_last || x < 0) {
    fatal_error("Tried to index a non-existent descriptor in the darray table");
  }
  return(da_table[x]);
}

void init_exch_table()
{
  int i;
  
  for (i = 0; i < SCHED_TABLE_SIZE; i++)
    exch_table[i] = create_List();
  exch_done_init = 1;
}

void destroy_exch_table()
{
  int i;
  exch_cell* exch;
  
  if (exch_done_init) {
    for (i = 0; i < SCHED_TABLE_SIZE; i++) {
      FOREACH(exch, exch_table[i]) {
	free_sched_space(exch->exch_sched);
     /* printf ("Delete exch_table[%d] list entry = %p \n",i,exch); */

        assert (exch->referenceCount >= 0);
        if (exch->referenceCount-- == 0)
           {
          /* printf ("Delete exch_table[%d] list entry = %p FREEING! \n",i,exch); 
             printf ("exch->referenceCount = %d \n",exch->referenceCount);
             printf ("exch->exch_sched     = %p \n",exch->exch_sched);
             printf ("exch->dArrayPtr      = %p \n",exch->dArrayPtr);
           */
             if (exch->exch_sched != NULL)
                {
               /* printf ("exch->exch_sched->referenceCount = %d \n",exch->exch_sched->referenceCount); */
                  delete_SCHED(exch->exch_sched);
               /* DISPOSE(exch->exch_sched); */
                  exch->exch_sched = NULL;
                }
          /* This data is cleaned up in other ways (it is a part of a seperate list)
             if (exch->dArrayPtr != NULL)
                {
                  printf ("exch->dArrayPtr->referenceCount = %d \n",exch->dArrayPtr->referenceCount);
                  DISPOSE(exch->dArrayPtr);
                  exch->dArrayPtr = NULL;
                }
           */
	     DISPOSE(exch);
           }
          else
           {
          /* printf ("Delete exch_table[%d] list entry = %p not freed! \n",i,exch); */
           }
      }
      destroy_List(exch_table[i]);
    }
  }
  exch_done_init = 0;
}

int exch_hash(dArrayPtr, ndims, fillVec)
DARRAY *dArrayPtr;
int ndims, *fillVec;
{
  int hash, i;
  char* p = (char*) dArrayPtr;
  
  hash = ((p[0] ^ p[1] ^ p[2] ^ p[3]) + ndims);
  for (i = 0; i < ndims; i++) {
    hash += fillVec[i];
  }
  hash &= 0xff;

/*
printf("Node %d: generating exchange hash value %d\n", PARTI_myproc(), hash);
*/
  return hash;
}

void insert_exch_table(exch, dArrayPtr, ndims, fillVec)
SCHED* exch;
DARRAY *dArrayPtr;
int ndims, *fillVec;
{
  int i, j;
  exch_cell* exchc;
  exch_cell* x;

/* printf ("Inside of insert_exch_table: Inserting exch SCEHED into hash table! (exch=%p) \n",exch); */

  if (!exch_done_init)
    init_exch_table();
  i = exch_hash(dArrayPtr, ndims, fillVec);
  exch -> hash = i;
  
  exchc = NEW(exch_cell);
  exchc->referenceCount = 0;     /* Code added by Dan Quinlan */
  exchc->exch_sched = exch;
  exch->referenceCount++;     /* Code added by Dan Quinlan */
  exchc->dArrayPtr = dArrayPtr;
  exchc->ndims = ndims;
  for (j = 0; j < ndims; j++) {
    exchc->fillVec[j] = fillVec[j];
  }
  insert_List(exchc, exch_table[i]);
}

void delete_exch_table(sched)
SCHED* sched;
{
  int i = sched -> hash;
  exch_cell* exchc;

  FOREACH(exchc, exch_table[i]){
    if (exchc -> exch_sched == sched) {
      (void) remove_List(exchc, exch_table[i]);
      break;
    }
  }
}

SCHED* lookup_exch(dArrayPtr, ndims, fillVec)
DARRAY *dArrayPtr;
int ndims, *fillVec;
{
  int i, j, found;
  exch_cell* v;

  if (!exch_done_init)
    init_exch_table();
  i = exch_hash(dArrayPtr, ndims, fillVec);

  FOREACH(v, exch_table[i]) {
    found = 1;
    if (v->dArrayPtr != dArrayPtr || v->ndims != ndims)
      found = 0;
    for (j = 0; j < ndims; j++) {
      if (v->fillVec[j] != fillVec[j])
	found = 0;
    }

    if (found) return v->exch_sched;
  }

  return NULL;
}

void init_sub_table()
{
  int i;

  for (i = 0; i < SCHED_TABLE_SIZE; i++)
    subarray_table[i] = create_List();
  sub_done_init = 1;
}

void destroy_sub_table()
{
  int i;
  sub_cell* sub;

  if (sub_done_init) {
    for (i = 0; i < SCHED_TABLE_SIZE; i++) {
     /* printf ("In destroy_sub_table subarray_table[%d] = %p \n",i,subarray_table[i]); */
      FOREACH(sub, subarray_table[i]) {
     /* if (sub->subarray_sched == NULL)
             printf ("sub->subarray_sched == NULL \n");
      */
        free_sched_space(sub->subarray_sched);    /* Commented out by Dan Quinlan */
     /* printf ("Delete subarray_table[%d] list entry = %p \n",i,sub); */
        sub->subarray_sched = NULL;               /* Code added by Dan Quinlan */

        assert (sub->referenceCount >= 0);
        if (sub->referenceCount-- == 0)
           {
          /* printf ("Delete subarray_table[%d] list entry = %p FREEING! \n",i,sub); 
             printf ("sub->referenceCount = %d \n",sub->referenceCount);
             printf ("sub->subarray_sched = %p \n",sub->subarray_sched);
             printf ("sub->srcDA          = %p \n",sub->srcDA);
             printf ("sub->destDA         = %p \n",sub->destDA);
           */
             if (sub->subarray_sched != NULL)
                {
               /* printf ("sub->subarray_sched->referenceCount = %d \n",sub->subarray_sched->referenceCount); */
                  delete_SCHED(sub->subarray_sched);
               /* DISPOSE(sub->subarray_sched); */
                  sub->subarray_sched = NULL;
                }
          /* This data is cleaned up in other ways (it is a part of a seperate list)
             if (sub->dArrayPtr != NULL)
                {
                  printf ("sub->dArrayPtr->referenceCount = %d \n",sub->dArrayPtr->referenceCount);
                  DISPOSE(sub->dArrayPtr);
                  sub->dArrayPtr = NULL;
                }
           */
	     DISPOSE(sub);
           }
      }
      destroy_List(subarray_table[i]);
    }
  }
  sub_done_init = 0;
}

int sub_hash(srcDA, destDA, numDims,
	     srcDims, sLos, sHis, sStrides,
	     destDims, dLos, dHis, dStrides)
DARRAY *srcDA, *destDA;
int numDims;
int *srcDims, *sLos, *sHis, *sStrides;
int *destDims, *dLos, *dHis, *dStrides;
{
  int hash, i;
  char* p1 = (char*) srcDA;
  char* p2 = (char*) destDA;
  
  hash = (p1[0] ^ p1[1] ^ p1[2] ^ p1[3] ^ p2[0] ^ p2[1] ^ p2[2] ^ p2[3] );
  hash += numDims;
  for (i = 0; i < numDims; i++) {
    hash += srcDims[i] + sLos[i] + sHis[i] + sStrides[i];
    hash += destDims[i] + dLos[i] + dHis[i] + dStrides[i];
  }
  hash &= 0xff;

/*
printf("Node %d: generating subarray hash value %d\n", PARTI_myproc(), hash);
*/
  return hash;
}

void insert_sub_table(sub, srcDA, destDA, numDims,
		      srcDims, sLos, sHis, sStrides,
		      destDims, dLos, dHis, dStrides)
SCHED* sub;
DARRAY *srcDA, *destDA;
int numDims;
int *srcDims, *sLos, *sHis, *sStrides;
int *destDims, *dLos, *dHis, *dStrides;
{
  int i, j;
  sub_cell* subc;
  sub_cell* x;

  if (!sub_done_init)
    init_sub_table();
  i = sub_hash(srcDA, destDA, numDims,
	       srcDims, sLos, sHis, sStrides,
	       destDims, dLos, dHis, dStrides);
  sub -> hash = i;
  
  subc = NEW(sub_cell);
  subc->referenceCount = 0;     /* Code added by Dan Quinlan */
  subc->subarray_sched = sub;
  sub->referenceCount++;     /* Code added by Dan Quinlan */
  subc->srcDA = srcDA;
  subc->destDA = destDA;
  subc->numDims = numDims;
  for (j = 0; j < numDims; j++) {
    subc->srcDims[j] = srcDims[j];
    subc->sLos[j] = sLos[j];
    subc->sHis[j] = sHis[j];
    subc->sStrides[j] = sStrides[j];
    subc->destDims[j] = destDims[j];
    subc->dLos[j] = dLos[j];
    subc->dHis[j] = dHis[j];
    subc->dStrides[j] = dStrides[j];
  }

/* Initialize the rest to avoid purify URM errors */
  for (j = numDims; j < MAX_DIM; j++) {
    subc->srcDims[j]  = 0;
    subc->sLos[j]     = 0;
    subc->sHis[j]     = 0;
    subc->sStrides[j] = 0;
    subc->destDims[j] = 0;
    subc->dLos[j]     = 0;
    subc->dHis[j]     = 0;
    subc->dStrides[j] = 0;
  }

  insert_List(subc, subarray_table[i]);
}

void delete_sub_table(sched)
SCHED* sched;
{
  int i = sched -> hash;
  sub_cell* subc;

  FOREACH(subc, subarray_table[i]){
    if (subc -> subarray_sched == sched) {
      (void) remove_List(subc, subarray_table[i]);
      break;
    }
  }
}

SCHED* lookup_sub(srcDA, destDA, numDims,
		  srcDims, sLos, sHis, sStrides,
		  destDims, dLos, dHis, dStrides)
DARRAY *srcDA, *destDA;
int numDims;
int *srcDims, *sLos, *sHis, *sStrides;
int *destDims, *dLos, *dHis, *dStrides;
{
  int i, found, j;
  sub_cell* v;

  if (!sub_done_init)
    init_sub_table();
  i = sub_hash(srcDA, destDA, numDims,
	       srcDims, sLos, sHis, sStrides,
	       destDims, dLos, dHis, dStrides);

  FOREACH(v, subarray_table[i]) {
    found = 1;
    if (v->srcDA != srcDA || v->destDA != destDA || v->numDims != numDims)
      found = 0;

#if 0
 /* This is part of debuging code used to isolate purify UMR errors */
    printf ("numDims = %d \n",numDims);

    assert(v != NULL);
    assert(v->srcDims[0] >= 0);
    assert(v->sLos[0] >= 0);
    assert(v->sHis[0] >= 0);
    assert(v->sStrides[0] >= 0);
    assert(srcDims[0] >= 0);
    assert(sLos[0] >= 0);
    assert(sHis[0] >= 0);
    assert(sStrides[0] >= 0);
    assert(v->destDims[0] >= 0);
    assert(v->dLos[0] >= 0);
    assert(v->dHis[0] >= 0);
    assert(v->dStrides[0] >= 0);
    assert(destDims[0] >= 0);
    assert(dLos[0] >= 0);
    assert(dHis[0] >= 0);
    assert(dStrides[0] >= 0);

    assert(v->srcDims[1] >= 0);
    assert(v->sLos[1] >= 0);
    assert(v->sHis[1] >= 0);
    assert(v->sStrides[1] >= 0);
    assert(srcDims[1] >= 0);
    assert(sLos[1] >= 0);
    assert(sHis[1] >= 0);
    assert(sStrides[1] >= 0);
    assert(v->destDims[1] >= 0);
    assert(v->dLos[1] >= 0);
    assert(v->dHis[1] >= 0);
    assert(v->dStrides[1] >= 0);
    assert(destDims[1] >= 0);
    assert(dLos[1] >= 0);
    assert(dHis[1] >= 0);
    assert(dStrides[1] >= 0);

    assert(v->srcDims[2] >= 0);
    assert(v->sLos[2] >= 0);
    assert(v->sHis[2] >= 0);
    assert(v->sStrides[2] >= 0);
    assert(srcDims[2] >= 0);
    assert(sLos[2] >= 0);
    assert(sHis[2] >= 0);
    assert(sStrides[2] >= 0);
    assert(v->destDims[2] >= 0);
    assert(v->dLos[2] >= 0);
    assert(v->dHis[2] >= 0);
    assert(v->dStrides[2] >= 0);
    assert(destDims[2] >= 0);
    assert(dLos[2] >= 0);
    assert(dHis[2] >= 0);
    assert(dStrides[2] >= 0);

    assert(v->srcDims[3] >= 0);
    assert(v->sLos[3] >= 0);
    assert(v->sHis[3] >= 0);
    assert(v->sStrides[3] >= 0);
    assert(srcDims[3] >= 0);
    assert(sLos[3] >= 0);
    assert(sHis[3] >= 0);
    assert(sStrides[3] >= 0);
    assert(v->destDims[3] >= 0);
    assert(v->dLos[3] >= 0);
    assert(v->dHis[3] >= 0);
    assert(v->dStrides[3] >= 0);
    assert(destDims[3] >= 0);
    assert(dLos[3] >= 0);
    assert(dHis[3] >= 0);
    assert(dStrides[3] >= 0);

    assert(v->srcDims[4] >= 0);
    assert(v->sLos[4] >= 0);
    assert(v->sHis[4] >= 0);
    assert(v->sStrides[4] >= 0);
    assert(srcDims[4] >= 0);
    assert(sLos[4] >= 0);
    assert(sHis[4] >= 0);
    assert(sStrides[4] >= 0);
    assert(v->destDims[4] >= 0);
    assert(v->dLos[4] >= 0);
    assert(v->dHis[4] >= 0);
    assert(v->dStrides[4] >= 0);
    assert(destDims[4] >= 0);
    assert(dLos[4] >= 0);
    assert(dHis[4] >= 0);
    assert(dStrides[4] >= 0);

    assert(v->srcDims[5] >= 0);
    assert(v->sLos[5] >= 0);
    assert(v->sHis[5] >= 0);
    assert(v->sStrides[5] >= 0);
    assert(srcDims[5] >= 0);
    assert(sLos[5] >= 0);
    assert(sHis[5] >= 0);
    assert(sStrides[5] >= 0);
    assert(v->destDims[5] >= 0);
    assert(v->dLos[5] >= 0);
    assert(v->dHis[5] >= 0);
    assert(v->dStrides[5] >= 0);
    assert(destDims[5] >= 0);
    assert(dLos[5] >= 0);
    assert(dHis[5] >= 0);
    assert(dStrides[5] >= 0);
#endif

    for (j = 0; j < numDims; j++)
       {
         if (v->srcDims[j] != srcDims[j] || v->sLos[j] != sLos[j] ||
             v->sHis[j] != sHis[j] || v->sStrides[j] != sStrides[j])
              found = 0;
         if (v->destDims[j] != destDims[j] || v->dLos[j] != dLos[j] ||
             v->dHis[j] != dHis[j] || v->dStrides[j] != dStrides[j])
              found = 0;
    }

    if (found) return v->subarray_sched;
  }

  return NULL;
}

/* Function added by Dan Quinlan dquinlan@lanl.gov */
void cleanup_da_table()
   {
     int i = 0;
  /* printf ("Inside of cleanup_da_table \n"); */
     for (i=0; i < DA_TABLE_SIZE; i++)
        {
       /* printf ("da_table[%d] = %p \n",i,(void*)(da_table[i])); */
#if 1
          if (da_table[i] != NULL)
             {
               if (da_table[i]->referenceCount-- == 0)
                  {
                  /* Should't we use the delete_DARRAY function here! */
                 /* printf ("Should't we use the delete_DARRAY function here! \n"); */
                 /* free (da_table[i]); */
                    delete_DARRAY(da_table[i]);
                  }
               da_table[i] = NULL;
             }
#endif
        }
   }

/* Function added by Dan Quinlan dquinlan@lanl.gov */
void cleanup_subarray_table()
   {
#if 1
  /* printf ("Inside of cleanup_subarray_table \n"); */
     destroy_sub_table();
#else
     int i = 0;
     List l;
     Cell* c;
     Cell* t;
     for (i=0; i < SCHED_TABLE_SIZE; i++)
        {
       /* printf ("subarray_table[%d] = %p \n",i,subarray_table[i]); */
       /* int_destroy_List (subarray_table[i]); */

          if (subarray_table[i] != NULL)
             {
               l = subarray_table[i];
               c = FIRST(l);
               while (STILL_IN(c, l)) 
                  {
                    t = NEXT(c);
                    if (l->dummy != NULL) 
                         delete_Cell(l->dummy);
                    if (l->fptr  != NULL) 
                         delete_Cell(l->fptr);
                    if (l->optr  != NULL) 
                         delete_Cell(l->optr);

                    assert (c->referenceCount >= 0);
                    if (c->referenceCount-- == 0)
                       {
                      /* DISPOSE(c); */
                         delete_Cell(c);
                       }
                    c = t;
                  }
               assert (l->dummy->referenceCount >= 0);
               if (l->dummy->referenceCount-- == 0)
                  {
                    DISPOSE(l->dummy);
                  }
             }
        }
#endif
   }

/* Function added by Dan Quinlan dquinlan@lanl.gov */
void cleanup_exch_table()
   {
#if 1
  /* printf ("Inside of cleanup_exch_table \n"); */
     destroy_exch_table();
#else
     int i = 0;
     List l;
     Cell* c;
     Cell* t;
     for (i=0; i < SCHED_TABLE_SIZE; i++)
        {
       /* printf ("exch_table[%d] = %p \n",i,exch_table[i]); */
       /* int_destroy_List (exch_table[i]); */

          if (exch_table[i] != NULL)
             {
               l = exch_table[i];
               c = FIRST(l);
               while (STILL_IN(c, l)) 
                  {
                    t = NEXT(c);

                    assert (c->referenceCount >= 0);
                    if (c->referenceCount-- == 0)
                       {
                      /* DISPOSE(c); */
                         delete_Cell(c);
                       }
                    c = t;
                  }

               assert (l->dummy->referenceCount >= 0);
               if (l->dummy->referenceCount-- == 0)
                  {
                    DISPOSE(l->dummy);
                  }
             }
        }
#endif
   }


