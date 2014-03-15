! This macro set the values at the 2 ghost lines for a Neumann or mixed BC
! by using the 'normal' derivative of the interior equation
#beginMacro neumannAndEquationBC3dOrder4(DIR)
! ***************************************************************
 ! define these derivatives of the mapping (which are not normally computed by the standard macros):
ajrxxr=ajrx*ajrxrr + ajsx*ajrxrs + ajtx*ajrxrt + ajrxr*ajrxr +ajsxr*ajrxs + ajtxr*ajrxt
ajrxxs=ajrx*ajrxrs + ajsx*ajrxss + ajtx*ajrxst + ajrxs*ajrxr +ajsxs*ajrxs + ajtxs*ajrxt
ajrxxt=ajrx*ajrxrt + ajsx*ajrxst + ajtx*ajrxtt + ajrxt*ajrxr +ajsxt*ajrxs + ajtxt*ajrxt
ajrxyr=ajry*ajrxrr + ajsy*ajrxrs + ajty*ajrxrt + ajryr*ajrxr +ajsyr*ajrxs + ajtyr*ajrxt
ajrxys=ajry*ajrxrs + ajsy*ajrxss + ajty*ajrxst + ajrys*ajrxr +ajsys*ajrxs + ajtys*ajrxt
ajrxyt=ajry*ajrxrt + ajsy*ajrxst + ajty*ajrxtt + ajryt*ajrxr +ajsyt*ajrxs + ajtyt*ajrxt
ajrxzr=ajrz*ajrxrr + ajsz*ajrxrs + ajtz*ajrxrt + ajrzr*ajrxr +ajszr*ajrxs + ajtzr*ajrxt
ajrxzs=ajrz*ajrxrs + ajsz*ajrxss + ajtz*ajrxst + ajrzs*ajrxr +ajszs*ajrxs + ajtzs*ajrxt
ajrxzt=ajrz*ajrxrt + ajsz*ajrxst + ajtz*ajrxtt + ajrzt*ajrxr +ajszt*ajrxs + ajtzt*ajrxt
ajryxr=ajrx*ajryrr + ajsx*ajryrs + ajtx*ajryrt + ajrxr*ajryr +ajsxr*ajrys + ajtxr*ajryt
ajryxs=ajrx*ajryrs + ajsx*ajryss + ajtx*ajryst + ajrxs*ajryr +ajsxs*ajrys + ajtxs*ajryt
ajryxt=ajrx*ajryrt + ajsx*ajryst + ajtx*ajrytt + ajrxt*ajryr +ajsxt*ajrys + ajtxt*ajryt
ajryyr=ajry*ajryrr + ajsy*ajryrs + ajty*ajryrt + ajryr*ajryr +ajsyr*ajrys + ajtyr*ajryt
ajryys=ajry*ajryrs + ajsy*ajryss + ajty*ajryst + ajrys*ajryr +ajsys*ajrys + ajtys*ajryt
ajryyt=ajry*ajryrt + ajsy*ajryst + ajty*ajrytt + ajryt*ajryr +ajsyt*ajrys + ajtyt*ajryt
ajryzr=ajrz*ajryrr + ajsz*ajryrs + ajtz*ajryrt + ajrzr*ajryr +ajszr*ajrys + ajtzr*ajryt
ajryzs=ajrz*ajryrs + ajsz*ajryss + ajtz*ajryst + ajrzs*ajryr +ajszs*ajrys + ajtzs*ajryt
ajryzt=ajrz*ajryrt + ajsz*ajryst + ajtz*ajrytt + ajrzt*ajryr +ajszt*ajrys + ajtzt*ajryt
ajrzxr=ajrx*ajrzrr + ajsx*ajrzrs + ajtx*ajrzrt + ajrxr*ajrzr +ajsxr*ajrzs + ajtxr*ajrzt
ajrzxs=ajrx*ajrzrs + ajsx*ajrzss + ajtx*ajrzst + ajrxs*ajrzr +ajsxs*ajrzs + ajtxs*ajrzt
ajrzxt=ajrx*ajrzrt + ajsx*ajrzst + ajtx*ajrztt + ajrxt*ajrzr +ajsxt*ajrzs + ajtxt*ajrzt
ajrzyr=ajry*ajrzrr + ajsy*ajrzrs + ajty*ajrzrt + ajryr*ajrzr +ajsyr*ajrzs + ajtyr*ajrzt
ajrzys=ajry*ajrzrs + ajsy*ajrzss + ajty*ajrzst + ajrys*ajrzr +ajsys*ajrzs + ajtys*ajrzt
ajrzyt=ajry*ajrzrt + ajsy*ajrzst + ajty*ajrztt + ajryt*ajrzr +ajsyt*ajrzs + ajtyt*ajrzt
ajrzzr=ajrz*ajrzrr + ajsz*ajrzrs + ajtz*ajrzrt + ajrzr*ajrzr +ajszr*ajrzs + ajtzr*ajrzt
ajrzzs=ajrz*ajrzrs + ajsz*ajrzss + ajtz*ajrzst + ajrzs*ajrzr +ajszs*ajrzs + ajtzs*ajrzt
ajrzzt=ajrz*ajrzrt + ajsz*ajrzst + ajtz*ajrztt + ajrzt*ajrzr +ajszt*ajrzs + ajtzt*ajrzt
ajsxxr=ajrx*ajsxrr + ajsx*ajsxrs + ajtx*ajsxrt + ajrxr*ajsxr +ajsxr*ajsxs + ajtxr*ajsxt
ajsxxs=ajrx*ajsxrs + ajsx*ajsxss + ajtx*ajsxst + ajrxs*ajsxr +ajsxs*ajsxs + ajtxs*ajsxt
ajsxxt=ajrx*ajsxrt + ajsx*ajsxst + ajtx*ajsxtt + ajrxt*ajsxr +ajsxt*ajsxs + ajtxt*ajsxt
ajsxyr=ajry*ajsxrr + ajsy*ajsxrs + ajty*ajsxrt + ajryr*ajsxr +ajsyr*ajsxs + ajtyr*ajsxt
ajsxys=ajry*ajsxrs + ajsy*ajsxss + ajty*ajsxst + ajrys*ajsxr +ajsys*ajsxs + ajtys*ajsxt
ajsxyt=ajry*ajsxrt + ajsy*ajsxst + ajty*ajsxtt + ajryt*ajsxr +ajsyt*ajsxs + ajtyt*ajsxt
ajsxzr=ajrz*ajsxrr + ajsz*ajsxrs + ajtz*ajsxrt + ajrzr*ajsxr +ajszr*ajsxs + ajtzr*ajsxt
ajsxzs=ajrz*ajsxrs + ajsz*ajsxss + ajtz*ajsxst + ajrzs*ajsxr +ajszs*ajsxs + ajtzs*ajsxt
ajsxzt=ajrz*ajsxrt + ajsz*ajsxst + ajtz*ajsxtt + ajrzt*ajsxr +ajszt*ajsxs + ajtzt*ajsxt
ajsyxr=ajrx*ajsyrr + ajsx*ajsyrs + ajtx*ajsyrt + ajrxr*ajsyr +ajsxr*ajsys + ajtxr*ajsyt
ajsyxs=ajrx*ajsyrs + ajsx*ajsyss + ajtx*ajsyst + ajrxs*ajsyr +ajsxs*ajsys + ajtxs*ajsyt
ajsyxt=ajrx*ajsyrt + ajsx*ajsyst + ajtx*ajsytt + ajrxt*ajsyr +ajsxt*ajsys + ajtxt*ajsyt
ajsyyr=ajry*ajsyrr + ajsy*ajsyrs + ajty*ajsyrt + ajryr*ajsyr +ajsyr*ajsys + ajtyr*ajsyt
ajsyys=ajry*ajsyrs + ajsy*ajsyss + ajty*ajsyst + ajrys*ajsyr +ajsys*ajsys + ajtys*ajsyt
ajsyyt=ajry*ajsyrt + ajsy*ajsyst + ajty*ajsytt + ajryt*ajsyr +ajsyt*ajsys + ajtyt*ajsyt
ajsyzr=ajrz*ajsyrr + ajsz*ajsyrs + ajtz*ajsyrt + ajrzr*ajsyr +ajszr*ajsys + ajtzr*ajsyt
ajsyzs=ajrz*ajsyrs + ajsz*ajsyss + ajtz*ajsyst + ajrzs*ajsyr +ajszs*ajsys + ajtzs*ajsyt
ajsyzt=ajrz*ajsyrt + ajsz*ajsyst + ajtz*ajsytt + ajrzt*ajsyr +ajszt*ajsys + ajtzt*ajsyt
ajszxr=ajrx*ajszrr + ajsx*ajszrs + ajtx*ajszrt + ajrxr*ajszr +ajsxr*ajszs + ajtxr*ajszt
ajszxs=ajrx*ajszrs + ajsx*ajszss + ajtx*ajszst + ajrxs*ajszr +ajsxs*ajszs + ajtxs*ajszt
ajszxt=ajrx*ajszrt + ajsx*ajszst + ajtx*ajsztt + ajrxt*ajszr +ajsxt*ajszs + ajtxt*ajszt
ajszyr=ajry*ajszrr + ajsy*ajszrs + ajty*ajszrt + ajryr*ajszr +ajsyr*ajszs + ajtyr*ajszt
ajszys=ajry*ajszrs + ajsy*ajszss + ajty*ajszst + ajrys*ajszr +ajsys*ajszs + ajtys*ajszt
ajszyt=ajry*ajszrt + ajsy*ajszst + ajty*ajsztt + ajryt*ajszr +ajsyt*ajszs + ajtyt*ajszt
ajszzr=ajrz*ajszrr + ajsz*ajszrs + ajtz*ajszrt + ajrzr*ajszr +ajszr*ajszs + ajtzr*ajszt
ajszzs=ajrz*ajszrs + ajsz*ajszss + ajtz*ajszst + ajrzs*ajszr +ajszs*ajszs + ajtzs*ajszt
ajszzt=ajrz*ajszrt + ajsz*ajszst + ajtz*ajsztt + ajrzt*ajszr +ajszt*ajszs + ajtzt*ajszt
ajtxxr=ajrx*ajtxrr + ajsx*ajtxrs + ajtx*ajtxrt + ajrxr*ajtxr +ajsxr*ajtxs + ajtxr*ajtxt
ajtxxs=ajrx*ajtxrs + ajsx*ajtxss + ajtx*ajtxst + ajrxs*ajtxr +ajsxs*ajtxs + ajtxs*ajtxt
ajtxxt=ajrx*ajtxrt + ajsx*ajtxst + ajtx*ajtxtt + ajrxt*ajtxr +ajsxt*ajtxs + ajtxt*ajtxt
ajtxyr=ajry*ajtxrr + ajsy*ajtxrs + ajty*ajtxrt + ajryr*ajtxr +ajsyr*ajtxs + ajtyr*ajtxt
ajtxys=ajry*ajtxrs + ajsy*ajtxss + ajty*ajtxst + ajrys*ajtxr +ajsys*ajtxs + ajtys*ajtxt
ajtxyt=ajry*ajtxrt + ajsy*ajtxst + ajty*ajtxtt + ajryt*ajtxr +ajsyt*ajtxs + ajtyt*ajtxt
ajtxzr=ajrz*ajtxrr + ajsz*ajtxrs + ajtz*ajtxrt + ajrzr*ajtxr +ajszr*ajtxs + ajtzr*ajtxt
ajtxzs=ajrz*ajtxrs + ajsz*ajtxss + ajtz*ajtxst + ajrzs*ajtxr +ajszs*ajtxs + ajtzs*ajtxt
ajtxzt=ajrz*ajtxrt + ajsz*ajtxst + ajtz*ajtxtt + ajrzt*ajtxr +ajszt*ajtxs + ajtzt*ajtxt
ajtyxr=ajrx*ajtyrr + ajsx*ajtyrs + ajtx*ajtyrt + ajrxr*ajtyr +ajsxr*ajtys + ajtxr*ajtyt
ajtyxs=ajrx*ajtyrs + ajsx*ajtyss + ajtx*ajtyst + ajrxs*ajtyr +ajsxs*ajtys + ajtxs*ajtyt
ajtyxt=ajrx*ajtyrt + ajsx*ajtyst + ajtx*ajtytt + ajrxt*ajtyr +ajsxt*ajtys + ajtxt*ajtyt
ajtyyr=ajry*ajtyrr + ajsy*ajtyrs + ajty*ajtyrt + ajryr*ajtyr +ajsyr*ajtys + ajtyr*ajtyt
ajtyys=ajry*ajtyrs + ajsy*ajtyss + ajty*ajtyst + ajrys*ajtyr +ajsys*ajtys + ajtys*ajtyt
ajtyyt=ajry*ajtyrt + ajsy*ajtyst + ajty*ajtytt + ajryt*ajtyr +ajsyt*ajtys + ajtyt*ajtyt
ajtyzr=ajrz*ajtyrr + ajsz*ajtyrs + ajtz*ajtyrt + ajrzr*ajtyr +ajszr*ajtys + ajtzr*ajtyt
ajtyzs=ajrz*ajtyrs + ajsz*ajtyss + ajtz*ajtyst + ajrzs*ajtyr +ajszs*ajtys + ajtzs*ajtyt
ajtyzt=ajrz*ajtyrt + ajsz*ajtyst + ajtz*ajtytt + ajrzt*ajtyr +ajszt*ajtys + ajtzt*ajtyt
ajtzxr=ajrx*ajtzrr + ajsx*ajtzrs + ajtx*ajtzrt + ajrxr*ajtzr +ajsxr*ajtzs + ajtxr*ajtzt
ajtzxs=ajrx*ajtzrs + ajsx*ajtzss + ajtx*ajtzst + ajrxs*ajtzr +ajsxs*ajtzs + ajtxs*ajtzt
ajtzxt=ajrx*ajtzrt + ajsx*ajtzst + ajtx*ajtztt + ajrxt*ajtzr +ajsxt*ajtzs + ajtxt*ajtzt
ajtzyr=ajry*ajtzrr + ajsy*ajtzrs + ajty*ajtzrt + ajryr*ajtzr +ajsyr*ajtzs + ajtyr*ajtzt
ajtzys=ajry*ajtzrs + ajsy*ajtzss + ajty*ajtzst + ajrys*ajtzr +ajsys*ajtzs + ajtys*ajtzt
ajtzyt=ajry*ajtzrt + ajsy*ajtzst + ajty*ajtztt + ajryt*ajtzr +ajsyt*ajtzs + ajtyt*ajtzt
ajtzzr=ajrz*ajtzrr + ajsz*ajtzrs + ajtz*ajtzrt + ajrzr*ajtzr +ajszr*ajtzs + ajtzr*ajtzt
ajtzzs=ajrz*ajtzrs + ajsz*ajtzss + ajtz*ajtzst + ajrzs*ajtzr +ajszs*ajtzs + ajtzs*ajtzt
ajtzzt=ajrz*ajtzrt + ajsz*ajtzst + ajtz*ajtztt + ajrzt*ajtzr +ajszt*ajtzs + ajtzt*ajtzt
! ***************************************************************
! PDE: cxx*uxx + cyy*uyy + czz*uzz + cxy*uxy + cxz*uxz + cyz*uyz + cx*ux + cy*uy + cz*uz + c0 *u = f 
! PDE: cRR*urr + cSS*uss + cTT*utt + cRS*urs + cRT*urt + cST*ust  + ccR*ur + ccS*us + ccT*ut + c0 *u = f 
! =============== Start: Laplace operator: ==================== 
 cxx=1.
 cyy=1.
 czz=1.
 cxy=0.
 cxz=0.
 cyz=0.
 cx=0. 
 cy=0. 
 cz=0.
 c0=0.
 cRR=cxx*ajrx**2+cyy*ajry**2+czz*ajrz**2 +cxy*ajrx*ajry+cxz*ajrx*ajrz+cyz*ajry*ajrz
 cSS=cxx*ajsx**2+cyy*ajsy**2+czz*ajsz**2 +cxy*ajsx*ajsy+cxz*ajsx*ajsz+cyz*ajsy*ajsz
 cTT=cxx*ajtx**2+cyy*ajty**2+czz*ajtz**2 +cxy*ajtx*ajty+cxz*ajtx*ajtz+cyz*ajty*ajtz
 cRS=2.*(cxx*ajrx*ajsx+cyy*ajry*ajsy+czz*ajrz*ajsz) +cxy*(ajrx*ajsy+ajry*ajsx)+cxz*(ajrx*ajsz+ajrz*ajsx)+cyz*(ajry*ajsz+ajrz*ajsy)
 cRT=2.*(cxx*ajrx*ajtx+cyy*ajry*ajty+czz*ajrz*ajtz) +cxy*(ajrx*ajty+ajry*ajtx)+cxz*(ajrx*ajtz+ajrz*ajtx)+cyz*(ajry*ajtz+ajrz*ajty)
 cST=2.*(cxx*ajsx*ajtx+cyy*ajsy*ajty+czz*ajsz*ajtz) +cxy*(ajsx*ajty+ajsy*ajtx)+cxz*(ajsx*ajtz+ajsz*ajtx)+cyz*(ajsy*ajtz+ajsz*ajty)
 ccR=cxx*ajrxx+cyy*ajryy+czz*ajrzz +cxy*ajrxy+cxz*ajrxz+cyz*ajryz + cx*ajrx+cy*ajry+cz*ajrz
 ccS=cxx*ajsxx+cyy*ajsyy+czz*ajszz +cxy*ajsxy+cxz*ajsxz+cyz*ajsyz + cx*ajsx+cy*ajsy+cz*ajsz
 ccT=cxx*ajtxx+cyy*ajtyy+czz*ajtzz +cxy*ajtxy+cxz*ajtxz+cyz*ajtyz + cx*ajtx+cy*ajty+cz*ajtz
