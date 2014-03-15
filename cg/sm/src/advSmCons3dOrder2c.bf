#beginMacro setupMaterialAndMetric()
Jip=Jac(ip,j,k)
J0=Jac(i,j,k)
Jim=Jac(im,j,k)
q1ip=qx(ip,j,k)
q10=qx(i,j,k)
q1im=qx(im,j,k)
q2ip=qy(ip,j,k)
q20=qy(i,j,k)
q2im=qy(im,j,k)
q3ip=qz(ip,j,k)
q30=qz(i,j,k)
q3im=qz(im,j,k)
r1ip=rx(ip,j,k)
r10=rx(i,j,k)
r1im=rx(im,j,k)
r2ip=ry(ip,j,k)
r20=ry(i,j,k)
r2im=ry(im,j,k)
r3ip=rz(ip,j,k)
r30=rz(i,j,k)
r3im=rz(im,j,k)
s1ip=sx(ip,j,k)
s10=sx(i,j,k)
s1im=sx(im,j,k)
s2ip=sy(ip,j,k)
s20=sy(i,j,k)
s2im=sy(im,j,k)
s3ip=sz(ip,j,k)
s30=sz(i,j,k)
s3im=sz(im,j,k)
l2mip=lam2mu(ip,j,k)
l2m0=lam2mu(i,j,k)
l2mim=lam2mu(im,j,k)
lip=lam(ip,j,k)
l0=lam(i,j,k)
lim=lam(im,j,k)
mip=mu(ip,j,k)
m0=mu(i,j,k)
mim=mu(im,j,k)
Jjp=Jac(i,jp,k)
Jjm=Jac(i,jm,k)
q1jp=qx(i,jp,k)
q1jm=qx(i,jm,k)
q2jp=qy(i,jp,k)
q2jm=qy(i,jm,k)
q3jp=qz(i,jp,k)
q3jm=qz(i,jm,k)
r1jp=rx(i,jp,k)
r1jm=rx(i,jm,k)
r2jp=ry(i,jp,k)
r2jm=ry(i,jm,k)
r3jp=rz(i,jp,k)
r3jm=rz(i,jm,k)
s1jp=sx(i,jp,k)
s1jm=sx(i,jm,k)
s2jp=sy(i,jp,k)
s2jm=sy(i,jm,k)
s3jp=sz(i,jp,k)
s3jm=sz(i,jm,k)
l2mjp=lam2mu(i,jp,k)
l2mjm=lam2mu(i,jm,k)
ljp=lam(i,jp,k)
ljm=lam(i,jm,k)
mjp=mu(i,jp,k)
mjm=mu(i,jm,k)
Jkp=Jac(i,j,kp)
Jkm=Jac(i,j,km)
q1kp=qx(i,j,kp)
q1km=qx(i,j,km)
q2kp=qy(i,j,kp)
q2km=qy(i,j,km)
q3kp=qz(i,j,kp)
q3km=qz(i,j,km)
r1kp=rx(i,j,kp)
r1km=rx(i,j,km)
r2kp=ry(i,j,kp)
r2km=ry(i,j,km)
r3kp=rz(i,j,kp)
r3km=rz(i,j,km)
s1kp=sx(i,j,kp)
s1km=sx(i,j,km)
s2kp=sy(i,j,kp)
s2km=sy(i,j,km)
s3kp=sz(i,j,kp)
s3km=sz(i,j,km)
l2mkp=lam2mu(i,j,kp)
l2mkm=lam2mu(i,j,km)
lkp=lam(i,j,kp)
lkm=lam(i,j,km)
mkp=mu(i,j,kp)
mkm=mu(i,j,km)
#endMacro

#beginMacro setupU()
u0=u(i,j,k,uc)
uipjp=u(ip,jp,k,uc)
uipjm=u(ip,jm,k,uc)
uimjp=u(im,jp,k,uc)
uimjm=u(im,jm,k,uc)
uipkp=u(ip,j,kp,uc)
uipkm=u(ip,j,km,uc)
uimkp=u(im,j,kp,uc)
uimkm=u(im,j,km,uc)
ujpkp=u(i,jp,kp,uc)
ujpkm=u(i,jp,km,uc)
ujmkp=u(i,jm,kp,uc)
ujmkm=u(i,jm,km,uc)
duip=dri(0)*(u(ip,j,k,uc)-u0)
duim=dri(0)*(u0-u(im,j,k,uc))
dujp=dri(1)*(u(i,jp,k,uc)-u0)
dujm=dri(1)*(u0-u(i,jm,k,uc))
dukp=dri(2)*(u(i,j,kp,uc)-u0)
dukm=dri(2)*(u0-u(i,j,km,uc))
#endMacro

#beginMacro setupV()
u0=u(i,j,k,vc)
uipjp=u(ip,jp,k,vc)
uipjm=u(ip,jm,k,vc)
uimjp=u(im,jp,k,vc)
uimjm=u(im,jm,k,vc)
uipkp=u(ip,j,kp,vc)
uipkm=u(ip,j,km,vc)
uimkp=u(im,j,kp,vc)
uimkm=u(im,j,km,vc)
ujpkp=u(i,jp,kp,vc)
ujpkm=u(i,jp,km,vc)
ujmkp=u(i,jm,kp,vc)
ujmkm=u(i,jm,km,vc)
duip=dri(0)*(u(ip,j,k,vc)-u0)
duim=dri(0)*(u0-u(im,j,k,vc))
dujp=dri(1)*(u(i,jp,k,vc)-u0)
dujm=dri(1)*(u0-u(i,jm,k,vc))
dukp=dri(2)*(u(i,j,kp,vc)-u0)
dukm=dri(2)*(u0-u(i,j,km,vc))
#endMacro

#beginMacro setupW()
u0=u(i,j,k,wc) 
uipjp=u(ip,jp,k,wc)
uipjm=u(ip,jm,k,wc)
uimjp=u(im,jp,k,wc)
uimjm=u(im,jm,k,wc)
uipkp=u(ip,j,kp,wc)
uipkm=u(ip,j,km,wc)
uimkp=u(im,j,kp,wc)
uimkm=u(im,j,km,wc)
ujpkp=u(i,jp,kp,wc)
ujpkm=u(i,jp,km,wc)
ujmkp=u(i,jm,kp,wc)
ujmkm=u(i,jm,km,wc)
duip=dri(0)*(u(ip,j,k,wc)-u0)
duim=dri(0)*(u0-u(im,j,k,wc))
dujp=dri(1)*(u(i,jp,k,wc)-u0)
dujm=dri(1)*(u0-u(i,jm,k,wc))
dukp=dri(2)*(u(i,j,kp,wc)-u0)
dukm=dri(2)*(u0-u(i,j,km,wc))
#endMacro

