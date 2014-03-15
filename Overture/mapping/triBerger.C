
-------------------------check_edge---------------------------------
      subroutine check_edge(iedge ,tri0, tri1,     intersection,
     .                      nNewVerts,    NewVertList,
     .                      next_free,    num_needed, NewSegList,
     .                      TriList,      VertList,  EdgeList)

!                                                         ===inline===
!
!                                                 ...intersects iedge (tri1)
!                                                    against tri0
!                                                 1) Boolean check
!                                                 2) if pass (1), then
!                                                    compute intersect pt
!                                            * Note:"intersection" describes &
!                                                    classifies the result
!                                                   "Certain" identifies close
!                                                    calls.
      include 'param.i'
!     ----------------------              define fields for NewSegList
      integer    NSEG_FIELDS, NEW_E1, NEW_E2, INTTRI, NEXT, THREAD

      parameter (NSEG_FIELDS = 7)
      parameter (     NEW_E1 = 3)
      parameter (     NEW_E2 = 4)
      parameter (     INTTRI = 5)
      parameter (       NEXT = 6)
      parameter (     THREAD = 7)

      integer NewSegList(NSEG_FIELDS,*)
!     ----------------------
      integer  iedge, vert1, vert2, tri0, tri1,  intersection
      integer  next_free,  head
      integer  OppTri1,num_needed

      integer nNewVerts, NewVertList(2,*)

      logical DoesIntersect, Certain
      logical new_seg, found_old, check_done

!     -----------------------------------------------------
!                                                     ...set up edge vertices
      vert1 = EdgeList(V1,iedge)
      vert2 = EdgeList(V2,iedge)

!     -------------------------
!                                                     ...identify triangle 
!                                                        opposite to from 
!                                                        tri1 on iedge
      OppTri1 = EdgeList(T1,iedge)
      if (OppTri1 .EQ. tri1) OppTri1 = EdgeList(T2,iedge)

!     -------------------------
!                                                     ...check if this 
!                                                        intersection has 
!                                                        already been computed
      head = TriList(SEG_ENTRY,tri0)

      if (head .NE. NULL) then
        found_old = check_done( tri0, tri1, head, next_free,
     .                          iedge, new_seg, NewSegList)
        if (found_old) then
          Intersection = PROPER
!. . .    print*,'already have seg: edge (v1,v2) =' ,vert1,vert2,
!     .          ' tri ',tri0 
          return
        endif
      endif

!     -------------------------

      call CheckIntersection(tri0, vert1,  vert2,
     .             DoesIntersect,  Intersection,  Certain, 
     .             TriList,        VertList,      EdgeList )

!     -------------------------
!                                                 ..new intersection found (!)
      if (DoesIntersect) then 

!       -------

        if (.not.Certain) then
          print*,' '
          print*,' ==> WARNING: not certain about , edge/tri',
     .           iedge, tri0
        endif

!       -------                                  a) create the new vertex

        call create_new_vertex(    iedge,        tri0,
     .                         nNewVerts, NewVertList,
     .                           TriList,    VertList,  EdgeList)

!       -------                                  b) create new segments linked
!                                                   to tri0, tri1,OppTri1

        call create_segs(iedge, tri0, tri1, Opptri1,
     .                   nNewVerts, next_free,
     .                   intersection, new_seg,    NewSegList,
     .                   TriList,      VertList,      EdgeList  )

!. . .  print*,'creating 1/2 seg: edge (v1,v2) =' ,vert1,vert2,
!     .        ' tri ',tri0 

!       ------- 

      endif

!     -----------------------------------------------------
 999    format(' ---> found intersection of edge',i3,i3,
     .         ' with triangle ',i3)
!     -----------------------------------------------------
      return
      end


!===
      logical function check_done( tri0, tri1, head, next_free,
     .                             iedge, new_seg, NewSegList)
!
!                                                      === inline ===
!                                      ...Run  down multiply threaded
!                                         linked list of NewSegList
!                                         looking for a match to tri1
!                                         on tri0s thread.
!     ----------------------           ...define fields for NewSegList

      integer    NSEG_FIELDS, NEW_E1, NEW_E2, INTTRI, NEXT, NULL,V1,V2
      integer    THREAD

      parameter (NSEG_FIELDS = 7)
      parameter (       NULL = 0)
      parameter (         V1 = 1)
      parameter (         V2 = 2)
      parameter (     NEW_E1 = 3)
      parameter (     NEW_E2 = 4)
      parameter (     INTTRI = 5)
      parameter (       NEXT = 6)
      parameter (     THREAD = 7)

      integer NewSegList(NSEG_FIELDS,*)

      integer tri0, tri1, head, next_free, num_needed,iedge
      integer next_seg,   icount, old_head
      
      logical new_seg, found_one
      
