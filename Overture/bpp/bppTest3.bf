c  define a macro
#beginMacro FN(name)
   subroutine(name)
#endMacro

#beginMacro op(name)
  #If #name == "x" 
    FN(x)
  #Elif #name == "y"
    call y ## name()
  #End
#endMacro

      op(x)

      op(y)


c #Define op "xx"
c 
c #If op == "xx" 
c       subroutine xx()
c #Else
c       subroutine yy()
c #End

#beginFile bppTest3a.f
      op(x)
#endFile


#appendFile bppTest3a.f
      op(laplacian)
#endFile


      return
      end
