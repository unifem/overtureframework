%
% Compute errors for the rigid body routine
%

if checkErrors==1
  numErr=0; % counts error components
  errv = zeros(n,5);
end;

if checkErrors==1
  if itest==polynomialForcing
    errMaxx=0.; errMaxv=0.; errMaxh=0.; 
    for i=1:n
      t = tv(i);

      xv = [ wv(i,mx);  wv(i,mx+1); wv(i,mx+2) ];
      vv = [ wv(i,mv);  wv(i,mv+1); wv(i,mv+2) ];
      hv = [ wv(i,mh);  wv(i,mh+1); wv(i,mh+2) ];

      for j=1:3
        xve(j) = xv0(j) + vv0(j)*t + ( t^2*( .5*pf(j,1) + t*(pf(j,2)/(2.*3.) + t*(pf(j,3)/(3.*4.))) ) )/mass;
        vve(j) = vv0(j) + ( t*( pf(j,1) + t*(pf(j,2)/(2.) + t*(pf(j,3)/(3.))) ) )/mass;
        hve(j) = hv0(j) + t*( pg(j,1) + t*(pg(j,2)/(2.) + t*(pg(j,3)/(3.))) );
      end;    

      errMaxx=max(errMaxx,max(abs(xv(1)-xve(1)),max(abs(xv(2)-xve(2)),abs(xv(3)-xve(3)))));
      errMaxv=max(errMaxv,max(abs(vv(1)-vve(1)),max(abs(vv(2)-vve(2)),abs(vv(3)-vve(3)))));
      errMaxh=max(errMaxh,max(abs(hv(1)-hve(1)),max(abs(hv(2)-hve(2)),abs(hv(3)-hve(3)))));


    end;
    fprintf('Max. err in (x,v,h) = (%8.2e,%8.2e,%8.2e) [polynomial forcing].\n',errMaxx,errMaxv,errMaxh);

  elseif itest==twilightZoneSolution
    % twilightZone
    numErr=numErr+1; nErrv=numErr;  
    %                  '123456789012345'
    errorName(nErrv,:)='v-err          ';
    numErr=numErr+1; nErrh=numErr;  
    errorName(nErrh,:)='h-err          ';

    errMaxx=0.; errMaxv=0.; errMaxh=0.; 
    for i=1:n
      t = tv(i);

      xv = [ wv(i,mx);  wv(i,mx+1); wv(i,mx+2) ];
      vv = [ wv(i,mv);  wv(i,mv+1); wv(i,mv+2) ];
      hv = [ wv(i,mh);  wv(i,mh+1); wv(i,mh+2) ];
      ea = [ wv(i,me  ) wv(i,me+3) wv(i,me+6); wv(i,me+1) wv(i,me+4) wv(i,me+7); wv(i,me+2) wv(i,me+5) wv(i,me+8) ];

      [xve, vve, omegave ] = getExactSolution( 0, t ); 

      % hvExact = A*omegavExact  : we use computed A with exact omega
      A = ea*Lambda*ea';
      hve = A*omegave; 

      errv(i,nErrv) = max(abs(vv(1)-vve(1)),max(abs(vv(2)-vve(2)),abs(vv(3)-vve(3))));
      errv(i,nErrh) = max(abs(hv(1)-hve(1)),max(abs(hv(2)-hve(2)),abs(hv(3)-hve(3))));
      

      errMaxx=max(errMaxx,max(abs(xv(1)-xve(1)),max(abs(xv(2)-xve(2)),abs(xv(3)-xve(3)))));
      errMaxv=max(errMaxv,max(abs(vv(1)-vve(1)),max(abs(vv(2)-vve(2)),abs(vv(3)-vve(3)))));
      errMaxh=max(errMaxh,max(abs(hv(1)-hve(1)),max(abs(hv(2)-hve(2)),abs(hv(3)-hve(3)))));


    end;
    fprintf('Max. err in (x,v,h) = (%8.2e,%8.2e,%8.2e) [TZ=%s].\n',errMaxx,errMaxv,errMaxh,twilightZoneName);
     
  end;
end;



% Check if h = A*omega 
%   A = E Lambda E^T

if checkErrors==1
  errMax=0.;
  for i=1:n
    h = [ wv(i,mh);  wv(i,mh+1); wv(i,mh+2) ];
  
    ea = [ wv(i,me  ) wv(i,me+3) wv(i,me+6); wv(i,me+1) wv(i,me+4) wv(i,me+7); wv(i,me+2) wv(i,me+5) wv(i,me+8) ];
    aa = ea*Lambda*ea';
    omega = [ wv(i,mo);  wv(i,mo+1); wv(i,mo+2) ];
    h2 = aa*omega;
    errMax=max( errMax, max(abs(h(1)-h2(1)) ,max(abs(h(2)-h2(2)), abs(h(3)-h2(3)) ) ) );
    if debug > 1
      fprintf(' i=%d : h=[%8.2e,%8.2e,%8.2e] , A*omega =[%8.2e,%8.2e,%8.2e]  err=[%8.2e,%8.2e,%8.2e]\n',...
          i, h(1),h(2),h(3), h2(1),h2(2),h2(3), h(1)-h2(1),h(2)-h2(2),h(3)-h2(3) );
    end;
  end;
  fprintf('Max err in |h-A*omega| = %8.2e\n',errMax);
  if debug > 1
    pause;
  end;