! m=1...
 cRRr=2.*(ajrx*ajrxr+ajry*ajryr+ajrz*ajrzr)
 cRSr=2.*(ajrxr*ajsx+ajrx*ajsxr + ajryr*ajsy+ ajry*ajsyr + ajrzr*ajsz+ ajrz*ajszr)
 cRTr=2.*(ajrxr*ajtx+ajrx*ajtxr + ajryr*ajty+ ajry*ajtyr + ajrzr*ajtz+ ajrz*ajtzr)
 ccRr=ajrxxr+ajryyr+ajrzzr
 cRRs=2.*(ajrx*ajrxs+ajry*ajrys+ajrz*ajrzs)
 cRSs=2.*(ajrxs*ajsx+ajrx*ajsxs + ajrys*ajsy+ ajry*ajsys + ajrzs*ajsz+ ajrz*ajszs)
 cRTs=2.*(ajrxs*ajtx+ajrx*ajtxs + ajrys*ajty+ ajry*ajtys + ajrzs*ajtz+ ajrz*ajtzs)
 ccRs=ajrxxs+ajryys+ajrzzs
 cRRt=2.*(ajrx*ajrxt+ajry*ajryt+ajrz*ajrzt)
 cRSt=2.*(ajrxt*ajsx+ajrx*ajsxt + ajryt*ajsy+ ajry*ajsyt + ajrzt*ajsz+ ajrz*ajszt)
 cRTt=2.*(ajrxt*ajtx+ajrx*ajtxt + ajryt*ajty+ ajry*ajtyt + ajrzt*ajtz+ ajrz*ajtzt)
 ccRt=ajrxxt+ajryyt+ajrzzt
