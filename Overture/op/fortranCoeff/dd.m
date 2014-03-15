clear;
maple('clear');

maple('urr(i1,i2,i3,c) :=u(i1,i2,i3,c)-u(i1-1,i2,i3,c);');
maple('urr(i1+1,i2,i3,c) := subs(i1=i1+1,urr(i1,i2,i3,c));');

maple('uss(i1,i2,i3,c) := u(i1,i2,i3,c)-u(i1,i2-1,i3,c);');
maple('uss(i1,i2+1,i3,c) := subs(i2=i2+1,uss(i1,i2,i3,c));');

maple('urs(i1,i2,i3,c) :=(u(i1-1,i2+1,i3,c)+u(i1,i2+1,i3,c)-u(i1-1,i2-1,i3,c)-u(i1,i2-1,i3,c));');
maple('urs(i1+1,i2,i3,c) := subs(i1=i1+1,urs(i1,i2,i3,c));');

maple('usr(i1,i2,i3,c) := (u(i1+1,i2-1,i3,c) + u(i1+1,i2,i3,c) - u(i1-1,i2-1,i3,c) - u(i1-1,i2,i3,c));');
maple('usr(i1,i2+1,i3,c) := subs(i2=i2+1,usr(i1,i2,i3,c));');

% maple('f:= subs(i1=i1+1,urr)-urr;')


maple('d:=((a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c))+(a22(i1,i2+1,i3)*uss(i1,i2+1,i3,c)-a22(i1,i2,i3)*uss(i1,i2,i3,c))+(a21(i1,i2+1,i3)*usr(i1,i2+1,i3,c)-a21(i1,i2,i3)*usr(i1,i2,i3,c)+a12(i1+1,i2,i3)*urs(i1+1,i2,i3,c)-a12(i1,i2,i3)*urs(i1,i2,i3,c)))/jac(i1,i2,i3);')


maple('i1Name := [ i1-2, i1-1, i1, i1+1, i1+2 ];');
maple('i2Name := [ i2-2, i2-1, i2, i2+1, i2+2 ];');
maple('i3Name := [ i3-2, i3-1, i3, i3+1, i3+2 ];');

fprintf('        loopBody2ndOrder2d(\n');
maple('j3:=i3;');

maple('m2:=-2;');
for m2=-1:1
maple('m2:=m2+1;');
maple('j2:=i2Name[m2+3];');

maple('m1:=-2;');
for m1=-1:1
maple('m1:=m1+1;');
maple('j1:=i1Name[m1+3];');

  %maple('s:=''u(i1,i2,i3)'';') % sprintf("u(i1%+d,i2%+d,i3)",m1,m2);')
  % maple('s:=cat(u , "(i1,i2,i3,c)");')
  %  maple('s:=u."(i1,i2,i3,c)";');
  % maple('whattype(s)')

   % cc = maple('limit(subs(s=x,d)/x,x=infinity);');
  % maple('dd:=eval(d,u(j1,j2,j3,c)=XXX);')

 cc = maple('limit(eval(d,u(j1,j2,j3,c)=XXX)/XXX,XXX=infinity);');

 fprintf('        %s',cc);
 if m1+m2 ~=2 
   fprintf(', \\\n');
 end
end
end
fprintf(' )\n\n');


maple('utt(i1,i2,i3,c) :=u(i1,i2,i3,c)-u(i1,i2,i3-1,c);');
maple('utt(i1,i2,i3+1,c) := subs(i3=i3+1,utt(i1,i2,i3,c));');

maple('urt(i1,i2,i3,c) :=(u(i1-1,i2,i3+1,c)+u(i1,i2,i3+1,c)-u(i1-1,i2,i3-1,c)-u(i1,i2,i3-1,c));');
maple('urt(i1+1,i2,i3,c) := subs(i1=i1+1,urt(i1,i2,i3,c));');

maple('utr(i1,i2,i3,c) := (u(i1+1,i2,i3-1,c) + u(i1+1,i2,i3,c) - u(i1-1,i2,i3-1,c) - u(i1-1,i2,i3,c));');
maple('utr(i1,i2,i3+1,c) := subs(i3=i3+1,utr(i1,i2,i3,c));');

maple('uts(i1,i2,i3,c) :=(u(i1,i2+1,i3-1,c)+u(i1,i2+1,i3,c)-u(i1,i2-1,i3-1,c)-u(i1,i2-1,i3,c));');
maple('uts(i1,i2,i3+1,c) := subs(i3=i3+1,uts(i1,i2,i3,c));');

maple('ust(i1,i2,i3,c) := (u(i1,i2-1,i3+1,c) + u(i1,i2,i3+1,c) - u(i1,i2-1,i3-1,c) - u(i1,i2,i3-1,c));');
maple('ust(i1,i2+1,i3,c) := subs(i2=i2+1,ust(i1,i2,i3,c));');