!     -----------------------------------------------------
!     ----------------------              ... scan tri0s thread to check for
!                                             half completed intersections
!                                             with iedge and tri1
      next_seg  = head
      icount    = 1
      found_one = .FALSE.

      do while (NewSegList( NEXT  , next_seg ) .ne. NULL )
        
        if (NewSegList(THREAD,next_seg) .NE. tri0) then   !          (safty)
          write(*,*) ' ==>ERROR: Threads crossed in linked list'
          write(*,*) '           see "check_edge.f" for tri0=',tri0
          stop
        endif

        if (NewSegList(INTTRI,next_seg) .EQ. tri1 .AND.
     .     (NewSegList(NEW_E1,next_seg) .EQ. iedge).OR.
     .     (NewSegList(NEW_E2,next_seg) .EQ. iedge)) then !  found pre-exist
!                                                                    segment
          found_one = .TRUE.
          goto 10       
        end if

        next_seg = NewSegList( NEXT, next_seg )
        icount = icount + 1 
        
        if (icount .GT. next_free ) then
          write(*,*)' ==>ERROR: infinite while loop in check_done'
          write(*,*)'           see file "check_edge.f", tri0=',tri0
          stop
        endif

      end do
!     ----------------------  
!                                                       ...check tail
      if (NewSegList(THREAD,next_seg) .NE. tri0) then    !          (safty)
        write(*,*) ' ==>ERROR: Threads crossed in linked list'
        write(*,*) '           see "check_edge.f" for tri0=',tri0
        stop
      endif

      if (NewSegList(INTTRI,next_seg) .EQ. tri1 .AND.
     .   (NewSegList(NEW_E1,next_seg) .EQ. iedge).OR.
     .   (NewSegList(NEW_E2,next_seg) .EQ. iedge)) then !  found pre-exist
!                                                                  segment
          found_one = .TRUE. 
          goto 10       
      endif
!     ----------------------  
!                                                       ...return result
 10   if (found_one) then
        check_done = .TRUE.
      else
        check_done = .FALSE.
      endif

!     -----------------------------------------------------
!     * NOTES:  Can live without the thread checking, but its safe
!     -----------------------------------------------------
      return
      end








------------------------- checkintersection.f --------------------------
!      Interface to the C version of checkintersection.
!       Note that this step  is just to make the code in C neat.
!
!
!
      subroutine checkintersection ( itri, pt1,pt2,
     .                    DoesIntersect,  Intersection, Certain,
     .                    TriList,        VertList,     EdgeList  )


      include 'param.i'

      integer           itri,  pt1,   pt2, vert1, vert2, vert3
      integer           intersection

      logical           DoesIntersect, Certain, Opposites

      double precision  vv1(3),vv2(3),vv3(3),p1(3),p2(3)


      vert1         = TriList(v1, itri)
      vert2         = TriList(v2, itri)
      vert3         = TriList(v3, itri)

      vv1(1)         = dble ( VertList(1,vert1) )   
      vv1(2)         = dble ( VertList(2,vert1) )   
      vv1(3)         = dble ( VertList(3,vert1) )   
      
      vv2(1)         = dble ( VertList(1,vert2) )   
      vv2(2)         = dble ( VertList(2,vert2) )   
      vv2(3)         = dble ( VertList(3,vert2) )   
      
      vv3(1)         = dble ( VertList(1,vert3) )   
      vv3(2)         = dble ( VertList(2,vert3) )   
      vv3(3)         = dble ( VertList(3,vert3) )   
      
      p1(1)         = dble ( VertList(1,pt1) )   
      p1(2)         = dble ( VertList(2,pt1) )   
      p1(3)         = dble ( VertList(3,pt1) )   
      
      p2(1)         = dble ( VertList(1,pt2) )   
      p2(2)         = dble ( VertList(2,pt2) )   
      p2(3)         = dble ( VertList(3,pt2) )   
      

      call   checkintersectionC( vv1,vv2,vv3, p1,p2,
     .                    vert1,vert2,vert3,pt1,pt2,Intersection )

      if (Intersection.EQ.1) then
                   DoesIntersect =.TRUE.
      else        
                   DoesIntersect =.FALSE.
      end if 

      Certain       = .TRUE.

      return
      end

