#beginMacro dd(operator)
    #If #operator == "divScalarGrad"
      beginLoops()
      endLoops()
    #Else
      beginLoops()
      #If #operator == "laplacian"
       loopBody2ndOrder2d(0.,h22(2),0., h22(1),-2.*(h22(1)+h22(2)),h22(1), 0.,h22(2),0.)
      #Elif #operator == "xy"
        d=h21(1)*h21(2)
        loopBody2ndOrder2d(d,0.,-d, 0.,0.,0., -d,0.,d)
      #End
      endLoops()
    #End
#endMacro

      dd(divScalarGrad)