maple('urs(i1,i2,i3,c) :=(u(i1-1,i2+1,i3,c)+u(i1,i2+1,i3,c)-u(i1-1,i2-1,i3,c)-u(i1,i2-1,i3,c));');
maple('urs(i1+1,i2,i3,c) := subs(i1=i1+1,urs(i1,i2,i3,c));');


for k=1:11 % ------------- do 3D cases


if k==1 
 fprintf(' **** 3d rectangular  divScalarGrad *****\n');
  maple('d:=((a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c))+(a22(i1,i2+1,i3)*uss(i1,i2+1,i3,c)-a22(i1,i2,i3)*uss(i1,i2,i3,c))+(a33(i1,i2,i3+1)*utt(i1,i2,i3+1,c)-a33(i1,i2,i3)*utt(i1,i2,i3,c)));');

elseif k==2 

fprintf(' **** 3d curvilinear divScalarGrad *****\n');

maple('d:=((a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c))+(a22(i1,i2+1,i3)*uss(i1,i2+1,i3,c)-a22(i1,i2,i3)*uss(i1,i2,i3,c))+(a33(i1,i2,i3+1)*utt(i1,i2,i3+1,c)-a33(i1,i2,i3)*utt(i1,i2,i3,c))+(a21(i1,i2+1,i3)*usr(i1,i2+1,i3,c)-a21(i1,i2,i3)*usr(i1,i2,i3,c)+a12(i1+1,i2,i3)*urs(i1+1,i2,i3,c)-a12(i1,i2,i3)*urs(i1,i2,i3,c))+(a31(i1,i2,i3+1)*utr(i1,i2,i3+1,c)-a31(i1,i2,i3)*utr(i1,i2,i3,c)+a13(i1+1,i2,i3)*urt(i1+1,i2,i3,c)-a13(i1,i2,i3)*urt(i1,i2,i3,c))+(a32(i1,i2,i3+1)*uts(i1,i2,i3+1,c)-a32(i1,i2,i3)*uts(i1,i2,i3,c)+a23(i1,i2+1,i3)*ust(i1,i2+1,i3,c)-a23(i1,i2,i3)*ust(i1,i2,i3,c)))/jac(i1,i2,i3);');

elseif k==3

fprintf(' **** 3d rectangular Dx(s Dx) *****\n');  
maple('d:=(a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c));');

elseif k==4

fprintf(' **** 3d rectangular Dx(s Dy) *****\n');  
maple('d:=(a11(i1+1,i2,i3)*urs(i1+1,i2,i3,c)-a11(i1,i2,i3)*urs(i1,i2,i3,c));');


elseif k==5

fprintf(' **** 3d rectangular Dx(s Dz) *****\n');  
maple('d:=(a11(i1+1,i2,i3)*urt(i1+1,i2,i3,c)-a11(i1,i2,i3)*urt(i1,i2,i3,c));');

elseif k==6

fprintf(' **** 3d rectangular Dy(s Dx) *****\n');  
maple('d:=(a11(i1,i2+1,i3)*usr(i1,i2+1,i3,c)-a11(i1,i2,i3)*usr(i1,i2,i3,c));');

elseif k==7

fprintf(' **** 3d rectangular Dy(s Dy) *****\n');  
maple('d:=(a11(i1,i2+1,i3)*uss(i1,i2+1,i3,c)-a11(i1,i2,i3)*uss(i1,i2,i3,c));');
elseif k==8

fprintf(' **** 3d rectangular Dy(s Dz) *****\n');  
maple('d:=(a11(i1,i2+1,i3)*ust(i1,i2+1,i3,c)-a11(i1,i2,i3)*ust(i1,i2,i3,c));');

elseif k==9

fprintf(' **** 3d rectangular Dz(s Dx) *****\n');  
maple('d:=(a11(i1,i2,i3+1)*utr(i1,i2,i3+1,c)-a11(i1,i2,i3)*utr(i1,i2,i3,c));');

elseif k==10

fprintf(' **** 3d rectangular Dz(s Dy) *****\n');  
maple('d:=(a11(i1,i2,i3+1)*uts(i1,i2,i3+1,c)-a11(i1,i2,i3)*uts(i1,i2,i3,c));');

elseif k==11

fprintf(' **** 3d rectangular Dz(s Dz) *****\n');  
maple('d:=(a11(i1,i2,i3+1)*utt(i1,i2,i3+1,c)-a11(i1,i2,i3)*utt(i1,i2,i3,c));');

end

fprintf('        loopBody2ndOrder3d(\\\n        ');

len = 0;
maple('m3:=-2;');
for m3=-1:1
maple('m3:=m3+1;');
maple('j3:=i3Name[m3+3];');

maple('m2:=-2;');
for m2=-1:1
maple('m2:=m2+1;');
maple('j2:=i2Name[m2+3];');

