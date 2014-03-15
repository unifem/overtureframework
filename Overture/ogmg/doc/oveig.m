%
% Determine the eigenvalues and eigenvectors of the 1d overlapping grid Laplace operator
%
%  Usage:
%      oveig(m,n,delta,iorder,xa,xb,xm)   
% where: 
%    m : number of points on left 
%    n : number of points on right
%    delta : overlap distance is d*hv
%    iorder : order of interpolation (2 or 3)
%    xa,xb,xm
%
% Examples:
%    oveig(11,11,1.25,2);  ok 
%    oveig(11,11,1.25,3);  ok
%    oveig(11,17,2.25,2);  ok
%    oveig(11,17,3.,3);    ok : 
%    oveig(11,17,2.25,3);  **** Not right: large residuals implicit-interp ******
%
%
%
%     u(1)                  u(m+1)
%      1  2  3 ..        m  m+1
%      +--+--+--+--+--+--+--+                
%                        +--+--+--+--+--+--+--+
%                        1  2  3  4 ...    n  n+1
%                       v(1)                  v(n+1)
%      |                 |  |                 |
%     x=xa               xc xd                xb 
%                          |
%                         xm
%
% Numbering for the matrix with end-points eliminated:
%
%         1  2  3 ..    m-1    
%      X--+--+--+--+--+--+--X                
%                        X--+--+--+--+--+--+--X
%                           m  m+1  ...   m+n-2   
%
%         d = xd - xc 
%         hu=(xd-xa)/m;
%         hv=(xb-xc)/n;
%
%         d = delta*hv   : defines delta 
%         hv = (xb-xc)/n = d/delta
% 
% Current:
%         xc = .5 - .5*d
%         xd = .5 + .5*d
%
%         ( xb - .5 + .5*d )/n  = d/delta
%    =>   d*(1/delta - .5/n) = (xb-.5)/n 
%    =>   d = (xb-.5)/( n/delta -.5 )
%    =>   d = 1/( 2*(n)/delta -1);          % ... if xb=1 
%     
% ==================== cgEigs FUNCTION =====================
function oveig( m,n,delta,iorder,xa,xb,xm )

  % clear;
  clf;

  % set deaults if not provided
  if nargin < 1, m=11; end;
  if nargin < 2, n=11; end;
  if nargin < 3, delta=.75; end;
  if nargin < 4, iorder=2; end;
  if nargin < 5, xa=0.; end;
  if nargin < 6, xb=1.; end;
  if nargin < 7, xm=.5; end;

  set(gca,'FontSize',16);

  savePlots = 0; % =1 : save plots
  
  % iorder=2;
  % iorder=3;
  
  
  % m = 11;  % left interval
  % n = 11;  % right interval
  
  % m=21;
  % n=21;
  
  % -- check me with delta=2.75 -- eig's don't seem correct 
  % m = 11;
  % n = 17;
  
  %  m = 10;
  %  n = 10;
  
  
  % xa=0.;
  % xb=1.;
  % xm=.5; 
  
  %delta=1.; % overlap is delta*h
  %identifier='d1p0';  % d=1.0h
  
  % delta=.5;  % overlap is .5*h
  % identifier='dp5';  % d=.5h
  
  % delta=1.5178; % overlap is .5*h
  % identifier='d1p5178';  % d=.5h
  
  % delta=1.25;  % overlap is 1.25*h
  % identifier='d1p25'; 
  
  % --- check eigenfunction here at interp pts 
  % delta=2.25;  % overlap is 2.25*h
  % identifier='d2p25'; 
  
  %  delta=2.75
  %  identifier='d2p75';  % d=2.75h
  % d=5.4175/(m+n-1);
  
  identifier='         ';
  sprintf(identifier,'d%4.2f',delta);  % **** CHECK ME ****

  d = 1/( 2*(n)/delta -1);  % here is d
  
  xd=.5+.5*d;
  xc=.5-.5*d;
  
  hu=(xd-xa)/m;
  hv=(xb-xc)/n;
  

  fprintf(' *************  Laplacian Eigenvalues on a 1D Overlapping  Grid ***************\n');
  fprintf(' m=%d, n=%d, iorder=%d, delta=%9.3e, xa=%8.2e xb=%8.2e, xm=%8.2e \n',m,n,iorder,delta,xa,xb,xm)


  for i=1:m+1
    xu(i)=xa+(i-1)*hu;
    u(i)=sin(pi*xu(i));   % here is the lowest frequency eigenvector 
  end
  
  for j=1:n+1
   xv(j)=xc+(j-1)*hv;
   v(j)=sin(pi*xv(j));
  end
  
  % plot(xu,u,'r-o',xv,v,'b-x');
  
  %title('Multigrid Convergence');
  %ylabel('maximum residual');
  %xlabel('multigrid cycle');
  %set(gca,'YScale','log');
  
  % interpolation:  
  %    u(m+1)=alpha*v(p)+(1-alpha)*v(p+1)
  %    v(1) = beta*u(q)+(1-beta)*u(q+1)
  % p=m;   % v(2)
  % alpha=1.;
    
  if iorder==2 
    p = (xd-xc)/hv+m-1;
    alpha=p-fix(p);
    p=fix(p);
  
    q = (xc-xa)/hu;
    beta=q-fix(q);
    q=fix(q);
  else
  
    pv = (xd-xc)/hv + m-1; 
    p = max(fix( pv+.5 -1 ),m-1);  % left end of interp stencil, usually closest pt minus 1
    alpha=pv-p;
    
    qv = (xc-xa)/hu;
    q  = min(fix(qv+.5-1),m-2);  % left end of interp stencil, usually closest pt minus 1
    beta=qv-q;
  end

  fprintf(' Interpolation: u(m  ) = p1(a)*v(p)+p2(a)*v(p+1)+... m=%3d, p=%3d, a=%8.2e\n',m,p,alpha);
  fprintf(' Interpolation: v(m-1) = p1(b)*u(q)+p2(b)*u(q+1)+... m=%3d, q=%3d, a=%8.2e\n',m,q,beta);
  
  % pause
  
  husq=hu*hu;
  hvsq=hv*hv;
  
  adim=m+n-2;
  a=zeros(adim,adim);
  
  i=1;
  a(i,i)=-2./husq;
  a(i,i+1)=1./husq;
  for i=2:m-1
    a(i,i-1)=1./husq;
    a(i,i)=-2./husq;
    a(i,i+1)=1./husq;
  end 
  ia=m-1;         % last active point on left grid
  a(ia,ia+1)=0.;
  ib=m;           % first active point on right grid
  
  
  
  i=m;
  a(i,i)=-2./hvsq;
  a(i,i+1)=1./hvsq;
  for i=m+1:m+n-3
    a(i,i-1)=1./hvsq;
    a(i,i)=-2./hvsq;
    a(i,i+1)=1./hvsq;
  end 
  i=m+n-2;
  a(i,i-1)=1./hvsq;
  a(i,i)=-2./hvsq;
  
  % initialize all interpolation equations to zero
  cu1=0; p1=p;
  cu2=0; p2=p;
  cu3=0; p3=p;
  cu4=0; p4=p;
  
  cv1=0; q1=q;
  cv2=0; q2=q;
  cv3=0; q3=q;
  cv4=0; q4=q;
  
  
  if iorder==3
    % interpolation weights
    a1= .5*(1.-alpha)*(2-alpha); 
    a2=    (   alpha)*(2-alpha); 
    a3=-.5*(   alpha)*(1-alpha); 
    b1= .5*(1.-beta)*(2-beta);   
    b2=    (   beta)*(2-beta);   
    b3=-.5*(   beta)*(1-beta);   
  end
  
  % ************* Interpolation equations ****************
  % if p>=m & q<=(m-2)
  if p>=m & q+iorder<=m
    % explicit interpolation
    fprintf('**** EXPLICIT interpolation****\n');
  
    if iorder==2 
      a(ia,p  )=(1.-alpha)/husq;
      a(ia,p+1)=    alpha /husq;
      a(ib,q)=(1.-beta)/hvsq;
      a(ib,q+1)=  beta /hvsq;
      % save interp coeff to be used below
      p1=p; cu1=(1.-alpha); p2=p+1; cu2=alpha; p3=p+1; cu3=0.; p4=m; cu4=0;
      q1=q; cv1=(1.-beta ); q2=q+1; cv2=beta;  q3=q+1; cv3=0.; q4=m; cv4=0;
  
    else
      
      cu1=a1; p1=p;
      cu2=a2; p2=p+1;
      cu3=a3; p3=p+2;
      cu4=0 ; p4=p+2;
      
      cv1=b1; q1=q;
      cv2=b2; q2=q+1;
      cv3=b3; q3=q+2;
      cv4=0;  q4=q+2;
  
      a(ia,p1)=cu1/husq;
      a(ia,p2)=cu2/husq;
      a(ia,p3)=cu3/husq;   % bug found 090907 (was cu2)
  
      a(ib,q1)=cv1/hvsq;
      a(ib,q2)=cv2/hvsq;
      a(ib,q3)=cv3/hvsq;
    end 
  
  else
    % implicit interpolation
  
    fprintf('**** IMPLICIT interpolation****\n');
  
    if p==(m-1) & q==(m-1 -iorder+2) % both sides are implicit
      if iorder==2 
        den=1-beta*(1-alpha);
        a(ia,m-1)=a(ia,m-1)+(1-alpha)*(1-beta)/(den*husq);
        a(ia,m  )=a(ia,m  )+   alpha          /(den*husq);
        a(ib,m-1)=a(ib,m-1)+(1-beta)  /(den*hvsq);
        a(ib,m  )=a(ib,m  )+alpha*beta/(den*hvsq);
  
        % save interp coeff to be used below
        p1=m-1; cu1=(1-alpha)*(1-beta)/den; p2=m; cu2=alpha/den;       p3=m; cu3=0.; p4=m; cu4=0;
        q1=m-1; cv1=(1-beta)/den;           q2=m; cv2=alpha*beta/den;  q3=m; cv3=0.; q4=m; cv4=0;
  
      else
        den=1-b3*a1;
  
        cu1=a1*b1/(den); p1=q;
        cu2=a1*b2/(den); p2=q+1;
        cu3=a2   /(den); p3=p+1;
        cu4=a3   /(den); p4=p+2;
  		    
        cv1=b1   /(den); q1=q;
        cv2=b2   /(den); q2=q+1;
        cv3=a2*b3/(den); q3=p+1;
        cv4=a3*b3/(den); q4=p+2;
  
        a(ia,p1)=a(ia,p1)+cu1/husq;
        a(ia,p2)=a(ia,p2)+cu2/husq;
        a(ia,p3)=a(ia,p3)+cu3/husq;
        a(ia,p4)=a(ia,p4)+cu4/husq;
  			     
        a(ib,q1)=a(ib,q1)+cv1/hvsq;
        a(ib,q2)=a(ib,q2)+cv2/hvsq;
        a(ib,q3)=a(ib,q3)+cv3/hvsq;
        a(ib,q4)=a(ib,q4)+cv4/hvsq;
  
      end 
    elseif p==(m-1) & q<(m-1-iorder+2)
      if iorder==2 
        a(ia,q  )=a(ia,q  )+(1-alpha)*(1-beta)/husq;
        a(ia,q+1)=a(ia,q+1)+(1-alpha)*   beta /husq;
        a(ia,p+1)=a(ia,p+1)+   alpha          /husq;
        a(ib,q  )=(1.-beta)/hvsq; % same as default case 
        a(ib,q+1)=    beta /hvsq; % same as default case
      
        % save interp coeff to be used below
        p1=q; cu1=(1.-alpha)*(1-beta); p2=q+1; cu2=(1-alpha)*beta; p3=p+1; cu3=alpha; p4=p3; cu4=0;
        q1=q; cv1=(1.-beta ); q2=q+1; cv2=beta;  q3=q+1; cv3=0.; q4=q3; cv4=0;
  
      else
  
        den=1-b3*a1;
  
        cu1=a1*b1/(den); p1=q;
        cu2=a1*b2/(den); p2=q+1;
        cu3=a2   /(den); p3=p+1;
        cu4=a3   /(den); p4=p+2;
  
        cv1=b1; q1=q;
        cv2=b2; q2=q+1;
        cv3=b3; q3=q+2;
        cv4=0;  q4=q+2;
  
        a(ia,p1)=a(ia,p1)+cu1/husq;
        a(ia,p2)=a(ia,p2)+cu2/husq;
        a(ia,p3)=a(ia,p3)+cu3/husq;
        a(ia,p4)=a(ia,p4)+cu4/husq;
  
        a(ib,q1)=cv1/hvsq;
        a(ib,q2)=cv2/hvsq;
        a(ib,q3)=cv3/hvsq;
  
      end
  
    elseif p >(m-1) & q==(m-1 -iorder+2)
      if iorder==2 
        a(ia,p  )=(1.-alpha)/husq; % same as default case 
        a(ia,p+1)=    alpha /husq; % same as default case 
      
        a(ib,p  )=a(ib,p  )+   beta*(1-alpha)/hvsq;
        a(ib,p+1)=a(ib,p+1)+   beta*   alpha /hvsq;
        a(ib,q)  =a(ib,q)+  (1-beta)         /hvsq;
        % save interp coeff to be used below
        p1=p; cu1=(1.-alpha); p2=p+1; cu2=alpha; p3=p+1; cu3=0.; p4=p3; cu4=0;
        q1=p; cv1=beta*(1-alpha); q2=p+1; cv2=beta*alpha;  q3=q; cv3=(1-beta);q4=q3; cv4=0;
      else
  
        cu1=a1; p1=p;
        cu2=a2; p2=p+1;
        cu3=a3; p3=p+2;
        cu3=0;  p4=p+2;
  
        den=1-b3*a1;
  
        cv1=b1   /(den); q1=q;
        cv2=b2   /(den); q2=q+1;
        cv3=a2*b3/(den); q3=p+1;
        cv4=a3*b3/(den); q4=p+2;
  
        a(ia,p1)=cu1/husq;
        a(ia,p2)=cu2/husq;
        a(ia,p2)=cu3/husq;
  			     
        a(ib,q1)=a(ib,q1)+cv1/hvsq;
        a(ib,q2)=a(ib,q2)+cv2/hvsq;
        a(ib,q3)=a(ib,q3)+cv3/hvsq;
        a(ib,q4)=a(ib,q4)+cv4/hvsq;
  
      end
    else
      fprintf('ERROR: unexpected case\n');
      pause;
    end 
  end 
  
  %  a
  %  pause
  
  [v,L] = eig(a);
  
  % Generalized: 
  % [V,D] = EIG(A,B)

  % ============== Now plot the results =======================
  
  for i=1:adim
    lambda(i)=L(i,i);
  end
  
  % sort from smallest to largest in absolute value
  [zz,ia] = sort(-lambda);
  
  uTrue=zeros(m+2,1);
  vTrue=zeros(n+2,1);
  
  eu=zeros(m+1,1);
  ev=zeros(n+1,1);
  
  % ------  Build eigenfunctions for plotting -------------
  maxDiff=0.; 
  epsLambda=1.e-12; 
  numComplex=0; 
  for j=1:m+n-2  % loop over eigenfunctions
    % ie=m+n-1-j;
    ie=ia(j);
  
    lam = lambda(ie);
  
    fprintf(' j=%3i ie=%3i lambda(ie)=( %9.2e , %9.2e )',j,ie,real(lam),imag(lam));
    if abs(imag(lam)) > 0 
     numComplex=numComplex+1;
    end;
  
   vu(1,j)=0;
   for i=2:m  
     vu(i,j)=v(i-1,ie);
   end 
   for i=2:n
     vv(i,j)=v(i+m-2,ie);
   end
   vv(n+1,j)=0.;
  
  
   vu(m+1,j)=cu1*v(p1,ie)+cu2*v(p2,ie)+cu3*v(p3,ie)+cu4*v(p4,ie);  % interpolated value
  
   vv(  1,j)=cv1*v(q1,ie)+cv2*v(q2,ie)+cv3*v(q3,ie)+cv4*v(q4,ie);   % interpolated value 
  
  
   % ==== DOUBLE CHECK RESULTS ====
    
  
   diff=0.;
   for i=2:m  
     diff = diff + abs( (vu(i+1,j)-2.*vu(i,j)+vu(i-1,j))/husq - lam*vu(i,j) );
   end
   for i=2:n
     diff = diff + abs( (vv(i+1,j)-2.*vv(i,j)+vv(i-1,j))/hvsq - lam*vv(i,j) );
   end
   fprintf(', err(A*v-lambda*v)=%8.2e',diff);
   if diff > epsLambda*(1.+abs(lam))
    fprintf(' ****\n');
   else
    fprintf('\n');
   end;
   maxDiff=max(maxDiff,diff);
  
   % arrange the eigenfunctions so they always are initially positive (easier then to compare)
   if vu(2,j) < 0 
     vu(:,j)=vu(:,j)*(-1);
     vv(:,j)=vv(:,j)*(-1);
   end 
  
   if 0 == 1 
     % NOTE
     uTrue=sin(pi*j*xu);
     vTrue=sin(pi*j*xv);
    
     alpha=vu(:,j).'*uTrue.'/(vu(:,j).'*vu(:,j));
    
     vu(:,j)=vu(:,j)*alpha; % scale to match uTrue: min || uTrue - alpha*v || 
     eu(:,j)=vu(:,j)-uTrue.';  % error in the eigenfunction
    
     % NO: do not scale vu and vv differently alpha=vv(:,j).'*vTrue.'/(vv(:,j).'*vv(:,j));
     vv(:,j)=vv(:,j)*alpha; % scale to match uTrue: min || uTrue - alpha*v || 
     ev(:,j)=vv(:,j)-vTrue.';  % error in the eigenfunction
  
     eigerr=max(max(abs(eu(:,j))),max(abs(ev(:,j))));
     fprintf(' ... error in eig=%8.2e\n',eigerr);
  
   end; 
  
  
   % -- shift the eigenfunctions for plotting ---
   vu(:,j)=vu(:,j)*.5+j;
   vv(:,j)=vv(:,j)*.5+j;
  
  
  
   if 0==1
     plot(xu,vu(:,j),'r-o',xv,vv(:,j),'b-x');
     title(sprintf('Eigenfunction %i, \\lambda=%8.2e,\n m=%i, n=%i, d=%3.2fh_1=%3.2fh_2',j,lambda(ie),m,n,d/hu,d/hv));
     pause
  
     plot(xu,eu(:,j),'r-o',xv,ev(:,j),'b-x');
     title(sprintf('Error in Eigenfn %i, \\lambda=%8.2e,\n m=%i, n=%i, d=%3.2fh_1=%3.2fh_2',j,lambda(ie),m,n,d/hu,d/hv));
     pause
   end
  end
  
  if numComplex>0 
    fprintf(' **** There were %d complex eigenvalues ***\n',numComplex);
  end;
  if maxDiff > epsLambda*(1.+abs(lam))
    fprintf(' *********** WARNING: There is a large residual in A*v-lambda*v ************\n');
  end;
  % =========== plot eigenvalues =============
  plot( real(lambda),imag(lambda),'bo' );
  title(sprintf('Eigenvalues n=%d m=%d \\delta=%8.2e interp=%d',m,n,delta,iorder));
  xlabel('Real');
  ylabel('Imaginary');
  grid on;
  pause; 
  
  
  
  if 0==1 
    k=0;
    for j=1:4:m+n-2
     k=k+1;
     plot(xu,vu(:,j  ),'r-o',xv,vv(:,j  ),'r-x', ...
          xu,vu(:,j+1),'g-o',xv,vv(:,j+1),'g-x', ...
          xu,vu(:,j+2),'m-o',xv,vv(:,j+2),'m-x', ...
          xu,vu(:,j+3),'b-o',xv,vv(:,j+3),'b-x');
     title(sprintf('Eigenfunctions %i-%i, m=%i, n=%i, d=%3.2fh_1=%3.2fh_2',j,j+3,m,n,d/hu,d/hv));
  
  fprintf(' **************** LEGEND! ********');
  
     legend(sprintf('e%i (left)',j  ),sprintf('e%i (right)',j  ),...
            sprintf('e%i (left)',j+1),sprintf('e%i (right)',j+1),...
            sprintf('e%i (left)',j+2),sprintf('e%i (right)',j+2),...
            sprintf('e%i (left)',j+3),sprintf('e%i (right)',j+3))
    
      name = sprintf('eigenfunction-%s-%i.eps',identifier,k);
    
     if savePlots == 1 
      fprintf('Save %s\n',name);
      print('-depsc2',name);
     end;
    
     pause
    end
  else
    k=0;
    for j=1:10:m+n-2
     k=k+1;
     plot(xu,real(vu(:,j  )),'r-o',xv,real(vv(:,j  )),'r-x', ...
          xu,real(vu(:,j+1)),'g-o',xv,real(vv(:,j+1)),'g-x', ...
          xu,real(vu(:,j+2)),'m-o',xv,real(vv(:,j+2)),'m-x', ...
          xu,real(vu(:,j+3)),'b-o',xv,real(vv(:,j+3)),'b-x', ...
          xu,real(vu(:,j+4)),'r-o',xv,real(vv(:,j+4)),'r-x');
     hold on;
     plot(xu,real(vu(:,j+5)),'g-o',xv,real(vv(:,j+5)),'g-x', ...
          xu,real(vu(:,j+6)),'m-o',xv,real(vv(:,j+6)),'m-x', ...
          xu,real(vu(:,j+7)),'b-o',xv,real(vv(:,j+7)),'b-x', ...
          xu,real(vu(:,j+8)),'r-o',xv,real(vv(:,j+8)),'r-x', ...
          xu,real(vu(:,j+9)),'g-o',xv,real(vv(:,j+9)),'g-x' );
  
     title(sprintf('Eigenfunctions (real part) %i-%i, N_1=%i, N_2=%i, d=%3.2fh_1=%3.2fh_2',j,j+9,m,n,d/hu,d/hv));
     hold off;
  
      name = sprintf('eigenfunction%i-%i-%s-%i.eps',m,n,identifier,k);
    
     if savePlots == 1 
       fprintf('Save %s\n',name);
       print('-depsc2',name);
     end;
  
     pause
    end
  
  end
  
  
  
  % print -deps2 eigenfunction.eps
  % print -depsc2 eigenfunction.eps
  
  
  
% ==================== END FUNCTION =====================
end