!===

      subroutine CheckRay ( itri, p1, p2, DoesIntersect,
     .                      TriList, VertList, EdgeList, ref_tri)
      include 'param.i'

      integer           itri,  vert1, vert2, vert3, ref_tri
      integer           intersection, max_index, p1index, p2index

      logical           DoesIntersect

      double precision  vv1(3),vv2(3),vv3(3),p1(3),p2(3)
!     -------------

      vert1 = TriList(v1, itri)
      vert2 = TriList(v2, itri)
      vert3 = TriList(v3, itri)

      max_index = max(vert1,vert2,vert3)
!                                               ...set up indices for p1/p2 
      p2index = 2*max_index + 1 !                  (for the sort in sos)-take
c .. mjb
c     p2index = nVerts+1
      p2index = 999999
c .. mjb
      p1index = TriList(V1,ref_tri) !              ray root indx from orig tri

      vv1(1) = dble ( VertList(1,vert1) )   
      vv1(2) = dble ( VertList(2,vert1) )   
      vv1(3) = dble ( VertList(3,vert1) )   
      
      vv2(1) = dble ( VertList(1,vert2) )   
      vv2(2) = dble ( VertList(2,vert2) )   
      vv2(3) = dble ( VertList(3,vert2) )   
      
      vv3(1) = dble ( VertList(1,vert3) )   
      vv3(2) = dble ( VertList(2,vert3) )   
      vv3(3) = dble ( VertList(3,vert3) )   

      call   checkintersectionC( vv1,  vv2,  vv3,  p1,     p2,
     .                           vert1,vert2,vert3,p1index,p2index,
     .                                                    Intersection )

      if (Intersection.EQ.1) then
                   DoesIntersect =.TRUE.
      else        
                   DoesIntersect =.FALSE.
      end if 

      return
      end

!===


------------------------- det.c -------------------------------------
#include <stdio.h>
#include "predicates.h"

#define TRUE     1
#define FALSE    0
#define PROPER   1
#define COPLANAR 2
#define NO       0

/*   --------------------------------------------  */
/*   test whether the line  (p1,p2) intersect 
     the triangle    (t1,t2,t3)                    */
/*   --------------------------------------------  */


int  sign ( s)
double s;
{
   if (s>0.0) return (1);
   if (s<0.0) return (-1);
              return (0);
}



int sos_2d ( double a1,  double a2,
             double b1,  double b2,
             double c1,  double c2 )
{
   double t1[2], t2[2], t3[2];

   t1[0] = a1;  t1[1] = a2;
   t2[0] = b1;  t2[1] = b2;
   t3[0] = c1;  t3[1] = c2;

   return (sign ( orient2d ( t1,t2,t3) ) );

}

int sos_1d ( double a,
             double b )
{
   if (a>b)   return (1);
   if (a<b)   return (-1);
              return (0);
}

int sos_3d (     double *t1, double *t2, double *t3, double *t4 )
{
    int d;

        d  =  sign ( orient3d ( t1,t2,t3,t4 )); if (d!=0)  return (d);

        d =       sos_2d ( t2[0], t2[1],
                           t3[0], t3[1],
                           t4[0], t4[1] );      if (d!=0)  return (d);
 
        d =  -1 * sos_2d ( t2[0], t2[2],
                           t3[0], t3[2],
                           t4[0], t4[2] );      if (d!=0)  return (d);

        d =       sos_2d ( t2[1], t2[2],
                           t3[1], t3[2],
                           t4[1], t4[2] );      if (d!=0)  return (d);

        d =  -1 * sos_2d ( t1[0], t1[1],
                           t3[0], t3[1],
                           t4[0], t4[1] );      if (d!=0)  return (d);

        d =       sos_1d ( t3[0],
                           t4[0] );             if (d!=0)  return (d);

        d =  -1 * sos_1d ( t3[1],
                           t4[1] );             if (d!=0)  return (d);

        d =       sos_2d ( t1[0], t1[2],
                           t3[0], t3[2],
                           t4[0], t4[2] );      if (d!=0)  return (d);

        d =       sos_1d ( t3[2],
                           t4[2] );             if (d!=0)  return (d);

        d =  -1 * sos_2d ( t1[1], t1[2],
                           t3[1], t3[2],
                           t4[1], t4[2] );      if (d!=0)  return (d);

        d =       sos_2d ( t1[0], t1[1],
                           t2[0], t2[1],
                           t4[0], t4[1] );      if (d!=0)  return (d);

        d =  -1 * sos_1d ( t2[0],
                           t4[0] );             if (d!=0)  return (d);

        d =       sos_1d ( t3[1],
                           t4[1] );             if (d!=0)  return (d);

        d =       sos_1d ( t3[0],
                           t4[0] );             if (d!=0)  return (d);

        return (1);

}