! m=2...
 cSSr=2.*(ajsx*ajsxr+ajsy*ajsyr+ajsz*ajszr)
 cSTr=2.*(ajsxr*ajtx+ajsx*ajtxr + ajsyr*ajty+ ajsy*ajtyr + ajszr*ajtz+ ajsz*ajtzr)
 ccSr=ajsxxr+ajsyyr+ajszzr
 cSSs=2.*(ajsx*ajsxs+ajsy*ajsys+ajsz*ajszs)
 cSTs=2.*(ajsxs*ajtx+ajsx*ajtxs + ajsys*ajty+ ajsy*ajtys + ajszs*ajtz+ ajsz*ajtzs)
 ccSs=ajsxxs+ajsyys+ajszzs
 cSSt=2.*(ajsx*ajsxt+ajsy*ajsyt+ajsz*ajszt)
 cSTt=2.*(ajsxt*ajtx+ajsx*ajtxt + ajsyt*ajty+ ajsy*ajtyt + ajszt*ajtz+ ajsz*ajtzt)
 ccSt=ajsxxt+ajsyyt+ajszzt
! m=3...
 cTTr=2.*(ajtx*ajtxr+ajty*ajtyr+ajtz*ajtzr)
 ccTr=ajtxxr+ajtyyr+ajtzzr
 cTTs=2.*(ajtx*ajtxs+ajty*ajtys+ajtz*ajtzs)
 ccTs=ajtxxs+ajtyys+ajtzzs
 cTTt=2.*(ajtx*ajtxt+ajty*ajtyt+ajtz*ajtzt)
 ccTt=ajtxxt+ajtyyt+ajtzzt
 c0r=0.
 c0s=0.
 c0t=0.
