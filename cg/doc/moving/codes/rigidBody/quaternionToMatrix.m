%
% Convert a Quaternion qv(1:4) into a matrix R(1:3,1:3)
%
function R = quaternionToMatrix(qv)


s  = qv(1); 
vx = qv(2);
vy = qv(3);
vz = qv(4);
vx2=vx*vx;
vy2=vy*vy;
vz2=vz*vz;

r11 = 1.-2*(vy2+vz2);
r12 = 2.*( vx*vy-s*vz);
r13 = 2.*( vx*vz+s*vy);

r21 = 2.*( vx*vy+s*vz);
r22 = 1.-2*(vx2+vz2);
r23 = 2.*( vy*vz-s*vx);

r31 = 2.*( vx*vz-s*vy);
r32 = 2.*( vy*vz+s*vx);
r33 = 1.-2*(vx2+vy2);

R = [ r11 r12 r13; r21 r22 r23; r31 r32 r33 ];