#include <stdio.h>

#include "List.h"

#undef NIL
#define NIL(X) ((X*) 0)


/*
 * This file implements:
 *   doubly linked, circular lists of things
 */
  
  
/* create an empty List */
List create_List()
{
  List l;
  Cell* c;

  /* empty lists always have at least one thing in them */
  l = NEW(struct _list);
  l->referenceCount = 0; /* Code added by Dan Quinlan */
  c = NEW(Cell);
  c->referenceCount = 0; /* Code added by Dan Quinlan */
  l->dummy = c;
  c->item = NIL(void);     
  c->next = c;
  c->prev = c;

  return l;
}


/* destroy a (possibly nonempty) list */
void int_destroy_List(l)
List l;
{
  Cell* c;
  Cell* t;

  c = FIRST(l);
  while (STILL_IN(c, l)) {
    t = NEXT(c);
    assert (c->referenceCount >= 0);
    if (c->referenceCount-- == 0)
       {
         DISPOSE(c);
       }
    c = t;
  }
  assert (l->dummy->referenceCount >= 0);
  if (l->dummy->referenceCount-- == 0)
     {
       DISPOSE(l->dummy);
     }
  assert (l->referenceCount >= 0);
  if (l->referenceCount-- == 0)
     {
       DISPOSE(l);
     }
}


/* insert b onto the end of List */
void insert_List(b, l)
void* b;
List l;
{
  Cell* c;

  c = NEW(Cell);
  c->referenceCount = 0; /* Code added by Dan Quinlan */
  c->item = b;
  
  c->next = l->dummy;
  c->prev = l->dummy->prev;
  l->dummy->prev->next = c;
  l->dummy->prev = c;
}

/*
  remove a cell from a list
  if x is NIL, then remove the first cell
  otherwise, find x and remove it.
  if x is not in the list, return NIL
*/  
void* remove_List(x, l)
Gtype* x;
List l;
{
  Cell* c;
  void* b;

  PARTI_ASSERT(!EMPTY(l), "attempt to remove from empty list");
  if (x) { /* find x and remove it */
    for (c = FIRST(l); STILL_IN(c, l); c = NEXT(c)) {
      if (c->item == x)
        break;
    }
    if (STILL_IN(c, l)) { /* we found x */
      c->prev->next = c->next;
      c->next->prev = c->prev;

      assert (c->referenceCount >= 0);
      if (c->referenceCount-- == 0)
         {
           DISPOSE(c);
         }

      return x;
    }
    else /* x is not in the list */
      return NIL(Gtype);
  }
  else { /* remove the first element */
    c = FIRST(l);
    FIRST(l) = c->next;
    (FIRST(l))->prev = l->dummy;
    b = c->item;

    assert (c->referenceCount >= 0);
    if (c->referenceCount-- == 0)
       {
         DISPOSE(c);
       }
    return b;
  }
}

/*
  remove every item in l1 from l2
*/
void remove_List_List(l1, l2)
List l1;
List l2;
{
  Cell* c;
  
  for (c = FIRST(l1); STILL_IN(c, l1); c = NEXT(c))
    if (member_List(c->item, l2))
      remove_List(c->item, l2);
}



Gtype* member_List(b, l)
void* b;
List l;
{
  Cell* x;
  
  for (x = FIRST(l); STILL_IN(x, l); x = NEXT(x)) {
    if (x->item == b)
      return x->item;
  }
  return NIL(Gtype);
}


Gtype* eq_member_List(b, l, f)
void* b;
List l;
pfunc f;
{
  Cell* x;
  
  for (x = FIRST(l); STILL_IN(x, l); x = NEXT(x)) {
    if (f(x->item, b))
      return x->item;
  }
  return NIL(Gtype);
}



int length_List(l)
List l;
{
  Cell* x;
  int count = 0;
  
  for (x = FIRST(l); STILL_IN(x, l); x = NEXT(x)) 
    count++;

  return count;
}


/*
  Duplicate a list
*/
List dup_List(l)
List l;
{
  List l2 = create_List();
  Cell* c;

  for (c = FIRST(l); STILL_IN(c, l); c = NEXT(c)) {
    insert_List(c->item, l2);
  }
  return l2;
}


/*
  Append two lists together.
  l1 <-- l1 + l2
  ***** NOTE THIS DESTROYS l2!!!!! *****
  if you don't want l2 destroyed, you'd better copy it first
*/
void append_List(l1, l2)
List l1;
List l2;
{
  l1->dummy->prev->next = l2->dummy->next; /* end of l1 points to l2 */
  l2->dummy->next->prev = l1->dummy->prev; /*      vice versa        */
  l2->dummy->prev->next = l1->dummy; /* end of the list is l1->dummy */
  l1->dummy->prev = l2->dummy->prev;

  assert (l2->dummy->referenceCount >= 0);
  if (l2->dummy->referenceCount-- == 0)
     {
       DISPOSE(l2->dummy);
     }

  assert (l2->referenceCount >= 0);
  if (l2->referenceCount-- == 0)
     {
       DISPOSE(l2);
     }
}

/*
  compute the intersection of two lists
*/
List intersection_List(l1, l2)
List l1;
List l2;
{
  List l = create_List();
  Cell* c;

  for (c = FIRST(l1); STILL_IN(c, l1); c = NEXT(c)) {
    if (member_List(c->item, l2)) 
      insert_List(c->item, l);
  }
  return l;
}

/*
  compute the union of two lists
*/
List union_List(l1, l2)
List l1;
List l2;
{
  List l = dup_List(l1);
  Cell* c;

  for (c = FIRST(l2); STILL_IN(c, l2); c = NEXT(c)) {
    if (!member_List(c->item, l)) 
      insert_List(c->item, l);
  }
  return l;
}


/*
  replace one item in a list with another item
  (this function is useful in ordered lists)
*/
void replace_List(i1, i2, l)
Gtype* i1;
Gtype* i2;
List l;
{
  Cell* c;
  for (c = FIRST(l); STILL_IN(c, l); c = NEXT(c))
    if (c->item == i1) {
      c->item = i2;
      break;
    }
}

        

  

  