! =============== End: Laplace operator: ==================== 
! ---------------- Start: Boundary condition: --------------- 
! BC: a1*u.n + a0*u = g 
! nsign=2*side-1
! a1=1.
! a0=0.


 ! ---------------- Start r direction ---------------
#If #DIR eq "R"
 ! Outward normal : (n1,n2,n3) 
 ani=nsign/sqrt(ajrx**2+ajry**2+ajrz**2)
 n1=ajrx*ani
 n2=ajry*ani
 n3=ajrz*ani
 ! BC : anR*ur + anS*us + anT*ut + a0*u 
 anR=a1*(n1*ajrx+n2*ajry+n3*ajrz)
 anS=a1*(n1*ajsx+n2*ajsy+n3*ajsz)
 anT=a1*(n1*ajtx+n2*ajty+n3*ajtz)
! >>>>>>>
 anis=-(ajrx*ajrxs+ajry*ajrys+ajrz*ajrzs)*ani**3
 aniss=-(ajrx*ajrxss+ajry*ajryss+ajrz*ajrzss+ajrxs*ajrxs+ajrys*ajrys+ajrzs*ajrzs)*ani**3 -3.*(ajrx*ajrxs+ajry*ajrys+ajrz*ajrzs)*ani**2*anis
 n1s=ajrxs*ani + ajrx*anis
 n1ss=ajrxss*ani + 2.*ajrxs*anis + ajrx*aniss
 n2s=ajrys*ani + ajry*anis
 n2ss=ajryss*ani + 2.*ajrys*anis + ajry*aniss
 n3s=ajrzs*ani + ajrz*anis
 n3ss=ajrzss*ani + 2.*ajrzs*anis + ajrz*aniss

 anRs =a1*(n1*ajrxs+n2*ajrys+n3*ajrzs+n1s*ajrx+n2s*ajry+n3s*ajrz)
 anRss=a1*(n1*ajrxss+n2*ajryss+n3*ajrzss+2.*(n1s*ajrxs+n2s*ajrys+n3s*ajrzs)+n1ss*ajrx+n2ss*ajry+n3ss*ajrz)
 anSs =a1*(n1*ajsxs+n2*ajsys+n3*ajszs+n1s*ajsx+n2s*ajsy+n3s*ajsz)
 anSss=a1*(n1*ajsxss+n2*ajsyss+n3*ajszss+2.*(n1s*ajsxs+n2s*ajsys+n3s*ajszs)+n1ss*ajsx+n2ss*ajsy+n3ss*ajsz)
 anTs =a1*(n1*ajtxs+n2*ajtys+n3*ajtzs+n1s*ajtx+n2s*ajty+n3s*ajtz)
 anTss=a1*(n1*ajtxss+n2*ajtyss+n3*ajtzss+2.*(n1s*ajtxs+n2s*ajtys+n3s*ajtzs)+n1ss*ajtx+n2ss*ajty+n3ss*ajtz)