int sort ( int * t ,  double ** k)
{
   int  p;
   int  temp,i,j;
   double *tempk;


   p = 1;

   for (j=0;j<3;j++)
   for (i=0;i<3;i++)
      if (  t[i]>t[i+1] ) 
         {  temp   = t[i];
            t[i]   = t[i+1];
            t[i+1] = temp;

            tempk  = k[i];
            k[i]   = k[i+1];
            k[i+1] = tempk;

            p    = p * (-1);
         }     

   for (i=0;i<3;i++)
      if (t[i] == t[i+1] )
             printf ( "ERROR.... determinent must be ZERO %d %d %d %d\n",
                        t[0],t[1],t[2],t[3] ); 
   return (p);
}

int  sign_orient3d_sos ( int  a1, int  a2, int  a3, int  a4,
                   double *t1, double *t2, double *t3, double *t4 )
{
    int a[4];
    int permutation;
    double *k[4];

    /*  sorting etc... */
    a[0] = a1; a[1] = a2;  a[2] = a3;  a[3] = a4;
    k[0] = t1; k[1] = t2;  k[2] = t3;  k[3] = t4;

    permutation  = sort (a,k);

    return  ( permutation*sos_3d ( k[0] , k[1], k[2], k[3] ) );
} 

void checkintersectionc_
                       (
                         double *t1,
                         double *t2,
                         double *t3,
                         double *p1,
                         double *p2,
                         int    *p_t1,
                         int    *p_t2,
                         int    *p_t3,
                         int    *p_p1,
                         int    *p_p2,
                         int    *Intersection )

{
   int   d1,d2,d3;
   double  dd1,dd2;

/*
   printf ( " %.5G %.5G %.5G    %.5G %.5G %.5G   %.5G %.5G %.5G   ----  ",
                 t1[0],t1[1],t1[2],
                 t2[0],t2[1],t2[2],
                 t3[0],t3[1],t3[2] );
   printf ( "    with  %.5G %.5G %.5G     %.5G %.5G %.5G\n", 
                 p1[0],p1[1],p1[2],  
                 p2[0],p2[1],p2[2] );
*/

   d1 = sign ( orient3d (p1,t1,t2,t3 ) );
   d2 = sign ( orient3d (p2,t1,t2,t3 ) );

/*
   printf ( " Before..  %d %d \n", d1,d2 );
*/

   if (d1==0)  d1 = sign_orient3d_sos ( *p_p1, *p_t1, *p_t2, *p_t3,
                                             p1,    t1,    t2,    t3  );
   if (d2==0)  d2 = sign_orient3d_sos ( *p_p2, *p_t1, *p_t2, *p_t3,
                                             p2,    t1,    t2,    t3  );

/*
   printf ( " After..  %d %d \n", d1,d2 );
*/

   if ( (d1*d2) > 0 )
    {
      *Intersection =NO;            /* no intersection */
      return;
    }

   if ( (d1*d2) == 0 ) 
    {
      *Intersection =COPLANAR;      /* degeneracy  */
      inc_volume6_zero_();
      return;
    }
  
   d1 = sign ( orient3d (t1, p1,p2,t2) );
   d2 = sign ( orient3d (t2, p1,p2,t3) );
   d3 = sign ( orient3d (t3, p1,p2,t1) );
 
/*
   printf ( " Before..  %d %d %d \n", d1,d2,d3 );
*/
   if (d1==0)  d1 = sign_orient3d_sos ( *p_t1, *p_p1, *p_p2, *p_t2,
                                             t1,    p1,    p2,    t2  );
   if (d2==0)  d2 = sign_orient3d_sos ( *p_t2, *p_p1, *p_p2, *p_t3,
                                             t2,    p1,    p2,    t3  );
   if (d3==0)  d3 = sign_orient3d_sos ( *p_t3, *p_p1, *p_p2, *p_t1,
                                             t3,    p1,    p2,    t1  );

/*
   printf ( " After..  %d %d %d\n", d1,d2,d3 );
*/

   if (  (d1*d2*d3) == 0 )
      { *Intersection =COPLANAR;
         inc_volume6_zero_(); }     /* degemeracy  */ 
   else
     {
        if ( (d1 == d2) && ( d2==d3) && ( d1==d3) )
             { *Intersection =PROPER;  /* proper intersection */ }
        else { *Intersection =NO;      /* no intersection     */ }
     }
   return;
}