#beginMacro addUCrossterms()
! // u terms in u eq.
rhtmpu = rhtmpu + fq*fr*(Jip*q1ip*r1ip*l2mip*(uipjp-uipjm)-Jim*q1im*r1im*l2mim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jip*q2ip*r2ip*mip*(uipjp-uipjm)-Jim*q2im*r2im*mim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jip*q3ip*r3ip*mip*(uipjp-uipjm)-Jim*q3im*r3im*mim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fs*(Jip*q1ip*s1ip*l2mip*(uipkp-uipkm)-Jim*q1im*s1im*l2mim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fs*(Jip*q2ip*s2ip*mip*(uipkp-uipkm)-Jim*q2im*s2im*mim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fs*(Jip*q3ip*s3ip*mip*(uipkp-uipkm)-Jim*q3im*s3im*mim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r1jp*q1jp*l2mjp*(uipjp-uimjp)-Jjm*r1jm*q1jm*l2mjm*(uipjm-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r2jp*q2jp*mjp*(uipjp-uimjp)-Jjm*r2jm*q2jm*mjm*(uipjm-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r3jp*q3jp*mjp*(uipjp-uimjp)-Jjm*r3jm*q3jm*mjm*(uipjm-uimjm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r1jp*s1jp*l2mjp*(ujpkp-ujpkm)-Jjm*r1jm*s1jm*l2mjm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r2jp*s2jp*mjp*(ujpkp-ujpkm)-Jjm*r2jm*s2jm*mjm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r3jp*s3jp*mjp*(ujpkp-ujpkm)-Jjm*r3jm*s3jm*mjm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fq*fs*(Jkp*s1kp*q1kp*l2mkp*(uipkp-uimkp)-Jkm*s1km*q1km*l2mkm*(uipkm-uimkm))
rhtmpu = rhtmpu + fq*fs*(Jkp*s2kp*q2kp*mkp*(uipkp-uimkp)-Jkm*s2km*q2km*mkm*(uipkm-uimkm))
rhtmpu = rhtmpu + fq*fs*(Jkp*s3kp*q3kp*mkp*(uipkp-uimkp)-Jkm*s3km*q3km*mkm*(uipkm-uimkm))
rhtmpu = rhtmpu + fr*fs*(Jkp*s1kp*r1kp*l2mkp*(ujpkp-ujmkp)-Jkm*s1km*r1km*l2mkm*(ujpkm-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jkp*s2kp*r2kp*mkp*(ujpkp-ujmkp)-Jkm*s2km*r2km*mkm*(ujpkm-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jkp*s3kp*r3kp*mkp*(ujpkp-ujmkp)-Jkm*s3km*r3km*mkm*(ujpkm-ujmkm))
!// u terms in v eq. 
rhtmpv = rhtmpv + fq*fr*(Jip*q1ip*r2ip*mip*(uipjp-uipjm)-Jim*q1im*r2im*mim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jip*q2ip*r1ip*lip*(uipjp-uipjm)-Jim*q2im*r1im*lim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fs*(Jip*q1ip*s2ip*mip*(uipkp-uipkm)-Jim*q1im*s2im*mim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fs*(Jip*q2ip*s1ip*lip*(uipkp-uipkm)-Jim*q2im*s1im*lim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r1jp*q2jp*mjp*(uipjp-uimjp)-Jjm*r1jm*q2jm*mjm*(uipjm-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r2jp*q1jp*ljp*(uipjp-uimjp)-Jjm*r2jm*q1jm*ljm*(uipjm-uimjm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r1jp*s2jp*mjp*(ujpkp-ujpkm)-Jjm*r1jm*s2jm*mjm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r2jp*s1jp*ljp*(ujpkp-ujpkm)-Jjm*r2jm*s1jm*ljm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fq*fs*(Jkp*s1kp*q2kp*mkp*(uipkp-uimkp)-Jkm*s1km*q2km*mkm*(uipkm-uimkm))	
rhtmpv = rhtmpv + fq*fs*(Jkp*s2kp*q1kp*lkp*(uipkp-uimkp)-Jkm*s2km*q1km*lkm*(uipkm-uimkm))	
rhtmpv = rhtmpv + fr*fs*(Jkp*s1kp*r2kp*mkp*(ujpkp-ujmkp)-Jkm*s1km*r2km*mkm*(ujpkm-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jkp*s2kp*r1kp*lkp*(ujpkp-ujmkp)-Jkm*s2km*r1km*lkm*(ujpkm-ujmkm))
!// u terms in w eq.
rhtmpw = rhtmpw + fq*fr*(Jip*q1ip*r3ip*mip*(uipjp-uipjm)-Jim*q1im*r3im*mim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jip*q3ip*r1ip*lip*(uipjp-uipjm)-Jim*q3im*r1im*lim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fs*(Jip*q1ip*s3ip*mip*(uipkp-uipkm)-Jim*q1im*s3im*mim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fs*(Jip*q3ip*s1ip*lip*(uipkp-uipkm)-Jim*q3im*s1im*lim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r1jp*q3jp*mjp*(uipjp-uimjp)-Jjm*r1jm*q3jm*mjm*(uipjm-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r3jp*q1jp*ljp*(uipjp-uimjp)-Jjm*r3jm*q1jm*ljm*(uipjm-uimjm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r1jp*s3jp*mjp*(ujpkp-ujpkm)-Jjm*r1jm*s3jm*mjm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r3jp*s1jp*ljp*(ujpkp-ujpkm)-Jjm*r3jm*s1jm*ljm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fq*fs*(Jkp*s1kp*q3kp*mkp*(uipkp-uimkp)-Jkm*s1km*q3km*mkm*(uipkm-uimkm))	
rhtmpw = rhtmpw + fq*fs*(Jkp*s3kp*q1kp*lkp*(uipkp-uimkp)-Jkm*s3km*q1km*lkm*(uipkm-uimkm))	
rhtmpw = rhtmpw + fr*fs*(Jkp*s1kp*r3kp*mkp*(ujpkp-ujmkp)-Jkm*s1km*r3km*mkm*(ujpkm-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jkp*s3kp*r1kp*lkp*(ujpkp-ujmkp)-Jkm*s3km*r1km*lkm*(ujpkm-ujmkm))
#endMacro

#beginMacro addUterms()
! u terms in u eq.
rhtmpu = rhtmpu +  0.5*dri(0)*(Jip*q1ip*q1ip*l2mip*duip-Jim*q1im*q1im*l2mim*duim+J0*q10*q10*l2m0*(duip-duim))
rhtmpu = rhtmpu +  0.5*dri(0)*(Jip*q2ip*q2ip*mip*duip-Jim*q2im*q2im*mim*duim+J0*q20*q20*m0*(duip-duim))
rhtmpu = rhtmpu +  0.5*dri(0)*(Jip*q3ip*q3ip*mip*duip-Jim*q3im*q3im*mim*duim+J0*q30*q30*m0*(duip-duim))
rhtmpu = rhtmpu +  0.5*dri(1)*(Jjp*r1jp*r1jp*l2mjp*dujp-Jjm*r1jm*r1jm*l2mjm*dujm+J0*r10*r10*l2m0*(dujp-dujm))
rhtmpu = rhtmpu +  0.5*dri(1)*(Jjp*r2jp*r2jp*mjp*dujp-Jjm*r2jm*r2jm*mjm*dujm+J0*r20*r20*m0*(dujp-dujm))
rhtmpu = rhtmpu +  0.5*dri(1)*(Jjp*r3jp*r3jp*mjp*dujp-Jjm*r3jm*r3jm*mjm*dujm+J0*r30*r30*m0*(dujp-dujm))
rhtmpu = rhtmpu +  0.5*dri(2)*(Jkp*s1kp*s1kp*l2mkp*dukp-Jkm*s1km*s1km*l2mkm*dukm+J0*s10*s10*l2m0*(dukp-dukm))
rhtmpu = rhtmpu +  0.5*dri(2)*(Jkp*s2kp*s2kp*mkp*dukp-Jkm*s2km*s2km*mkm*dukm+J0*s20*s20*m0*(dukp-dukm))
rhtmpu = rhtmpu +  0.5*dri(2)*(Jkp*s3kp*s3kp*mkp*dukp-Jkm*s3km*s3km*mkm*dukm+J0*s30*s30*m0*(dukp-dukm))
! u terms in v eq. 
rhtmpv = rhtmpv +  0.5*dri(0)*(Jip*q1ip*q2ip*(lip+mip)*duip-Jim*q1im*q2im*(lim+mim)*duim+J0*q10*q20*(l0+m0)*(duip-duim))
rhtmpv = rhtmpv +  0.5*dri(1)*(Jjp*r1jp*r2jp*(ljp+mjp)*dujp-Jjm*r1jm*r2jm*(ljm+mjm)*dujm+J0*r10*r20*(l0+m0)*(dujp-dujm))
rhtmpv = rhtmpv +  0.5*dri(2)*(Jkp*s1kp*s2kp*(lkp+mkp)*dukp-Jkm*s1km*s2km*(lkm+mkm)*dukm+J0*s10*s20*(l0+m0)*(dukp-dukm))
! u terms in w eq.
rhtmpw = rhtmpw +  0.5*dri(0)*(Jip*q1ip*q3ip*(lip+mip)*duip-Jim*q1im*q3im*(lim+mim)*duim+J0*q10*q30*(l0+m0)*(duip-duim))
rhtmpw = rhtmpw +  0.5*dri(1)*(Jjp*r1jp*r3jp*(ljp+mjp)*dujp-Jjm*r1jm*r3jm*(ljm+mjm)*dujm+J0*r10*r30*(l0+m0)*(dujp-dujm))
rhtmpw = rhtmpw +  0.5*dri(2)*(Jkp*s1kp*s3kp*(lkp+mkp)*dukp-Jkm*s1km*s3km*(lkm+mkm)*dukm+J0*s10*s30*(l0+m0)*(dukp-dukm))
#endMacro	

#beginMacro addVCrossterms()
!// v terms in u eq.
rhtmpu = rhtmpu + fq*fr*(Jip*q1ip*r2ip*lip*(uipjp-uipjm)-Jim*q1im*r2im*lim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jip*q2ip*r1ip*mip*(uipjp-uipjm)-Jim*q2im*r1im*mim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fs*(Jip*q1ip*s2ip*lip*(uipkp-uipkm)-Jim*q1im*s2im*lim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fs*(Jip*q2ip*s1ip*mip*(uipkp-uipkm)-Jim*q2im*s1im*mim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r1jp*q2jp*ljp*(uipjp-uimjp)-Jjm*r1jm*q2jm*ljm*(uipjm-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r2jp*q1jp*mjp*(uipjp-uimjp)-Jjm*r2jm*q1jm*mjm*(uipjm-uimjm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r1jp*s2jp*ljp*(ujpkp-ujpkm)-Jjm*r1jm*s2jm*ljm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r2jp*s1jp*mjp*(ujpkp-ujpkm)-Jjm*r2jm*s1jm*mjm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fq*fs*(Jkp*s1kp*q2kp*lkp*(uipkp-uimkp)-Jkm*s1km*q2km*lkm*(uipkm-uimkm))	
rhtmpu = rhtmpu + fq*fs*(Jkp*s2kp*q1kp*mkp*(uipkp-uimkp)-Jkm*s2km*q1km*mkm*(uipkm-uimkm))	
rhtmpu = rhtmpu + fr*fs*(Jkp*s1kp*r2kp*lkp*(ujpkp-ujmkp)-Jkm*s1km*r2km*lkm*(ujpkm-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jkp*s2kp*r1kp*mkp*(ujpkp-ujmkp)-Jkm*s2km*r1km*mkm*(ujpkm-ujmkm))
!// v terms in v eq. 
rhtmpv = rhtmpv + fq*fr*(Jip*q1ip*r1ip*mip*(uipjp-uipjm)-Jim*q1im*r1im*mim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jip*q2ip*r2ip*l2mip*(uipjp-uipjm)-Jim*q2im*r2im*l2mim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jip*q3ip*r3ip*mip*(uipjp-uipjm)-Jim*q3im*r3im*mim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fs*(Jip*q1ip*s1ip*mip*(uipkp-uipkm)-Jim*q1im*s1im*mim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fs*(Jip*q2ip*s2ip*l2mip*(uipkp-uipkm)-Jim*q2im*s2im*l2mim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fs*(Jip*q3ip*s3ip*mip*(uipkp-uipkm)-Jim*q3im*s3im*mim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r1jp*q1jp*mjp*(uipjp-uimjp)-Jjm*r1jm*q1jm*mjm*(uipjm-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r2jp*q2jp*l2mjp*(uipjp-uimjp)-Jjm*r2jm*q2jm*l2mjm*(uipjm-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r3jp*q3jp*mjp*(uipjp-uimjp)-Jjm*r3jm*q3jm*mjm*(uipjm-uimjm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r1jp*s1jp*mjp*(ujpkp-ujpkm)-Jjm*r1jm*s1jm*mjm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r2jp*s2jp*l2mjp*(ujpkp-ujpkm)-Jjm*r2jm*s2jm*l2mjm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r3jp*s3jp*mjp*(ujpkp-ujpkm)-Jjm*r3jm*s3jm*mjm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fq*fs*(Jkp*s1kp*q1kp*mkp*(uipkp-uimkp)-Jkm*s1km*q1km*mkm*(uipkm-uimkm))
rhtmpv = rhtmpv + fq*fs*(Jkp*s2kp*q2kp*l2mkp*(uipkp-uimkp)-Jkm*s2km*q2km*l2mkm*(uipkm-uimkm))
rhtmpv = rhtmpv + fq*fs*(Jkp*s3kp*q3kp*mkp*(uipkp-uimkp)-Jkm*s3km*q3km*mkm*(uipkm-uimkm))
rhtmpv = rhtmpv + fr*fs*(Jkp*s1kp*r1kp*mkp*(ujpkp-ujmkp)-Jkm*s1km*r1km*mkm*(ujpkm-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jkp*s2kp*r2kp*l2mkp*(ujpkp-ujmkp)-Jkm*s2km*r2km*l2mkm*(ujpkm-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jkp*s3kp*r3kp*mkp*(ujpkp-ujmkp)-Jkm*s3km*r3km*mkm*(ujpkm-ujmkm))
!// v terms in w eq.
rhtmpw = rhtmpw + fq*fr*(Jip*q3ip*r2ip*lip*(uipjp-uipjm)-Jim*q3im*r2im*lim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jip*q2ip*r3ip*mip*(uipjp-uipjm)-Jim*q2im*r3im*mim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fs*(Jip*q3ip*s2ip*lip*(uipkp-uipkm)-Jim*q3im*s2im*lim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fs*(Jip*q2ip*s3ip*mip*(uipkp-uipkm)-Jim*q2im*s3im*mim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r3jp*q2jp*ljp*(uipjp-uimjp)-Jjm*r3jm*q2jm*ljm*(uipjm-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r2jp*q3jp*mjp*(uipjp-uimjp)-Jjm*r2jm*q3jm*mjm*(uipjm-uimjm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r3jp*s2jp*ljp*(ujpkp-ujpkm)-Jjm*r3jm*s2jm*ljm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r2jp*s3jp*mjp*(ujpkp-ujpkm)-Jjm*r2jm*s3jm*mjm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fq*fs*(Jkp*s3kp*q2kp*lkp*(uipkp-uimkp)-Jkm*s3km*q2km*lkm*(uipkm-uimkm))	
rhtmpw = rhtmpw + fq*fs*(Jkp*s2kp*q3kp*mkp*(uipkp-uimkp)-Jkm*s2km*q3km*mkm*(uipkm-uimkm))	
rhtmpw = rhtmpw + fr*fs*(Jkp*s3kp*r2kp*lkp*(ujpkp-ujmkp)-Jkm*s3km*r2km*lkm*(ujpkm-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jkp*s2kp*r3kp*mkp*(ujpkp-ujmkp)-Jkm*s2km*r3km*mkm*(ujpkm-ujmkm))
#endMacro	

#beginMacro addVterms()
! v terms in u eq.
rhtmpu = rhtmpu +  0.5*dri(0)*(Jip*q2ip*q1ip*(lip+mip)*duip-Jim*q2im*q1im*(lim+mim)*duim+J0*q20*q10*(l0+m0)*(duip-duim))
rhtmpu = rhtmpu +  0.5*dri(1)*(Jjp*r1jp*r2jp*(ljp+mjp)*dujp-Jjm*r1jm*r2jm*(ljm+mjm)*dujm+J0*r10*r20*(l0+m0)*(dujp-dujm))
rhtmpu = rhtmpu +  0.5*dri(2)*(Jkp*s1kp*s2kp*(lkp+mkp)*dukp-Jkm*s1km*s2km*(lkm+mkm)*dukm+J0*s10*s20*(l0+m0)*(dukp-dukm))
! v terms in v eq. 
rhtmpv = rhtmpv +  0.5*dri(0)*(Jip*q1ip*q1ip*mip*duip-Jim*q1im*q1im*mim*duim+J0*q10*q10*m0*(duip-duim))
rhtmpv = rhtmpv +  0.5*dri(0)*(Jip*q2ip*q2ip*l2mip*duip-Jim*q2im*q2im*l2mim*duim+J0*q20*q20*l2m0*(duip-duim))
rhtmpv = rhtmpv +  0.5*dri(0)*(Jip*q3ip*q3ip*mip*duip-Jim*q3im*q3im*mim*duim+J0*q30*q30*m0*(duip-duim))
rhtmpv = rhtmpv +  0.5*dri(1)*(Jjp*r1jp*r1jp*mjp*dujp-Jjm*r1jm*r1jm*mjm*dujm+J0*r10*r10*m0*(dujp-dujm))
rhtmpv = rhtmpv +  0.5*dri(1)*(Jjp*r2jp*r2jp*l2mjp*dujp-Jjm*r2jm*r2jm*l2mjm*dujm+J0*r20*r20*l2m0*(dujp-dujm))
rhtmpv = rhtmpv +  0.5*dri(1)*(Jjp*r3jp*r3jp*mjp*dujp-Jjm*r3jm*r3jm*mjm*dujm+J0*r30*r30*m0*(dujp-dujm))
rhtmpv = rhtmpv +  0.5*dri(2)*(Jkp*s1kp*s1kp*mkp*dukp-Jkm*s1km*s1km*mkm*dukm+J0*s10*s10*m0*(dukp-dukm))
rhtmpv = rhtmpv +  0.5*dri(2)*(Jkp*s2kp*s2kp*l2mkp*dukp-Jkm*s2km*s2km*l2mkm*dukm+J0*s20*s20*l2m0*(dukp-dukm))
rhtmpv = rhtmpv +  0.5*dri(2)*(Jkp*s3kp*s3kp*mkp*dukp-Jkm*s3km*s3km*mkm*dukm+J0*s30*s30*m0*(dukp-dukm))
! v terms in w eq.
rhtmpw = rhtmpw +  0.5*dri(0)*(Jip*q2ip*q3ip*(lip+mip)*duip-Jim*q2im*q3im*(lim+mim)*duim+J0*q20*q30*(l0+m0)*(duip-duim))
rhtmpw = rhtmpw +  0.5*dri(1)*(Jjp*r3jp*r2jp*(ljp+mjp)*dujp-Jjm*r3jm*r2jm*(ljm+mjm)*dujm+J0*r30*r20*(l0+m0)*(dujp-dujm))
rhtmpw = rhtmpw +  0.5*dri(2)*(Jkp*s3kp*s2kp*(lkp+mkp)*dukp-Jkm*s3km*s2km*(lkm+mkm)*dukm+J0*s30*s20*(l0+m0)*(dukp-dukm))
#endMacro

#beginMacro addWCrossterms()
!// w terms in u eq.
rhtmpu = rhtmpu + fq*fr*(Jip*q1ip*r3ip*lip*(uipjp-uipjm)-Jim*q1im*r3im*lim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jip*q3ip*r1ip*mip*(uipjp-uipjm)-Jim*q3im*r1im*mim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fs*(Jip*q1ip*s3ip*lip*(uipkp-uipkm)-Jim*q1im*s3im*lim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fs*(Jip*q3ip*s1ip*mip*(uipkp-uipkm)-Jim*q3im*s1im*mim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r1jp*q3jp*ljp*(uipjp-uimjp)-Jjm*r1jm*q3jm*ljm*(uipjm-uimjm))
rhtmpu = rhtmpu + fq*fr*(Jjp*r3jp*q1jp*mjp*(uipjp-uimjp)-Jjm*r3jm*q1jm*mjm*(uipjm-uimjm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r1jp*s3jp*ljp*(ujpkp-ujpkm)-Jjm*r1jm*s3jm*ljm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jjp*r3jp*s1jp*mjp*(ujpkp-ujpkm)-Jjm*r3jm*s1jm*mjm*(ujmkp-ujmkm))
rhtmpu = rhtmpu + fq*fs*(Jkp*s1kp*q3kp*lkp*(uipkp-uimkp)-Jkm*s1km*q3km*lkm*(uipkm-uimkm))	
rhtmpu = rhtmpu + fq*fs*(Jkp*s3kp*q1kp*mkp*(uipkp-uimkp)-Jkm*s3km*q1km*mkm*(uipkm-uimkm))	
rhtmpu = rhtmpu + fr*fs*(Jkp*s1kp*r3kp*lkp*(ujpkp-ujmkp)-Jkm*s1km*r3km*lkm*(ujpkm-ujmkm))
rhtmpu = rhtmpu + fr*fs*(Jkp*s3kp*r1kp*mkp*(ujpkp-ujmkp)-Jkm*s3km*r1km*mkm*(ujpkm-ujmkm))
!// w terms in v eq.
rhtmpv = rhtmpv + fq*fr*(Jip*q2ip*r3ip*lip*(uipjp-uipjm)-Jim*q2im*r3im*lim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jip*q3ip*r2ip*mip*(uipjp-uipjm)-Jim*q3im*r2im*mim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fs*(Jip*q2ip*s3ip*lip*(uipkp-uipkm)-Jim*q2im*s3im*lim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fs*(Jip*q3ip*s2ip*mip*(uipkp-uipkm)-Jim*q3im*s2im*mim*(uimkp-uimkm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r2jp*q3jp*ljp*(uipjp-uimjp)-Jjm*r2jm*q3jm*ljm*(uipjm-uimjm))
rhtmpv = rhtmpv + fq*fr*(Jjp*r3jp*q2jp*mjp*(uipjp-uimjp)-Jjm*r3jm*q2jm*mjm*(uipjm-uimjm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r2jp*s3jp*ljp*(ujpkp-ujpkm)-Jjm*r2jm*s3jm*ljm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jjp*r3jp*s2jp*mjp*(ujpkp-ujpkm)-Jjm*r3jm*s2jm*mjm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fq*fs*(Jkp*s2kp*q3kp*lkp*(uipkp-uimkp)-Jkm*s2km*q3km*lkm*(uipkm-uimkm))	
rhtmpv = rhtmpv + fq*fs*(Jkp*s3kp*q2kp*mkp*(uipkp-uimkp)-Jkm*s3km*q2km*mkm*(uipkm-uimkm))	
rhtmpv = rhtmpv + fr*fs*(Jkp*s2kp*r3kp*lkp*(ujpkp-ujmkp)-Jkm*s2km*r3km*lkm*(ujpkm-ujmkm))
rhtmpv = rhtmpv + fr*fs*(Jkp*s3kp*r2kp*mkp*(ujpkp-ujmkp)-Jkm*s3km*r2km*mkm*(ujpkm-ujmkm))
!// w terms in w eq.
rhtmpw = rhtmpw + fq*fr*(Jip*q1ip*r1ip*mip*(uipjp-uipjm)-Jim*q1im*r1im*mim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jip*q2ip*r2ip*mip*(uipjp-uipjm)-Jim*q2im*r2im*mim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jip*q3ip*r3ip*l2mip*(uipjp-uipjm)-Jim*q3im*r3im*l2mim*(uimjp-uimjm))
rhtmpw = rhtmpw + fq*fs*(Jip*q1ip*s1ip*mip*(uipkp-uipkm)-Jim*q1im*s1im*mim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fs*(Jip*q2ip*s2ip*mip*(uipkp-uipkm)-Jim*q2im*s2im*mim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fs*(Jip*q3ip*s3ip*l2mip*(uipkp-uipkm)-Jim*q3im*s3im*l2mim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r1jp*q1jp*mjp*(uipjp-uimjp)-Jjm*r1jm*q1jm*mjm*(uipjm-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r2jp*q2jp*mjp*(uipjp-uimjp)-Jjm*r2jm*q2jm*mjm*(uipjm-uimjm))
rhtmpw = rhtmpw + fq*fr*(Jjp*r3jp*q3jp*l2mjp*(uipjp-uimjp)-Jjm*r3jm*q3jm*l2mjm*(uipjm-uimjm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r1jp*s1jp*mjp*(ujpkp-ujpkm)-Jjm*r1jm*s1jm*mjm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r2jp*s2jp*mjp*(ujpkp-ujpkm)-Jjm*r2jm*s2jm*mjm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jjp*r3jp*s3jp*l2mjp*(ujpkp-ujpkm)-Jjm*r3jm*s3jm*l2mjm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fq*fs*(Jkp*s1kp*q1kp*mkp*(uipkp-uimkp)-Jkm*s1km*q1km*mkm*(uipkm-uimkm))
rhtmpw = rhtmpw + fq*fs*(Jkp*s2kp*q2kp*mkp*(uipkp-uimkp)-Jkm*s2km*q2km*mkm*(uipkm-uimkm))
rhtmpw = rhtmpw + fq*fs*(Jkp*s3kp*q3kp*l2mkp*(uipkp-uimkp)-Jkm*s3km*q3km*l2mkm*(uipkm-uimkm))
rhtmpw = rhtmpw + fr*fs*(Jkp*s1kp*r1kp*mkp*(ujpkp-ujmkp)-Jkm*s1km*r1km*mkm*(ujpkm-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jkp*s2kp*r2kp*mkp*(ujpkp-ujmkp)-Jkm*s2km*r2km*mkm*(ujpkm-ujmkm))
rhtmpw = rhtmpw + fr*fs*(Jkp*s3kp*r3kp*l2mkp*(ujpkp-ujmkp)-Jkm*s3km*r3km*l2mkm*(ujpkm-ujmkm))
#endMacro

#beginMacro addWterms()
! w terms in u eq.
rhtmpu = rhtmpu +  0.5*dri(0)*(Jip*q1ip*q3ip*(lip+mip)*duip-Jim*q1im*q3im*(lim+mim)*duim+J0*q10*q30*(l0+m0)*(duip-duim))
rhtmpu = rhtmpu +  0.5*dri(1)*(Jjp*r1jp*r3jp*(ljp+mjp)*dujp-Jjm*r1jm*r3jm*(ljm+mjm)*dujm+J0*r10*r30*(l0+m0)*(dujp-dujm))
rhtmpu = rhtmpu +  0.5*dri(2)*(Jkp*s1kp*s3kp*(lkp+mkp)*dukp-Jkm*s1km*s3km*(lkm+mkm)*dukm+J0*s10*s30*(l0+m0)*(dukp-dukm))
! w terms in v eq.
rhtmpv = rhtmpv +  0.5*dri(0)*(Jip*q2ip*q3ip*(lip+mip)*duip-Jim*q2im*q3im*(lim+mim)*duim+J0*q20*q30*(l0+m0)*(duip-duim))
rhtmpv = rhtmpv +  0.5*dri(1)*(Jjp*r2jp*r3jp*(ljp+mjp)*dujp-Jjm*r2jm*r3jm*(ljm+mjm)*dujm+J0*r20*r30*(l0+m0)*(dujp-dujm))
rhtmpv = rhtmpv +  0.5*dri(2)*(Jkp*s2kp*s3kp*(lkp+mkp)*dukp-Jkm*s2km*s3km*(lkm+mkm)*dukm+J0*s20*s30*(l0+m0)*(dukp-dukm))
! w terms in w eq.
rhtmpw = rhtmpw +  0.5*dri(0)*(Jip*q1ip*q1ip*mip*duip-Jim*q1im*q1im*mim*duim+J0*q10*q10*m0*(duip-duim))
rhtmpw = rhtmpw +  0.5*dri(0)*(Jip*q2ip*q2ip*mip*duip-Jim*q2im*q2im*mim*duim+J0*q20*q20*m0*(duip-duim))
rhtmpw = rhtmpw +  0.5*dri(0)*(Jip*q3ip*q3ip*l2mip*duip-Jim*q3im*q3im*l2mim*duim+J0*q30*q30*l2m0*(duip-duim))
rhtmpw = rhtmpw +  0.5*dri(1)*(Jjp*r1jp*r1jp*mjp*dujp-Jjm*r1jm*r1jm*mjm*dujm+J0*r10*r10*m0*(dujp-dujm))
rhtmpw = rhtmpw +  0.5*dri(1)*(Jjp*r2jp*r2jp*mjp*dujp-Jjm*r2jm*r2jm*mjm*dujm+J0*r20*r20*m0*(dujp-dujm))
rhtmpw = rhtmpw +  0.5*dri(1)*(Jjp*r3jp*r3jp*l2mjp*dujp-Jjm*r3jm*r3jm*l2mjm*dujm+J0*r30*r30*l2m0*(dujp-dujm))
rhtmpw = rhtmpw +  0.5*dri(2)*(Jkp*s1kp*s1kp*mkp*dukp-Jkm*s1km*s1km*mkm*dukm+J0*s10*s10*m0*(dukp-dukm))
rhtmpw = rhtmpw +  0.5*dri(2)*(Jkp*s2kp*s2kp*mkp*dukp-Jkm*s2km*s2km*mkm*dukm+J0*s20*s20*m0*(dukp-dukm))
rhtmpw = rhtmpw +  0.5*dri(2)*(Jkp*s3kp*s3kp*l2mkp*dukp-Jkm*s3km*s3km*l2mkm*dukm+J0*s30*s30*l2m0*(dukp-dukm))
#endMacro

#beginMacro beginLoops()
do k=n3a,n3b
do j=n2a,n2b
do i=n1a,n1b
#endMacro
#beginMacro beginLoopsD()
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
#endMacro

#beginMacro correctCorners(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b,imm,ipp,jmm,jpp,kmm,kpp,fqq,frr,fss)
do k=nn3a,nn3b 
do j=nn2a,nn2b
do i=nn1a,nn1b
rhtmpu=0.
rhtmpv=0.
rhtmpw=0.
im=i-imm
ip=i+ipp
jm=j-jmm
jp=j+jpp
km=k-kmm
kp=k+kpp
fq=fqq*dri(0)
fr=frr*dri(1)
fs=fss*dri(2)
setupMaterialAndMetric()
setupU()
addUCrossterms()	
setupV()
addVCrossterms()	
setupW()
addWCrossterms()
im=i-1
ip=i+1
jm=j-1
jp=j+1
km=k-1
kp=k+1
fq=0.5*dri(0)
fr=0.5*dri(1)
fs=0.5*dri(2)
setupMaterialAndMetric()
setupU()
addUterms()	
setupV()
addVterms()	
setupW()
addWterms()
rh1(i,j,k)=rhtmpu/Jac(i,j,k)
rh2(i,j,k)=rhtmpv/Jac(i,j,k)
rh3(i,j,k)=rhtmpw/Jac(i,j,k)
end do
end do
end do
#endMacro

c     
c     Advance the equations of solid mechanics
c       
	subroutine advSmCons3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     &     nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,xy, um,u,un,f, 
     &     ndMatProp,matIndex,matValpc,matVal, bc, dis, 
     &     varDis, ipar, rpar, ierr )
c======================================================================
c     Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
c     
c     nd : number of space dimensions
c     
c     ipar(0)  = option : option=0 - Elasticity+Artificial diffusion
c     =1 - AD only
c     
c     dis(i1,i2,i3) : temp space to hold artificial dissipation
c     varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
c======================================================================
	implicit none
	integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     &     nd3b,nd4a,nd4b
	real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
	real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
	real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
	real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
	real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
	real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
	real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
	real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
	integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
	integer bc(0:1,0:2),ierr
	integer ipar(0:*)
	real rpar(0:*)

      ! -- Declare arrays for variable material properties --
      include 'declareVarMatProp.h'

c       ---- local variables -----
	integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
	integer addForcing,orderOfDissipation,option
	integer useWhereMask,useWhereMaskSave,grid,myid,
     &     useVariableDissipation,timeSteppingMethod
	integer useConservative,combineDissipationWithAdvance
	integer uc,vc,wc,s1,s2,s3,a1,a2,a3,nc1a,nc1b,nc2a,nc2b,nc3a,nc3b
	integer i,j,k,im,ip,jm,jp,km,kp,icm,icp,jcm,jcp,kcm,kcp
	real dt,dx(0:3),adc,dr(0:3),c1,c2,kx,ky,kz,t,fcq,fcr,fcs
	real qx,qy,qz,rx,ry,rz,sx,sy,sz
	real lam2mu,lam,mu,uxy0,uy0,vxy0,vy0,epep
	real Dup, Dum, Dvp, Dvm, Ep, Em ,dcons,dc,Jac,u1,u2,u3
	real rh1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
	real rh2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
	real rh3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
	real dri(0:3)
	real dtsq,errmaxu,errtmpu,exsolu,errmaxv,errtmpv,exsolv
	real u0,uipjp,uipjm,uimjp,uimjm
	real uipkp,uipkm,uimkp,uimkm
	real ujpkp,ujpkm,ujmkp,ujmkm
	real duip,duim,dujm,dujp,dukm,dukp
	real q1ip,q2ip,q3ip,q1im,q2im,q3im,q10,q20,q30
	real r1ip,r2ip,r3ip,r1im,r2im,r3im,r10,r20,r30
	real s1ip,s2ip,s3ip,s1im,s2im,s3im,s10,s20,s30
	real Jip,Jim,J0,lip,l0,lim,mip,mim,m0,l2mip,l2mim,l2m0
	real Jjp,Jjm,ljp,ljm,mjp,mjm,l2mjp,l2mjm,q1jp,q2jp,q3jp,q1jm,q2jm,q3jm
	real r1jp,r2jp,r3jp,r1jm,r2jm,r3jm,s1jp,s2jp,s3jp,s1jm,s2jm,s3jm
	real Jkp,Jkm,lkp,lkm,mkp,mkm,l2mkp,l2mkm,q1kp,q2kp,q3kp,q1km,q2km,q3km
	real r1kp,r2kp,r3kp,r1km,r2km,r3km,s1kp,s2kp,s3kp,s1km,s2km,s3km
	real rhtmpu,rhtmpv,rhtmpw,fr,fq,fs
        real weight,energy
        real dtOld,cu,cum
        integer computeUt
	integer dirichlet,stressFree,debug
	parameter( dirichlet=1,stressFree=2 )
        real du,fd23d,fd43d,adcdt
	
c     ******* artificial dissipation ******
        du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)
        fd23d(i1,i2,i3,c)=\
        (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
        -6.*du(i1,i2,i3,c) )

c     -(fourth difference)
        fd43d(i1,i2,i3,c)=\
        (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
        +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
        -18.*du(i1,i2,i3,c) )
	
	qx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
	qy(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
	qz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
	rx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
	ry(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
	rz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
	sx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
	sy(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
	sz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
	Jac(i1,i2,i3)=(1.d0/(qx(i1,i2,i3)*ry(i1,i2,i3)*sz(i1,i2,i3)+qy(i1,i2,i3)*rz(i1,i2,i3)*sx(i1,i2,i3)+qz(i1,i2,i3)*rx(i1,i2,i3)*sy(i1,i2,i3)-sx(i1,i2,i3)*ry(i1,i2,i3)*qz(i1,i2,i3)-sy(i1,i2,i3)*rz(i1,i2,i3)*qx(i1,i2,i3)-sz(i1,i2,i3)*rx(i1,i2,i3)*qy(i1,i2,i3)))
	lam(i1,i2,i3)=(c1-c2)
	mu(i1,i2,i3)=c2
	lam2mu(i1,i2,i3)=(lam(i1,i2,i3)+2.0*mu(i1,12,i3))
	
	dt    =rpar(0)
	dx(0) =rpar(1)
	dx(1) =rpar(2)
	dx(2) =rpar(3)
	adc   =rpar(4)		! coefficient of artificial dissipation
	dr(0) =rpar(5)
	dr(1) =rpar(6)
	dr(2) =rpar(7)
	c1    =rpar(8)
	c2    =rpar(9) 
	kx    =rpar(10) 
	ky    =rpar(11) 
	kz    =rpar(12) 
	epep  =rpar(13)
	t     =rpar(14)
        dtOld =rpar(15) ! dt used on the previous time step 

	option             =ipar(0)
	gridType           =ipar(1)
	orderOfAccuracy    =ipar(2)
	orderInTime        =ipar(3)
	addForcing         =ipar(4)
	orderOfDissipation =ipar(5)
	uc                 =ipar(6)
	vc                 =ipar(7)
	wc                 =ipar(8)
	useWhereMask       =ipar(9)
	timeSteppingMethod =ipar(10)
	useVariableDissipation=ipar(11)
	useConservative    =ipar(12)   
	combineDissipationWithAdvance = ipar(13)
	debug              =ipar(14)
        computeUt          =ipar(15)
        materialFormat     =ipar(16) 
        myid               =ipar(17)

      if( materialFormat.ne.constantMaterialProperties )then
        write(*,'(" ***advSmCons3dOrder2c:ERROR: Finish me for variable material")')
        stop 11122
      end if


	dtsq=dt*dt
	do i=0,2	
	   dri(i)=1.0d0/dr(i)
	enddo

        ! *wdh* 100201 -- fixes for variable time step : locally 2nd order --
        cu=  2.     ! coeff. of u(t) in the time-step formula
        cum=-1.     ! coeff. of u(t-dtOld)
        if( dtOld.le.0 )then
          write(*,'(" advSmCons:ERROR : dtOld<=0 ")')
          stop 8167
        end if
        if( dt.ne.dtOld )then
          write(*,'(" advSmCons:INFO: dt=",e12.4," <> dtOld=",e12.4," diff=",e9.2)') dt,dtOld,dt-dtOld
          ! adjust the coefficients for a variable time step : this is locally second order accurate
          cu= 1.+dt/dtOld     ! coeff. of u(t) in the time-step formula
          cum=-dt/dtOld       ! coeff. of u(t-dtOld)
          dtsq=dt*(dt+dtOld)*.5
        end if
        ! for variable time step: ( *wdh* 100203 )
        if( computeUt.eq.0 )then
          adcdt = adc*(dt*(dt+dtOld)/2.)/dtOld  
        else
         adcdt= adc/dtOld                    
         write(*,*) 'ERROR: finish me'
         stop 12345
        end if

	beginLoops() 

	rhtmpu=0.
	rhtmpv=0.
	rhtmpw=0.

	im=i-1
	ip=i+1
	jm=j-1
	jp=j+1
	km=k-1
	kp=k+1
	fq=0.5*dri(0)
	fr=0.5*dri(1)
	fs=0.5*dri(2)

	setupMaterialAndMetric()

	setupU()
	addUterms()	
	addUCrossterms()	

	setupV()
	addVterms()	
	addVCrossterms()	

	setupW()
	addWterms()
	addWCrossterms()	

	rh1(i,j,k)=rhtmpu/Jac(i,j,k)
	rh2(i,j,k)=rhtmpv/Jac(i,j,k)
	rh3(i,j,k)=rhtmpw/Jac(i,j,k)
	endLoops()
        
        ! Correct the sides if necessary
        if(bc(0,0).eq.stressFree) then
           correctCorners(n1a,n1a,n2a,n2b,n3a,n3b,0,1,1,1,1,1,1.0,0.5,0.5)
        end if
        if(bc(1,0).eq.stressFree) then 
           correctCorners(n1b,n1b,n2a,n2b,n3a,n3b,1,0,1,1,1,1,1.0,0.5,0.5)
        end if
        if(bc(0,1).eq.stressFree) then
           correctCorners(n1a,n1b,n2a,n2a,n3a,n3b,1,1,0,1,1,1,0.5,1.0,0.5)
        end if
        if(bc(1,1).eq.stressFree) then 
           correctCorners(n1a,n1b,n2b,n2b,n3a,n3b,1,1,1,0,1,1,0.5,1.0,0.5)
        end if
        if(bc(0,2).eq.stressFree) then 
           correctCorners(n1a,n1b,n2a,n2b,n3a,n3a,1,1,1,1,0,1,0.5,0.5,1.0)
        end if
        if(bc(1,2).eq.stressFree) then 
           correctCorners(n1a,n1b,n2a,n2b,n3b,n3b,1,1,1,1,1,0,0.5,0.5,1.0)
        end if
! And the edge
        if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree)) then
           correctCorners(n1a,n1a,n2a,n2a,n3a,n3b,0,1,0,1,1,1,1.0,1.0,0.5)
        end if
        if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree)) then
           correctCorners(n1a,n1a,n2b,n2b,n3a,n3b,0,1,1,0,1,1,1.0,1.0,0.5)
        end if
        if((bc(0,0).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1a,n1a,n2a,n2b,n3a,n3a,0,1,1,1,0,1,1.0,0.5,1.0)
        end if
        if((bc(0,0).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1a,n1a,n2a,n2b,n3b,n3b,0,1,1,1,1,0,1.0,0.5,1.0)
        end if
        
        if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree)) then
           correctCorners(n1b,n1b,n2a,n2a,n3a,n3b,1,0,0,1,1,1,1.0,1.0,0.5)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(1,1).eq.stressFree)) then
           correctCorners(n1b,n1b,n2b,n2b,n3a,n3b,1,0,1,0,1,1,1.0,1.0,0.5)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1b,n1b,n2a,n2b,n3a,n3a,1,0,1,1,0,1,1.0,0.5,1.0)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1b,n1b,n2a,n2b,n3b,n3b,1,0,1,1,1,0,1.0,0.5,1.0)
        end if

        if((bc(0,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1a,n1b,n2a,n2a,n3a,n3a,1,1,0,1,0,1,0.5,1.0,1.0)
        end if
        if((bc(0,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1a,n1b,n2a,n2a,n3b,n3b,1,1,0,1,1,0,0.5,1.0,1.0)
        end if
        if((bc(1,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1a,n1b,n2b,n2b,n3a,n3a,1,1,1,0,0,1,0.5,1.0,1.0)
        end if
        if((bc(1,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1a,n1b,n2b,n2b,n3b,n3b,1,1,1,0,1,0,0.5,1.0,1.0)
        end if

!     Finally Corners
        if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1a,n1a,n2a,n2a,n3a,n3a,0,1,0,1,0,1,1.0,1.0,1.0)
        end if
        if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1a,n1a,n2a,n2a,n3b,n3b,0,1,0,1,1,0,1.0,1.0,1.0)
        end if
        if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1a,n1a,n2b,n2b,n3a,n3a,0,1,1,0,0,1,1.0,1.0,1.0)
        end if
        if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1a,n1a,n2b,n2b,n3b,n3b,0,1,1,0,1,0,1.0,1.0,1.0)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1b,n1b,n2a,n2a,n3a,n3a,1,0,0,1,0,1,1.0,1.0,1.0)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1b,n1b,n2a,n2a,n3b,n3b,1,0,0,1,1,0,1.0,1.0,1.0)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
           correctCorners(n1b,n1b,n2b,n2b,n3a,n3a,1,0,1,0,0,1,1.0,1.0,1.0)
        end if
        if((bc(1,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
           correctCorners(n1b,n1b,n2b,n2b,n3b,n3b,1,0,1,0,1,0,1.0,1.0,1.0)
        end if
        



c     Assign next to next time level            
	beginLoops() 
	un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*rh1(i,j,k)
	un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*rh2(i,j,k)
	un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*rh3(i,j,k)
	endLoops()
c     Add on forcing
        if(addForcing.ne.0) then
           beginLoops() 
           un(i,j,k,uc)=un(i,j,k,uc)+dtsq*f(i,j,k,uc)
           un(i,j,k,vc)=un(i,j,k,vc)+dtsq*f(i,j,k,vc)
           un(i,j,k,wc)=un(i,j,k,wc)+dtsq*f(i,j,k,wc)
           endLoops()       
        end if
        
        if( (orderOfDissipation.eq.4 ).and.(adc.gt.0))then
           ! *wdh* 100203 adcdt=adc*dt
           beginLoopsD() 
           un(i1,i2,i3,uc)=un(i1,i2,i3,uc)+adcdt*fd43d(i1,i2,i3,uc)
           un(i1,i2,i3,vc)=un(i1,i2,i3,vc)+adcdt*fd43d(i1,i2,i3,vc)
           un(i1,i2,i3,wc)=un(i1,i2,i3,wc)+adcdt*fd43d(i1,i2,i3,wc)
           endLoops()       
        end if
        if( (orderOfDissipation.eq.2 ).and.(adc.gt.0))then
           ! *wdh* 100203 adcdt=adc*dt
           beginLoopsD() 
           un(i1,i2,i3,uc)=un(i1,i2,i3,uc)+adcdt*fd23d(i1,i2,i3,uc)
           un(i1,i2,i3,vc)=un(i1,i2,i3,vc)+adcdt*fd23d(i1,i2,i3,vc)
           un(i1,i2,i3,wc)=un(i1,i2,i3,wc)+adcdt*fd23d(i1,i2,i3,wc)
           endLoops()       
        end if
      if(debug.eq.3) then	
      energy=0.d0
      ! DEAA ENERGY
      do i3=n3a,n3b
      do i2=n2a,n2b
      do i1=n1a,n1b
      	weight=1.d0
	if ((i1.eq.n1a).and.((bc(0,0).eq.stressFree))) weight=weight*0.5d0
	if ((i1.eq.n1b).and.((bc(1,0).eq.stressFree))) weight=weight*0.5d0
	if ((i2.eq.n2a).and.((bc(0,1).eq.stressFree))) weight=weight*0.5d0
	if ((i2.eq.n2b).and.((bc(1,1).eq.stressFree))) weight=weight*0.5d0
	if ((i3.eq.n3a).and.((bc(0,2).eq.stressFree))) weight=weight*0.5d0
	if ((i3.eq.n3b).and.((bc(1,2).eq.stressFree))) weight=weight*0.5d0
!        write (*,*),i1,i2,weight
	energy=energy-weight*un(i1,i2,i3,uc)*rh1(i1,i2,i3)*Jac(i1,i2,i3)
	energy=energy-weight*un(i1,i2,i3,vc)*rh2(i1,i2,i3)*Jac(i1,i2,i3)
	energy=energy-weight*un(i1,i2,i3,wc)*rh3(i1,i2,i3)*Jac(i1,i2,i3)
	!       we use f to store u_t
        rh1(i1,i2,i3)=(un(i1,i2,i3,uc)-u(i1,i2,i3,uc))/dt
        rh2(i1,i2,i3)=(un(i1,i2,i3,vc)-u(i1,i2,i3,vc))/dt
        rh3(i1,i2,i3)=(un(i1,i2,i3,wc)-u(i1,i2,i3,wc))/dt
	energy=energy+weight*rh1(i1,i2,i3)*rh1(i1,i2,i3)*Jac(i1,i2,i3)
	energy=energy+weight*rh2(i1,i2,i3)*rh2(i1,i2,i3)*Jac(i1,i2,i3)
	energy=energy+weight*rh3(i1,i2,i3)*rh3(i1,i2,i3)*Jac(i1,i2,i3)
      endLoops() 
      write(*,*) "Discrete energy  ",energy
      end if

      end 