! <<<<<<<
! >>>>>>>
 anit=-(ajrx*ajrxt+ajry*ajryt+ajrz*ajrzt)*ani**3
 anitt=-(ajrx*ajrxtt+ajry*ajrytt+ajrz*ajrztt+ajrxt*ajrxt+ajryt*ajryt+ajrzt*ajrzt)*ani**3 -3.*(ajrx*ajrxt+ajry*ajryt+ajrz*ajrzt)*ani**2*anit
 anist=-(ajrx*ajrxst+ajry*ajryst+ajrz*ajrzst+ajrxs*ajrxt+ajrys*ajryt+ajrzs*ajrzt)*ani**3 -3.*(ajrx*ajrxs+ajry*ajrys+ajrz*ajrzs)*ani**2*anit
 n1t=ajrxt*ani + ajrx*anit
 n1tt=ajrxtt*ani + 2.*ajrxt*anit + ajrx*anitt
 n1st=ajrxst*ani + ajrxt*anis + ajrxs*anit + ajrx*anist
 n2t=ajryt*ani + ajry*anit
 n2tt=ajrytt*ani + 2.*ajryt*anit + ajry*anitt
 n2st=ajryst*ani + ajryt*anis + ajrys*anit + ajry*anist
 n3t=ajrzt*ani + ajrz*anit
 n3tt=ajrztt*ani + 2.*ajrzt*anit + ajrz*anitt
 n3st=ajrzst*ani + ajrzt*anis + ajrzs*anit + ajrz*anist

 anRt =a1*(n1*ajrxt+n2*ajryt+n3*ajrzt+n1t*ajrx+n2t*ajry+n3t*ajrz)
 anRtt=a1*(n1*ajrxtt+n2*ajrytt+n3*ajrztt+2.*(n1t*ajrxt+n2t*ajryt+n3t*ajrzt)+n1tt*ajrx+n2tt*ajry+n3tt*ajrz)
 anRst=a1*(n1*ajrxst+n2*ajryst+n3*ajrzst +n1s*ajrxt+n2s*ajryt+n3s*ajrzt +n1t*ajrxs+n2t*ajrys+n3t*ajrzs +n1st*ajrx+n2st*ajry+n3st*ajrz)
 anSt =a1*(n1*ajsxt+n2*ajsyt+n3*ajszt+n1t*ajsx+n2t*ajsy+n3t*ajsz)
 anStt=a1*(n1*ajsxtt+n2*ajsytt+n3*ajsztt+2.*(n1t*ajsxt+n2t*ajsyt+n3t*ajszt)+n1tt*ajsx+n2tt*ajsy+n3tt*ajsz)
 anSst=a1*(n1*ajsxst+n2*ajsyst+n3*ajszst +n1s*ajsxt+n2s*ajsyt+n3s*ajszt +n1t*ajsxs+n2t*ajsys+n3t*ajszs +n1st*ajsx+n2st*ajsy+n3st*ajsz)
 anTt =a1*(n1*ajtxt+n2*ajtyt+n3*ajtzt+n1t*ajtx+n2t*ajty+n3t*ajtz)
 anTtt=a1*(n1*ajtxtt+n2*ajtytt+n3*ajtztt+2.*(n1t*ajtxt+n2t*ajtyt+n3t*ajtzt)+n1tt*ajtx+n2tt*ajty+n3tt*ajtz)
 anTst=a1*(n1*ajtxst+n2*ajtyst+n3*ajtzst +n1s*ajtxt+n2s*ajtyt+n3s*ajtzt +n1t*ajtxs+n2t*ajtys+n3t*ajtzs +n1st*ajtx+n2st*ajty+n3st*ajtz)
! <<<<<<<
 ! Here are the expressions for the normal derivatives
 uur=(g-a0*uu-anS*uus-anT*uut)/anR
 uurs=(gs-a0*uus-a0s*uu-anS*uuss-anSs*uus-anT*uust-anTs*uut-anRs*uur)/anR
 uurss=(gss-a0*uuss-2*a0s*uus-a0ss*uu-anS*uusss-2*anSs*uuss-anSss*uus-anT*uusst-2*anTs*uust-anTss*uut-2*anRs*uurs-anRss*uur)/anR
 uurt=(gt-a0*uut-a0t*uu-anS*uust-anSt*uus-anT*uutt-anTt*uut-anRt*uur)/anR
 uurtt=(gtt-a0*uutt-2*a0t*uut-a0tt*uu-anS*uustt-2*anSt*uust-anStt*uus-anT*uuttt-2*anTt*uutt-anTtt*uut-2*anRt*uurt-anRtt*uur)/anR
 uurst=(gst-a0*uust-a0s*uut-a0t*uus-anS*uusst-anSt*uuss-anSs*uust-anSst*uus-anT*uustt-anTt*uust-anTs*uutt-anTst*uut-anRs*uurt-anRt*uurs-anRst*uur)/anR
 uurr=(ff-cSS*uuss-cTT*uutt-cRS*uurs-cRT*uurt-cST*uust-ccR*uur-ccS*uus-ccT*uut-c0*uu)/cRR
 uurrs=(ffs-cSS*uusss-cTT*uustt-cRS*uurss-cRT*uurst-cST*uusst-ccR*uurs-ccS*uuss-ccT*uust-c0*uus-cSSs*uuss-cTTs*uutt-cRSs*uurs-cRTs*uurt-cSTs*uust-ccRs*uur-ccSs*uus-ccTs*uut-c0s*uu-cRRs*uurr)/cRR
 uurrt=(fft-cSS*uusst-cTT*uuttt-cRS*uurst-cRT*uurtt-cST*uustt-ccR*uurt-ccS*uust-ccT*uutt-c0*uut-cSSt*uuss-cTTt*uutt-cRSt*uurs-cRTt*uurt-cSTt*uust-ccRt*uur-ccSt*uus-ccTt*uut-c0t*uu-cRRt*uurr)/cRR

 unnn2=(ffr-cSS*uurss-cTT*uurtt-cRS*uurrs-cRT*uurrt-cST*uurst-ccR*uurr-ccS*uurs-ccT*uurt-c0*uur-cSSr*uuss-cTTr*uutt-cRSr*uurs-cRTr*uurt-cSTr*uust-ccRr*uur-ccSr*uus-ccTr*uut-c0r*uu-cRRr*uurr)/cRR
 un4=(g-a0*uu-anS*uus-anT*uut)/anR

dn=dr(axis)
 u(i1-is1,i2-is2,i3-is3)= u(i1+is1,i2+is2,i3+is3) +nsign*(2.*dn)*( un4 + (dn**2/6.)*unnn2 )
 u(i1-2*is1,i2-2*is2,i3-2*is3)= u(i1+2*is1,i2+2*is2,i3+2*is3) +nsign*(4.*dn)*( un4 + (2.*dn**2/3.)*unnn2 )
#End

 ! ---------------- Start s direction ---------------
#If #DIR eq "S"
 ! Outward normal : (n1,n2,n3) 
 ani=nsign/sqrt(ajsx**2+ajsy**2+ajsz**2)
 n1=ajsx*ani
 n2=ajsy*ani
 n3=ajsz*ani
 ! BC : anS*us + anT*ut + anR*ur + a0*u 
 anS=a1*(n1*ajsx+n2*ajsy+n3*ajsz)
 anT=a1*(n1*ajtx+n2*ajty+n3*ajtz)
 anR=a1*(n1*ajrx+n2*ajry+n3*ajrz)
