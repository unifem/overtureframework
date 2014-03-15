#! /bin/csh -f
#
#
echo '-------------------------------------------------------'
echo '  Some threed examples for CGMG '
echo '-------------------------------------------------------'


echo '>>>Enter option: '
echo ' pipe        : 3D pipe '
echo ' pipes2      : Intersecting pipes'
echo ' ellipse2(m) : Ellipsoid in a box (m=Mixed BCs)'
echo ' cylinder(n) : Cylinder (n=Neumann BCs)'
set option = $<

if( $option == "pipe" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/pipe.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=5.,fratio=25.,
      flags='C',tol=1.e-2,idcges=1,info=7,
      epsz=1.e-6
 &end
EOF

else if( $option == "pipes2" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/pipes2.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=5.,fratio=25.,
      flags='C',tol=1.e-2,idcges=1,info=7,
      epsz=1.e-6
 &end
EOF

else if( $option == "ellipse2" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/ellipse2.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=8.,fratio=25.,
      flags='C',tol=1.e-2,idcges=1,info=1,
      epsz=1.e-6
 &end
EOF

else if( $option == "ellipse2m" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=17: Laplace, Mixed BC'
echo 'iord=2  : Second Order'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/ellipse2.dat
 &inp itest=17,iord=2,eps=1.e-2,zratio=7.,fratio=100.,epsz=1.e-5,
      ipcm=1,intbc=1,eta=.9,
      idcges=3,info=3,epsz=1.e-6,
      flags='C',tol=.1,icg=2,nsave=40,
   bc0(1,1,1)=5,bc0(1,2,1)=5,
   bc0(2,1,1)=5,bc0(2,2,1)=5,
   bc0(3,1,1)=5,bc0(3,2,1)=5,
   bc0(1,1,2)=1,bc0(1,2,2)=1,
   bc0(2,1,2)=1,bc0(2,2,2)=1,
   bc0(3,1,2)=1,bc0(3,2,2)=1,
   bc0(1,1,3)=1,bc0(1,2,3)=1,
   bc0(2,1,3)=1,bc0(2,2,3)=1,
   bc0(3,1,3)=1,bc0(3,2,3)=1,
   bc0(1,1,4)=1,bc0(1,2,4)=1,
   bc0(2,1,4)=1,bc0(2,2,4)=1,
   bc0(3,1,4)=1,bc0(3,2,4)=1,
   bc0(3,1,1)=2,bc0(3,2,1)=4
 &end
EOF
exit
      flags='C',tol=.2,
      flags='S',tol=.1,nit=32,
      flags='S',tol=.1,nit=32,

else if( $option == "cylinder" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/cyl3d2.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=8.,fratio=25.,
      flags='Y',tol=1.e-3,idcges=1,info=3,eta=.8,
      smth(1,1)=2,epsz=1.e-6,
   bc0(1,1,1)=5,bc0(1,2,1)=5,
   bc0(2,1,1)=5,bc0(2,2,1)=5,
   bc0(3,1,1)=5,bc0(3,2,1)=5
 &end
EOF

else
  echo " Invalid option = $option"
endif

exit









