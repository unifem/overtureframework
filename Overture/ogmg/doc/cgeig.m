%
% Determine the eigenvalues and eigenvectors of the 1d overlapping grid Laplace operator
%       *NEW* version using the generalized eigenvalue solver 
%
%  Usage:
%      cgeig(m,n,delta,iorder,xa,xb,xm,xc,xd)   
% where: 
%    m : number of points on left 
%    n : number of points on right
%    delta : overlap distance is d*hv
%    iorder : order of interpolation (2 or 3)
%    xa,xb,xm
%
% Examples:
%    cgeig(5,5,.5,2);
%    cgeig(5,5,.5,3);
%
%    cgeig(11,11,1.25,2);  ok 
%    cgeig(11,11,1.25,3);  ok
%    cgeig(11,17,2.25,2);  ok
%    cgeig(11,17,3.,3);    2 complex
%    cgeig(11,17,2.25,3);  ok
%    cgeig(13,19,2.35,3);  2 complex
%    cgeig(13,5,1.892,3);  2 complex
%
%   -- specify xc and xd 
%     cgeig(21,6,-1.,3,0.,1.,-1.,.75,.8);   ok 
%     cgeig(21,6,-1.,3,0.,1.,-1.,.715,.82);
%     cgeig(21,8,-1.,3,0.,1.,-1.,.715,.82);  2 complex 
%     cgeig(21,8,-1.,3,0.,1.,-1.,.715,.825); 2 complex
%     cgeig(21,10,-1.,3,0.,1.,-1.,.719,.82); 2 complex alpha=.59, beta=1.41 :  modes 22,23 of 29
%     cgeig(21,10,-1.,3,0.,1.,-1.,.74,.82);  2 complex alpha=1.08, beta=.95 : modes 13,14 of  29
%     cgeig(31,20,-1.,3,0.,1.,-1.,.719,.82); 4 complex, a=1.19, b=1.18,  : modes 21,22, 34,35 of 49
%     cgeig(31,20,-1.,3,0.,1.,-1.,.79,.8);   ok 
%     cgeig(31,20,-1.,3,0.,1.,-1.,.589,.82);  10 complex, large overlap
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
function cgeig( m,n,delta,iorder,xa,xb,xm,xc,xd )

  % clear;
  clf;
  set(gca,'FontSize',16);

  savePlots = 0; % =1 : save plots
  plotEigenfunctions = 1;

  % set deaults if not provided
  if nargin < 1, m=11; end;
  if nargin < 2, n=11; end;
  if nargin < 3, delta=.75; end;
  if nargin < 4, iorder=2; end;
  if nargin < 5, xa=0.; end;
  if nargin < 6, xb=1.; end;
  if nargin < 7, xm=.5; end;
  if nargin < 8, xc=1.e6; end;
  if nargin < 9, xd=-1.e6; end;

  %delta=1.; % overlap is delta*h
  %identifier='d1p0';  % d=1.0h
  
  if xd < xc 
    % xc and xd not supplied
    d = 1/( 2*(n)/delta -1);  % here is d
    xd=.5+.5*d;
    xc=.5-.5*d;
  else
    % xc and xd supplied
    d = xd-xc;
    hv=(xb-xc)/n;
    delta = d/hv;
    xm=(xc+xd)*.5; 
  end;
  
  hu=(xd-xa)/m;
  hv=(xb-xc)/n;
  
  identifier='         ';
  sprintf(identifier,'d%4.2f',delta);  % **** CHECK ME ****

  fprintf(' *************  Laplacian Eigenvalues on a 1D Overlapping  Grid ***************\n');
  fprintf(' m=%d, n=%d, iorder=%d, delta=%9.3e, xa=%8.2e xb=%8.2e, [xc,xd]=[%8.2e,%8.2e] xm=%8.2e \n',...
                m,n,iorder,delta,xa,xb,xc,xd,xm)


  for i=1:m+1
    xu(i)=xa+(i-1)*hu;  % grid points
    u(i)=sin(pi*xu(i));   % here is the lowest frequency eigenvector 
  end
  
  for j=1:n+1
   xv(j)=xc+(j-1)*hv;  % grid points
   v(j)=sin(pi*xv(j));
  end
  
  
  % interpolation:  
  %    u(m+1)=alpha*v(p)+(1-alpha)*v(p+1)
  %    v(1) = beta*u(q)+(1-beta)*u(q+1)
  % p=m;   % v(2)
  % alpha=1.;
    
  ia=m+1;       % interp pt on u grid  
  ib=m+2;       % interp pt on v grid 


  if iorder==2 
    pv = (xd-xc)/hv + ib;  % offset from ib 
    p= max( fix(pv), ib);

    alpha=pv-p;
  
    qv = (xc-xa)/hu + 1;   % offset from ia 
    q  = min( fix(qv), ia-iorder+1);
    beta=qv-q;
  else
  
    pv = (xd-xc)/hv + ib; 
    p = max(fix( pv+.5 -1 ),ib);  % left end of interp stencil, usually closest pt minus 1
    alpha=pv-p;
    
    qv = (xc-xa)/hu + 1;
    q  = min(fix(qv+.5-1),ia-iorder+1);  % left end of interp stencil, usually closest pt minus 1
    beta=qv-q;
  end

  fprintf(' Interpolation: u(m  ) = p1(a)*v(p)+p2(a)*v(p+1)+... m=%3d, p=%3d, a=%8.2e\n',m,p,alpha);
  fprintf(' Interpolation: v(m-1) = p1(b)*u(q)+p2(b)*u(q+1)+... m=%3d, q=%3d, b=%8.2e\n',m,q,beta);
  
  plot(xu,u,'r-o',xv,v,'b-x');
  title(sprintf('Grid and exact eigenfunction, \\delta=%4.2f \\alpha=%4.2f \\beta=%4.2f',delta,alpha,beta));
  pause;

  % pause
  
  husq=hu*hu;
  hvsq=hv*hv;
  
  adim=m+1 + n+1;
  a=zeros(adim,adim);
  b=zeros(adim,adim);
  
  i=1;
  a(i,i)=1; % dirichlet BC 
  for i=2:m
    a(i,i-1)= 1./husq;
    a(i,i  )=-2./husq;    b(i,i)=1.; % 
    a(i,i+1)= 1./husq;
  end 

  % interpolation equations: 
  if iorder==2
    cv1 = (1.-alpha);
    cv2 = alpha;
    cu1 = (1.-beta);
    cu2 = beta;
  else
    % interpolation weights
    cv1= .5*(1.-alpha)*(2-alpha); 
    cv2=    (   alpha)*(2-alpha); 
    cv3=-.5*(   alpha)*(1-alpha); 

    cu1= .5*(1.-beta)*(2-beta);   
    cu2=    (   beta)*(2-beta);   
    cu3=-.5*(   beta)*(1-beta);   
  end

  a(ia,ia)=-1.;
  a(ia,p  )=cv1;
  a(ia,p+1)=cv2;
  if iorder==3 , a(ia,p+2)=cv3; end; 

  a(ib,ib)=-1.;
  a(ib,q  )=cu1;
  a(ib,q+1)=cu2;
  if iorder==3 , a(ib,q+2)=cu3; end; 
  
  for i=m+3:m+n+1
    a(i,i-1)=1./hvsq;
    a(i,i) =-2./hvsq; b(i,i)=1.; % 
    a(i,i+1)=1./hvsq;
  end 
  i=m+n+2;
  a(i,i)=1.; % dirichlet BC 
  
  % display(a);
  % display(b);

  %pause;  % *****************************

  %  a
  %  pause
  
  % [v,L] = eig(a);
  
  % Generalized: 
  [v,L] = eig(a,b);


  % ============== Now plot the results =======================
  
  % There are 4 eigenvalue of inf that we ignore

  num = m+n+2 - 4;

  j=1;
  for i=1:adim
    if ~isinf(L(i,i))
      lambda(j)=L(i,i); j=j+1; 
    end;
  end
  
  %display(lambda);

  %pause; % ******************************************


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
  for j=1:num  % loop over eigenfunctions

    ie=ia(j);
    lam = lambda(ie);
  
    fprintf(' j=%3i ie=%3i lambda(ie)=( %9.2e , %9.2e )',j,ie,real(lam),imag(lam));
    if abs(imag(lam)) > 0 
     numComplex=numComplex+1;
    end;
  

   for i=1:m+1
     vu(i,j)=v(i,ie);
   end 
   for i=1:n+1
     vv(i,j)=v(i+m+1,ie);
   end
  
  
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
  
   % -- shift the eigenfunctions for plotting ---
   vu(:,j)=vu(:,j)*.5+j;
   vv(:,j)=vv(:,j)*.5+j;
  
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
  
  
  
  if plotEigenfunctions == 1 
    k=0;
    for j=1:10:num
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