! >>>>>>>
 anit=-(ajsx*ajsxt+ajsy*ajsyt+ajsz*ajszt)*ani**3
 anitt=-(ajsx*ajsxtt+ajsy*ajsytt+ajsz*ajsztt+ajsxt*ajsxt+ajsyt*ajsyt+ajszt*ajszt)*ani**3 -3.*(ajsx*ajsxt+ajsy*ajsyt+ajsz*ajszt)*ani**2*anit
 n1t=ajsxt*ani + ajsx*anit
 n1tt=ajsxtt*ani + 2.*ajsxt*anit + ajsx*anitt
 n2t=ajsyt*ani + ajsy*anit
 n2tt=ajsytt*ani + 2.*ajsyt*anit + ajsy*anitt
 n3t=ajszt*ani + ajsz*anit
 n3tt=ajsztt*ani + 2.*ajszt*anit + ajsz*anitt

 anSt =a1*(n1*ajsxt+n2*ajsyt+n3*ajszt+n1t*ajsx+n2t*ajsy+n3t*ajsz)
 anStt=a1*(n1*ajsxtt+n2*ajsytt+n3*ajsztt+2.*(n1t*ajsxt+n2t*ajsyt+n3t*ajszt)+n1tt*ajsx+n2tt*ajsy+n3tt*ajsz)
 anTt =a1*(n1*ajtxt+n2*ajtyt+n3*ajtzt+n1t*ajtx+n2t*ajty+n3t*ajtz)
 anTtt=a1*(n1*ajtxtt+n2*ajtytt+n3*ajtztt+2.*(n1t*ajtxt+n2t*ajtyt+n3t*ajtzt)+n1tt*ajtx+n2tt*ajty+n3tt*ajtz)
 anRt =a1*(n1*ajrxt+n2*ajryt+n3*ajrzt+n1t*ajrx+n2t*ajry+n3t*ajrz)
 anRtt=a1*(n1*ajrxtt+n2*ajrytt+n3*ajrztt+2.*(n1t*ajrxt+n2t*ajryt+n3t*ajrzt)+n1tt*ajrx+n2tt*ajry+n3tt*ajrz)
! <<<<<<<
! >>>>>>>
 anir=-(ajsx*ajsxr+ajsy*ajsyr+ajsz*ajszr)*ani**3
 anirr=-(ajsx*ajsxrr+ajsy*ajsyrr+ajsz*ajszrr+ajsxr*ajsxr+ajsyr*ajsyr+ajszr*ajszr)*ani**3 -3.*(ajsx*ajsxr+ajsy*ajsyr+ajsz*ajszr)*ani**2*anir
 anirt=-(ajsx*ajsxrt+ajsy*ajsyrt+ajsz*ajszrt+ajsxt*ajsxr+ajsyt*ajsyr+ajszt*ajszr)*ani**3 -3.*(ajsx*ajsxt+ajsy*ajsyt+ajsz*ajszt)*ani**2*anir
 n1r=ajsxr*ani + ajsx*anir
 n1rr=ajsxrr*ani + 2.*ajsxr*anir + ajsx*anirr
 n1rt=ajsxrt*ani + ajsxr*anit + ajsxt*anir + ajsx*anirt
 n2r=ajsyr*ani + ajsy*anir
 n2rr=ajsyrr*ani + 2.*ajsyr*anir + ajsy*anirr
 n2rt=ajsyrt*ani + ajsyr*anit + ajsyt*anir + ajsy*anirt
 n3r=ajszr*ani + ajsz*anir
 n3rr=ajszrr*ani + 2.*ajszr*anir + ajsz*anirr
 n3rt=ajszrt*ani + ajszr*anit + ajszt*anir + ajsz*anirt

 anSr =a1*(n1*ajsxr+n2*ajsyr+n3*ajszr+n1r*ajsx+n2r*ajsy+n3r*ajsz)
 anSrr=a1*(n1*ajsxrr+n2*ajsyrr+n3*ajszrr+2.*(n1r*ajsxr+n2r*ajsyr+n3r*ajszr)+n1rr*ajsx+n2rr*ajsy+n3rr*ajsz)
 anSrt=a1*(n1*ajsxrt+n2*ajsyrt+n3*ajszrt +n1t*ajsxr+n2t*ajsyr+n3t*ajszr +n1r*ajsxt+n2r*ajsyt+n3r*ajszt +n1rt*ajsx+n2rt*ajsy+n3rt*ajsz)
 anTr =a1*(n1*ajtxr+n2*ajtyr+n3*ajtzr+n1r*ajtx+n2r*ajty+n3r*ajtz)
 anTrr=a1*(n1*ajtxrr+n2*ajtyrr+n3*ajtzrr+2.*(n1r*ajtxr+n2r*ajtyr+n3r*ajtzr)+n1rr*ajtx+n2rr*ajty+n3rr*ajtz)
 anTrt=a1*(n1*ajtxrt+n2*ajtyrt+n3*ajtzrt +n1t*ajtxr+n2t*ajtyr+n3t*ajtzr +n1r*ajtxt+n2r*ajtyt+n3r*ajtzt +n1rt*ajtx+n2rt*ajty+n3rt*ajtz)
 anRr =a1*(n1*ajrxr+n2*ajryr+n3*ajrzr+n1r*ajrx+n2r*ajry+n3r*ajrz)
 anRrr=a1*(n1*ajrxrr+n2*ajryrr+n3*ajrzrr+2.*(n1r*ajrxr+n2r*ajryr+n3r*ajrzr)+n1rr*ajrx+n2rr*ajry+n3rr*ajrz)
 anRrt=a1*(n1*ajrxrt+n2*ajryrt+n3*ajrzrt +n1t*ajrxr+n2t*ajryr+n3t*ajrzr +n1r*ajrxt+n2r*ajryt+n3r*ajrzt +n1rt*ajrx+n2rt*ajry+n3rt*ajrz)
