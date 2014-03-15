      subroutine getArea( x1,x2,x3,x4,
     *                    y1,y2,y3,y4,
     *                    Area )
c
c..declarations of incomming variables
      implicit none
      real x1,x2,x3,x4,y1,y2,y3,y4,Area
c
c..declarations of local varuables
      real Aa,Ab,Area1,Area2

      Aa = 0.5e0*(x2*(y3-y4)+x3*(y4-y2)+x4*(y2-y3))
      Ab = 0.5e0*(x2*(y4-y1)+x4*(y1-y2)+x1*(y2-y4))
      Area = Aa+Ab

      return
      end

      
