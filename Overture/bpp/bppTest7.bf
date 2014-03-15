#beginMacro dd(operator)
    #If #operator == "divScalarGrad"
      defineA22R()
      beginLoops()
      loopBody2ndOrder2d(0.,a22(i1,i2,i3),0., a11(i1,i2,i3),\
           -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2,i3)+a22(i1,i2+1,i3)), \
                  a11(i1+1,i2,i3),  0.,a22(i1,i2+1,i3),0.)
      endLoops()
    #Else
      beginLoops()
      #If #operator == "laplacian"
       loopBody2ndOrder2d(0.,h22(2),0., h22(1),-2.*(h22(1)+h22(2)),h22(1), 0.,h22(2),0.)
      #Elif #operator == "x"
        x2ndOrder2dRectangular(x,1)
      #End
      endLoops()
    #End
#endMacro

      dd(laplacian)