! <<<<<<<
 ! Here are the expressions for the normal derivatives
 uus=(g-a0*uu-anT*uut-anR*uur)/anS
 uust=(gt-a0*uut-a0t*uu-anT*uutt-anTt*uut-anR*uurt-anRt*uur-anSt*uus)/anS
 uustt=(gtt-a0*uutt-2*a0t*uut-a0tt*uu-anT*uuttt-2*anTt*uutt-anTtt*uut-anR*uurtt-2*anRt*uurt-anRtt*uur-2*anSt*uust-anStt*uus)/anS
 uurs=(gr-a0*uur-a0r*uu-anT*uurt-anTr*uut-anR*uurr-anRr*uur-anSr*uus)/anS
 uurrs=(grr-a0*uurr-2*a0r*uur-a0rr*uu-anT*uurrt-2*anTr*uurt-anTrr*uut-anR*uurrr-2*anRr*uurr-anRrr*uur-2*anSr*uurs-anSrr*uus)/anS
 uurst=(grt-a0*uurt-a0t*uur-a0r*uut-anT*uurtt-anTr*uutt-anTt*uurt-anTrt*uut-anR*uurrt-anRr*uurt-anRt*uurr-anRrt*uur-anSt*uurs-anSr*uust-anSrt*uus)/anS
 uuss=(ff-cTT*uutt-cRR*uurr-cST*uust-cRS*uurs-cRT*uurt-ccS*uus-ccT*uut-ccR*uur-c0*uu)/cSS
 uusst=(fft-cTT*uuttt-cRR*uurrt-cST*uustt-cRS*uurst-cRT*uurtt-ccS*uust-ccT*uutt-ccR*uurt-c0*uut-cTTt*uutt-cRRt*uurr-cSTt*uust-cRSt*uurs-cRTt*uurt-ccSt*uus-ccTt*uut-ccRt*uur-c0t*uu-cSSt*uuss)/cSS
 uurss=(ffr-cTT*uurtt-cRR*uurrr-cST*uurst-cRS*uurrs-cRT*uurrt-ccS*uurs-ccT*uurt-ccR*uurr-c0*uur-cTTr*uutt-cRRr*uurr-cSTr*uust-cRSr*uurs-cRTr*uurt-ccSr*uus-ccTr*uut-ccRr*uur-c0r*uu-cSSr*uuss)/cSS

 unnn2=(ffs-cTT*uustt-cRR*uurrs-cST*uusst-cRS*uurss-cRT*uurst-ccS*uuss-ccT*uust-ccR*uurs-c0*uus-cTTs*uutt-cRRs*uurr-cSTs*uust-cRSs*uurs-cRTs*uurt-ccSs*uus-ccTs*uut-ccRs*uur-c0s*uu-cSSs*uuss)/cSS
 un4=(g-a0*uu-anT*uut-anR*uur)/anS

dn=dr(axis)
 u(i1-is1,i2-is2,i3-is3)= u(i1+is1,i2+is2,i3+is3) +nsign*(2.*dn)*( un4 + (dn**2/6.)*unnn2 )
 u(i1-2*is1,i2-2*is2,i3-2*is3)= u(i1+2*is1,i2+2*is2,i3+2*is3) +nsign*(4.*dn)*( un4 + (2.*dn**2/3.)*unnn2 )
#End

 ! ---------------- Start t direction ---------------
#If #DIR eq "T"
 ! Outward normal : (n1,n2,n3) 
 ani=nsign/sqrt(ajtx**2+ajty**2+ajtz**2)
 n1=ajtx*ani
 n2=ajty*ani
 n3=ajtz*ani
 ! BC : anT*ut + anR*ur + anS*us + a0*u 
 anT=a1*(n1*ajtx+n2*ajty+n3*ajtz)
 anR=a1*(n1*ajrx+n2*ajry+n3*ajrz)
 anS=a1*(n1*ajsx+n2*ajsy+n3*ajsz)
! >>>>>>>
 anir=-(ajtx*ajtxr+ajty*ajtyr+ajtz*ajtzr)*ani**3
 anirr=-(ajtx*ajtxrr+ajty*ajtyrr+ajtz*ajtzrr+ajtxr*ajtxr+ajtyr*ajtyr+ajtzr*ajtzr)*ani**3 -3.*(ajtx*ajtxr+ajty*ajtyr+ajtz*ajtzr)*ani**2*anir
 n1r=ajtxr*ani + ajtx*anir
 n1rr=ajtxrr*ani + 2.*ajtxr*anir + ajtx*anirr
 n2r=ajtyr*ani + ajty*anir
 n2rr=ajtyrr*ani + 2.*ajtyr*anir + ajty*anirr
 n3r=ajtzr*ani + ajtz*anir
 n3rr=ajtzrr*ani + 2.*ajtzr*anir + ajtz*anirr

 anTr =a1*(n1*ajtxr+n2*ajtyr+n3*ajtzr+n1r*ajtx+n2r*ajty+n3r*ajtz)
 anTrr=a1*(n1*ajtxrr+n2*ajtyrr+n3*ajtzrr+2.*(n1r*ajtxr+n2r*ajtyr+n3r*ajtzr)+n1rr*ajtx+n2rr*ajty+n3rr*ajtz)
 anRr =a1*(n1*ajrxr+n2*ajryr+n3*ajrzr+n1r*ajrx+n2r*ajry+n3r*ajrz)
 anRrr=a1*(n1*ajrxrr+n2*ajryrr+n3*ajrzrr+2.*(n1r*ajrxr+n2r*ajryr+n3r*ajrzr)+n1rr*ajrx+n2rr*ajry+n3rr*ajrz)
 anSr =a1*(n1*ajsxr+n2*ajsyr+n3*ajszr+n1r*ajsx+n2r*ajsy+n3r*ajsz)
 anSrr=a1*(n1*ajsxrr+n2*ajsyrr+n3*ajszrr+2.*(n1r*ajsxr+n2r*ajsyr+n3r*ajszr)+n1rr*ajsx+n2rr*ajsy+n3rr*ajsz)
