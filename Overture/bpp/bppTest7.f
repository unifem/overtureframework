! This file automatically generated from bppTest7.bf with bpp.

! dd(laplacian)
!           #If "laplacian" == "divScalarGrad"
!           #Else
            beginLoops()
!             #If "laplacian" == "laplacian"
             loopBody2ndOrder2d(0.,h22(2),0., h22(1),-2.*(h22(1)+h22(2)
     & ),h22(1), 0.,h22(2),0.)
            endLoops()