maple('m1:=-2;');
for m1=-1:1
maple('m1:=m1+1;');
maple('j1:=i1Name[m1+3];');

 % maple('s:=sprintf("u(i1%i,i2%i,i3%i)",m1,m2,m3);');
 % cc = maple('limit(subs(s=x,d)/x,x=infinity);');
 cc = maple('limit(eval(d,u(j1,j2,j3,c)=XXX)/XXX,XXX=infinity);');

 len = len + size(cc,2);
 if len > 90
   fprintf(' \\\n        ');
   len=size(cc,2);
 end
 fprintf('%s',cc);
 if m1+m2+m3 ~=3 
   fprintf(',');
 end

end
end
end
fprintf(' )\n\n');


end % for k

% ******************** 4th order **********************


maple('ur4(i1,i2,i3,c) :=(8.*(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)) -(u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)));');
maple('us4(i1,i2,i3,c) :=(8.*(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)) -(u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)));');
maple('ut4(i1,i2,i3,c) :=(8.*(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)) -(u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)));');
maple('urr4(i1,i2,i3,c):=-30*u(i1,i2,i3,c)+16*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))-(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c));');
maple('uss4(i1,i2,i3,c):=-30*u(i1,i2,i3,c)+16*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))-(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c));');
maple('utt4(i1,i2,i3,c):=-30*u(i1,i2,i3,c)+16*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))-(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c));');

maple('urs4(i1,i2,i3,c) :=(8.*(subs(i2=i2+1,ur4(i1,i2,i3,c))-subs(i2=i2-1,ur4(i1,i2,i3,c)))-(subs(i2=i2+2,ur4(i1,i2,i3,c))-subs(i2=i2-2,ur4(i1,i2,i3,c))));');
maple('urt4(i1,i2,i3,c) :=(8.*(subs(i3=i3+1,ur4(i1,i2,i3,c))-subs(i3=i3-1,ur4(i1,i2,i3,c)))-(subs(i3=i3+2,ur4(i1,i2,i3,c))-subs(i3=i3-2,ur4(i1,i2,i3,c))));');
maple('ust4(i1,i2,i3,c) :=(8.*(subs(i3=i3+1,us4(i1,i2,i3,c))-subs(i3=i3-1,us4(i1,i2,i3,c)))-(subs(i3=i3+2,us4(i1,i2,i3,c))-subs(i3=i3-2,us4(i1,i2,i3,c))));');


for k=1:2 % ------------- 


if k==1 

nd=2;

fprintf(' **** 2d xy fourth order curvilinear *****\n');  
maple('d:=rxry*urr4(i1,i2,i3,c)+(rx*sy+ry*sx)*urs4(i1,i2,i3,c)+sx*sy*uss4(i1,i2,i3,c)+rxy*ur4(i1,i2,i3,c)+sxy4*us4(i1,i2,i3,c);');

elseif k==2

nd=3;

fprintf(' **** 3d xy fourth order curvilinear *****\n');  
maple('d:=rx*ry*urr4(i1,i2,i3,c)+sx*sy*uss4(i1,i2,i3,c)+tx*ty*utt4(i1,i2,i3,c)+(rx*sy+ry*sx)*urs4(i1,i2,i3,c)+(rx*ty+ry*tx)*urt4(i1,i2,i3,c)+(sx*ty+sy*tx)*ust4(i1,i2,i3,c)+rxy43*ur4(i1,i2,i3,c)+sxy43*us4(i1,i2,i3,c)+txy43*ut4(i1,i2,i3,c);');


end

if nd == 2 
  fprintf('        loopBody4thOrder2d(\\\n        ');
  m3b=0;
  maple('m3:=-1;');
else
  fprintf('        loopBody4thOrder3d(\\\n        ');
  m3b=2;
  maple('m3:=-3;');
end

len = 0;

for m3=-m3b:m3b
maple('m3:=m3+1;');
maple('j3:=i3Name[m3+3];');

maple('m2:=-3;');
for m2=-2:2
maple('m2:=m2+1;');
maple('j2:=i2Name[m2+3];');

maple('m1:=-3;');
for m1=-2:2
maple('m1:=m1+1;');
maple('j1:=i1Name[m1+3];');

 % maple('s:=sprintf("u(i1%i,i2%i,i3%i)",m1,m2,m3);');
 % cc = maple('limit(subs(s=x,d)/x,x=infinity);');
 cc = maple('limit(eval(d,u(j1,j2,j3,c)=XXX)/XXX,XXX=infinity);');

 len = len + size(cc,2);
 if len > 90
   fprintf(' \\\n        ');
   len=size(cc,2);
 end
 fprintf('%s',cc);
 if m1+m2+m3 ~= nd*2
   fprintf(',');
 end

end
end
end
fprintf(' )\n\n');


end % for k