! <<<<<<<
! >>>>>>>
 anis=-(ajtx*ajtxs+ajty*ajtys+ajtz*ajtzs)*ani**3
 aniss=-(ajtx*ajtxss+ajty*ajtyss+ajtz*ajtzss+ajtxs*ajtxs+ajtys*ajtys+ajtzs*ajtzs)*ani**3 -3.*(ajtx*ajtxs+ajty*ajtys+ajtz*ajtzs)*ani**2*anis
 anirs=-(ajtx*ajtxrs+ajty*ajtyrs+ajtz*ajtzrs+ajtxr*ajtxs+ajtyr*ajtys+ajtzr*ajtzs)*ani**3 -3.*(ajtx*ajtxr+ajty*ajtyr+ajtz*ajtzr)*ani**2*anis
 n1s=ajtxs*ani + ajtx*anis
 n1ss=ajtxss*ani + 2.*ajtxs*anis + ajtx*aniss
 n1rs=ajtxrs*ani + ajtxs*anir + ajtxr*anis + ajtx*anirs
 n2s=ajtys*ani + ajty*anis
 n2ss=ajtyss*ani + 2.*ajtys*anis + ajty*aniss
 n2rs=ajtyrs*ani + ajtys*anir + ajtyr*anis + ajty*anirs
 n3s=ajtzs*ani + ajtz*anis
 n3ss=ajtzss*ani + 2.*ajtzs*anis + ajtz*aniss
 n3rs=ajtzrs*ani + ajtzs*anir + ajtzr*anis + ajtz*anirs

 anTs =a1*(n1*ajtxs+n2*ajtys+n3*ajtzs+n1s*ajtx+n2s*ajty+n3s*ajtz)
 anTss=a1*(n1*ajtxss+n2*ajtyss+n3*ajtzss+2.*(n1s*ajtxs+n2s*ajtys+n3s*ajtzs)+n1ss*ajtx+n2ss*ajty+n3ss*ajtz)
 anTrs=a1*(n1*ajtxrs+n2*ajtyrs+n3*ajtzrs +n1r*ajtxs+n2r*ajtys+n3r*ajtzs +n1s*ajtxr+n2s*ajtyr+n3s*ajtzr +n1rs*ajtx+n2rs*ajty+n3rs*ajtz)
 anRs =a1*(n1*ajrxs+n2*ajrys+n3*ajrzs+n1s*ajrx+n2s*ajry+n3s*ajrz)
 anRss=a1*(n1*ajrxss+n2*ajryss+n3*ajrzss+2.*(n1s*ajrxs+n2s*ajrys+n3s*ajrzs)+n1ss*ajrx+n2ss*ajry+n3ss*ajrz)
 anRrs=a1*(n1*ajrxrs+n2*ajryrs+n3*ajrzrs +n1r*ajrxs+n2r*ajrys+n3r*ajrzs +n1s*ajrxr+n2s*ajryr+n3s*ajrzr +n1rs*ajrx+n2rs*ajry+n3rs*ajrz)
 anSs =a1*(n1*ajsxs+n2*ajsys+n3*ajszs+n1s*ajsx+n2s*ajsy+n3s*ajsz)
 anSss=a1*(n1*ajsxss+n2*ajsyss+n3*ajszss+2.*(n1s*ajsxs+n2s*ajsys+n3s*ajszs)+n1ss*ajsx+n2ss*ajsy+n3ss*ajsz)
 anSrs=a1*(n1*ajsxrs+n2*ajsyrs+n3*ajszrs +n1r*ajsxs+n2r*ajsys+n3r*ajszs +n1s*ajsxr+n2s*ajsyr+n3s*ajszr +n1rs*ajsx+n2rs*ajsy+n3rs*ajsz)
! <<<<<<<
 ! Here are the expressions for the normal derivatives
 uut=(g-a0*uu-anR*uur-anS*uus)/anT
 uurt=(gr-a0*uur-a0r*uu-anR*uurr-anRr*uur-anS*uurs-anSr*uus-anTr*uut)/anT
 uurrt=(grr-a0*uurr-2*a0r*uur-a0rr*uu-anR*uurrr-2*anRr*uurr-anRrr*uur-anS*uurrs-2*anSr*uurs-anSrr*uus-2*anTr*uurt-anTrr*uut)/anT
 uust=(gs-a0*uus-a0s*uu-anR*uurs-anRs*uur-anS*uuss-anSs*uus-anTs*uut)/anT
 uusst=(gss-a0*uuss-2*a0s*uus-a0ss*uu-anR*uurss-2*anRs*uurs-anRss*uur-anS*uusss-2*anSs*uuss-anSss*uus-2*anTs*uust-anTss*uut)/anT
 uurst=(grs-a0*uurs-a0r*uus-a0s*uur-anR*uurrs-anRs*uurr-anRr*uurs-anRrs*uur-anS*uurss-anSs*uurs-anSr*uuss-anSrs*uus-anTr*uust-anTs*uurt-anTrs*uut)/anT
 uutt=(ff-cRR*uurr-cSS*uuss-cRT*uurt-cST*uust-cRS*uurs-ccT*uut-ccR*uur-ccS*uus-c0*uu)/cTT
 uurtt=(ffr-cRR*uurrr-cSS*uurss-cRT*uurrt-cST*uurst-cRS*uurrs-ccT*uurt-ccR*uurr-ccS*uurs-c0*uur-cRRr*uurr-cSSr*uuss-cRTr*uurt-cSTr*uust-cRSr*uurs-ccTr*uut-ccRr*uur-ccSr*uus-c0r*uu-cTTr*uutt)/cTT
 uustt=(ffs-cRR*uurrs-cSS*uusss-cRT*uurst-cST*uusst-cRS*uurss-ccT*uust-ccR*uurs-ccS*uuss-c0*uus-cRRs*uurr-cSSs*uuss-cRTs*uurt-cSTs*uust-cRSs*uurs-ccTs*uut-ccRs*uur-ccSs*uus-c0s*uu-cTTs*uutt)/cTT

 unnn2=(fft-cRR*uurrt-cSS*uusst-cRT*uurtt-cST*uustt-cRS*uurst-ccT*uutt-ccR*uurt-ccS*uust-c0*uut-cRRt*uurr-cSSt*uuss-cRTt*uurt-cSTt*uust-cRSt*uurs-ccTt*uut-ccRt*uur-ccSt*uus-c0t*uu-cTTt*uutt)/cTT
 un4=(g-a0*uu-anR*uur-anS*uus)/anT

dn=dr(axis)
 u(i1-is1,i2-is2,i3-is3)= u(i1+is1,i2+is2,i3+is3) +nsign*(2.*dn)*( un4 + (dn**2/6.)*unnn2 )
 u(i1-2*is1,i2-2*is2,i3-2*is3)= u(i1+2*is1,i2+2*is2,i3+2*is3) +nsign*(4.*dn)*( un4 + (2.*dn**2/3.)*unnn2 )
#End
#endMacro