end

% Rotation matrix = E*E(0)'

% Check if the Rotation matrix agrees with that from the Quaternion
if checkErrors==1
  errMax=0.; 
  for i=1:n
  
    ea = [ wv(i,me  ) wv(i,me+3) wv(i,me+6); wv(i,me+1) wv(i,me+4) wv(i,me+7); wv(i,me+2) wv(i,me+5) wv(i,me+8) ];
    Ra = ea*ea0';
  
    qv = [ wv(i,mq);  wv(i,mq+1); wv(i,mq+2); wv(i,mq+3) ];  % Quaternion
    R = quaternionToMatrix(qv);
  
    errMax = max( errMax, max(max(abs(Ra-R))));

    if 1==0
      fprintf('Ra*Ra^T-I (direct)\n');
      Ra*Ra'-eye(3)
      fprintf('R*R^T-I (quaternion)\n');
      R*R'-eye(3)
    end;
  
    qNorm = sqrt( qv(1)^2 + qv(2:4)'*qv(2:4) );
  
    if debug > 1
      for j=1:3
        fprintf(' i=%d t=%9.3e : j=%d |q|=%8.2e, R=[%8.2e,%8.2e,%8.2e] Quaternion =[%8.2e,%8.2e,%8.2e]  err=[%8.2e,%8.2e,%8.2e]\n',...
            i, tv(i), j, qNorm, ...
               Ra(1,j),Ra(2,j),Ra(3,j), ...
               R(1,j) ,R(2,j) ,R(3,j), ...
               Ra(1,j)-R(1,j), Ra(2,j)-R(2,j), Ra(3,j)-R(3,j));
      end;
    end;
  end;
  fprintf('Max err in |R-Quaternion| = %8.2e\n',errMax);

  if debug > 1
    pause;
  end;

end;

if itest==freeRotation
  % free rotation test 
  % -- check the error --
  errMax = 0.;
  numErr=numErr+1;   % we save this error
  %                   '123456789012345'
  errorName(numErr,:)='omegaHat-err   ';

  if (freeRotationAxis==1 & I2~=I3) | (freeRotationAxis==2 & I1~=I3)| (freeRotationAxis==3 & I1~=I2)
    fprintf('free rotation test: ERROR: I1!=I2 or I1!=I3 or I2!=I3.\n');
    pause;
  end;

  Iv = [I1 I2 I3];
  i3 = freeRotationAxis;
  i1= mod(i3,3)+1;
  i2= mod(i1,3)+1;
  freq = abs(omegav0(i3)*(Iv(i3)-Iv(i1))/Iv(i1));
  scale =   (omegav0(i3)*(Iv(i3)-Iv(i1))/Iv(i1))/freq;   % sign( omegav0(3)*(I3-I1)/I1 )
  for i=1:n

    ea = [ wv(i,me  ) wv(i,me+3) wv(i,me+6); wv(i,me+1) wv(i,me+4) wv(i,me+7); wv(i,me+2) wv(i,me+5) wv(i,me+8) ];
    omega = [ wv(i,mo);  wv(i,mo+1); wv(i,mo+2) ];

    omegaHat = ea'*omega;

    % Exact solution:
    t=tv(i);
    omegae(i1) = omegav0(i1)*cos(freq*t) -scale*omegav0(i2)*sin(freq*t);
    omegae(i2) = omegav0(i2)*cos(freq*t) +scale*omegav0(i1)*sin(freq*t);
    omegae(i3) = omegav0(i3);

    if debug > 1
      fprintf(strcat('Free rotation(%i): i=%d t=%9.3e omegaHat=[%8.2e,%8.2e,%8.2e] true=[%8.2e,%8.2e,%8.2e]',...
              ' err=[%8.2e,%8.2e,%8.2e]\n'),...
               i3,i,tv(i),omegaHat(1),omegaHat(2),omegaHat(3),omegae(1),omegae(2),omegae(3), ...
               omegaHat(1)-omegae(i1),omegaHat(2)-omegae(i2),omegaHat(3)-omegae(i3) );

    end;

    errv(i,numErr) = max(abs(omegaHat(1)-omegae(1)), ...
                     max(abs(omegaHat(2)-omegae(2)),...
                         abs(omegaHat(3)-omegae(3)) ));

    errMax = max( errMax, errv(i,numErr) );

  end;

  fprintf('Free rotation(%i): Max. error in omegaHat = %8.2e\n',i3,errMax);

  if debug > 1
    pause;
  end;
end;
