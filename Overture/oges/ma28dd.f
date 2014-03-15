      subroutine ma28dd(n, a, licn, ivect, jvect, nz, icn, lenr, lenrl,
     * lenoff, ip, iq, iw1, iw, w1, iflag)
c this subroutine need never be called by the user directly.
c     it sorts the user's matrix into the structure of the decomposed
c     form and checks for the presence of duplicate entries or
c     non-zeros lying outside the sparsity pattern of the decomposition
c     it also calculates the largest element in the input matrix.
      real a(licn), zero, w1, aa
      integer iw(n,2), idisp(2)
      integer icn(licn), ivect(nz), jvect(nz), ip(n), iq(n),
     * lenr(n), iw1(n,3), lenrl(n), lenoff(n)
      logical lblock, grow, blockl
      common /ma28ed/ lp, mp, lblock, grow
      common /ma28gd/ idisp
      data zero /0.0e0/
      blockl = lenoff(1).ge.0
c iw1(i,3)  is set to the block in which row i lies and the
c     inverse permutations to ip and iq are set in iw1(.,1) and
c     iw1(.,2) resp.
c pointers to beginning of the part of row i in diagonal and
c   off-diagonal blocks are set in iw(i,2) and iw(i,1) resp.
      iblock = 1
      iw(1,1) = 1
      iw(1,2) = idisp(1)
      do 10 i=1,n
        iw1(i,3) = iblock
        if (ip(i).lt.0) iblock = iblock + 1
        ii = iabs(ip(i)+0)
        iw1(ii,1) = i
        jj = iq(i)
        jj = iabs(jj)
        iw1(jj,2) = i
        if (i.eq.1) go to 10
        if (blockl) iw(i,1) = iw(i-1,1) + lenoff(i-1)
        iw(i,2) = iw(i-1,2) + lenr(i-1)
   10 continue
c place each non-zero in turn into its correct location
c    in the a/icn array.
      idisp2 = idisp(2)
      do 170 i=1,nz
c necessary to avoid reference to unassigned element of icn.
        if (i.gt.idisp2) go to 20
        if (icn(i).lt.0) go to 170
   20   iold = ivect(i)
        jold = jvect(i)
        aa = a(i)
c this is a dummy loop for following a chain of interchanges.
c   it will be executed nz times in total.
        do 140 idummy=1,nz
c perform some validity checks on iold and jold.
          if (iold.le.n .and. iold.gt.0 .and. jold.le.n .and.
     *     jold.gt.0) go to 30
          if (lp.ne.0) write (lp,99999) i, a(i), iold, jold
          iflag = -12
          go to 180
   30     inew = iw1(iold,1)
          jnew = iw1(jold,2)
c are we in a valid block and is it diagonal or off-diagonal?
          if (iw1(inew,3)-iw1(jnew,3)) 40, 60, 50
   40     iflag = -13
          if (lp.ne.0) write (lp,99998) iold, jold
          go to 180
   50     j1 = iw(inew,1)
          j2 = j1 + lenoff(inew) - 1
          go to 110
c element is in diagonal block.
   60     j1 = iw(inew,2)
          if (inew.gt.jnew) go to 70
          j2 = j1 + lenr(inew) - 1
          j1 = j1 + lenrl(inew)
          go to 110
   70     j2 = j1 + lenrl(inew)
c binary search of ordered list  .. element in l part of row.
          do 100 jdummy=1,n
            midpt = (j1+j2)/2
            jcomp = iabs(icn(midpt)+0)
            if (jnew-jcomp) 80, 130, 90
   80       j2 = midpt
            go to 100
   90       j1 = midpt
  100     continue
          iflag = -13
          if (lp.ne.0) write (lp,99997) iold, jold
          go to 180
c linear search ... element in l part of row or off-diagonal blocks.
  110     do 120 midpt=j1,j2
            if (iabs(icn(midpt)+0).eq.jnew) go to 130
  120     continue
          iflag = -13
          if (lp.ne.0) write (lp,99997) iold, jold
          go to 180
c equivalent element of icn is in position midpt.
  130     if (icn(midpt).lt.0) go to 160
          if (midpt.gt.nz .or. midpt.le.i) go to 150
          w1 = a(midpt)
          a(midpt) = aa
          aa = w1
          iold = ivect(midpt)
          jold = jvect(midpt)
          icn(midpt) = -icn(midpt)
  140   continue
  150   a(midpt) = aa
        icn(midpt) = -icn(midpt)
        go to 170
  160   a(midpt) = a(midpt) + aa
c set flag for duplicate elements.
        iflag = n + 1
  170 continue
c reset icn array  and zero elements in l/u but not in a.
c also calculate maximum element of a.
  180 w1 = zero
      do 200 i=1,idisp2
        if (icn(i).lt.0) go to 190
        a(i) = zero
        go to 200
  190   icn(i) = -icn(i)
        w1 = max(w1,abs(a(i)))
  200 continue
      return
99999 format (9h element , i6, 12h with value , 1pd22.14, 10h has indic,
     * 3hes , i8, 2h ,, i8/36x, 20hindices out of range)
99998 format (36x, 8hnon-zero, i7, 2h ,, i6, 23h in zero off-diagonal b,
     * 4hlock)
99997 format (36x, 8h element, i6, 2h ,, i6, 23h was not in l/u pattern)
      end
