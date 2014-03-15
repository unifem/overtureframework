c Include file to define the data associated with multicomponent JWL EOS
      real gm1s,amjwl,rmjwl,ai,ri,fs0,fg0,gs0,gg0,
     *     fi0,gi0,ci,cs,cg,mjwlq,mvi0,mvs0,mvg0,iheat
      integer iterations, newMethod
      common / multijwl / gm1s(3),amjwl(2,2),rmjwl(2,2),
     *     ai(2),ri(2),fs0,fg0,gs0,gg0,fi0,gi0,ci,cs,cg,mjwlq,mvi0,
     *     mvs0,mvg0,iheat,iterations,newMethod
