rho    = 1;
mu     = 1;
lambda = 1;

%%%%%
%% get orthonormal set (this will be done differently in the actual code and this is just a stand in)
%%%%%
alpha = rand(3,1);
alpha = alpha./(norm(alpha));
tau   = [alpha(2:3);alpha(1)];
[junk,im] = max(abs(tau));
tau(im) = -tau(im);
gamma = [tau(2:3);tau(1)];
tau = tau-(alpha'*tau)*alpha;
tau = tau/(norm(tau));
gamma = gamma-(alpha'*gamma)*alpha;
gamma = gamma-(tau'*gamma)*tau;
gamma = gamma/(norm(gamma));

if( abs(alpha'*tau)>1e-15 | abs(alpha'*gamma)>1e-15 | abs(tau'*gamma)>1e-15 )
  fprintf( 'did not get orthonormal set ... paused \n' );
  pause
end

%alpha = [0;0;1];
%tau   = [1;0;0];
%gamma = [0;1;0];

%alpha = [0.7339217605802030; -0.4919028628108554;  0.4683913138662371];
%tau   = [0.6688253952167923;  0.6436229701492710; -0.3720511564413314];
%gamma = [-0.1184543796571368; 0.5863284453740285;  0.8013660300295475];

%alpha = [0.0000000000000000E+000   1.000000000000000       0.0000000000000000E+000];
%tau   = [0.0000000000000000E+000  0.0000000000000000E+000   1.000000000000000 ];
%gamma = [1.000000000000000       0.0000000000000000E+000  0.0000000000000000E+000];

%rad = 1.2;
%alpha = rad*alpha;


f = eye(3,3)+1e-2*rand(3,3);

%CC = getCMat3D( rho,mu,lambda,alpha,f );
CC = getCMat3D_LIN( rho,mu,lambda,alpha,f );

BB = getBMat3D( alpha,CC );

%% get eigenvalues of 3x3 system (this will be a LINPACK call)
[V,D] = eig( BB )

R  = zeros(12,12);
Ri = zeros(12,12);
L  = zeros(12,1);

% 1st eigen-vector
L(1) = -sqrt(D(1,1)/rho);
R(1,1) = V(1,1);
R(2,1) = V(2,1);
R(3,1) = V(3,1);
for j = 4:12
  R(j,1) = (-CC(j,1)*R(1,1)-CC(j,2)*R(2,1)-CC(j,3)*R(3,1))/L(1);
end

% 2nd eigen-vector
L(2) = -sqrt(D(2,2)/rho);
R(1,2) = V(1,2);
R(2,2) = V(2,2);
R(3,2) = V(3,2);
for j = 4:12
  R(j,2) = (-CC(j,1)*R(1,2)-CC(j,2)*R(2,2)-CC(j,3)*R(3,2))/L(2);
end

% 3rd eigen-vector
L(3) = -sqrt(D(3,3)/rho);
R(1,3) = V(1,3);
R(2,3) = V(2,3);
R(3,3) = V(3,3);
for j = 4:12
  R(j,3) = (-CC(j,1)*R(1,3)-CC(j,2)*R(2,3)-CC(j,3)*R(3,3))/L(3);
end

% 10th-12th eigen-vectors
L(10) = -L(3);
L(11) = -L(2);
L(12) = -L(1);
for j = 1:3
  R(j,10) = R(j,3);
  R(j,11) = R(j,2);
  R(j,12) = R(j,1);
end
for j = 4:12
  R(j,10) = -R(j,3);
  R(j,11) = -R(j,2);
  R(j,12) = -R(j,1);
end

% eigenvectors 4-9 associated with the 0 eigenvalues
R(4,4)  = -tau(2)*alpha(3)+tau(3)*alpha(2);
R(7,4)  =  tau(1)*alpha(3)-tau(3)*alpha(1);
R(10,4) = -tau(1)*alpha(2)+tau(2)*alpha(1);

R(5,5)  =  R(4,4);
R(8,5)  =  R(7,4);
R(11,5) =  R(10,4);

R(6,6)  =  R(4,4);
R(9,6)  =  R(7,4);
R(12,6) =  R(10,4);

R(4,7)  = -gamma(2)*alpha(3)+gamma(3)*alpha(2);
R(7,7)  =  gamma(1)*alpha(3)-gamma(3)*alpha(1);
R(10,7) = -gamma(1)*alpha(2)+gamma(2)*alpha(1);

R(5,8)  =  R(4,7);
R(8,8)  =  R(7,7);
R(11,8) =  R(10,7);

R(6,9)  =  R(4,7);
R(9,9)  =  R(7,7);
R(12,9) =  R(10,7);

%%%%%
%% now left eigenvectors
%%%%%
D1 = V(1,1)*V(2,2)*V(3,3)...
    -V(1,1)*V(3,2)*V(2,3)...
    -V(2,1)*V(1,2)*V(3,3)...
    +V(2,1)*V(3,2)*V(1,3)...
    +V(3,1)*V(1,2)*V(2,3)...
    -V(3,1)*V(2,2)*V(1,3);

Ri(1,1) = (V(2,2)*V(3,3)-V(3,2)*V(2,3))/(2*D1);
Ri(2,1) = (V(3,1)*V(2,3)-V(2,1)*V(3,3))/(2*D1);
Ri(3,1) = (V(2,1)*V(3,2)-V(3,1)*V(2,2))/(2*D1);

Ri(1,2) = (V(3,2)*V(1,3)-V(1,2)*V(3,3))/(2*D1);
Ri(2,2) = (V(1,1)*V(3,3)-V(3,1)*V(1,3))/(2*D1);
Ri(3,2) = (V(3,1)*V(1,2)-V(1,1)*V(3,2))/(2*D1);

Ri(1,3) = (V(1,2)*V(2,3)-V(2,2)*V(1,3))/(2*D1);
Ri(2,3) = (V(2,1)*V(1,3)-V(1,1)*V(2,3))/(2*D1);
Ri(3,3) = (V(1,1)*V(2,2)-V(2,1)*V(1,2))/(2*D1);

Ri(10,1) = Ri(3,1);
Ri(10,2) = Ri(3,2);
Ri(10,3) = Ri(3,3);

Ri(11,1) = Ri(2,1);
Ri(11,2) = Ri(2,2);
Ri(11,3) = Ri(2,3);

Ri(12,1) = Ri(1,1);
Ri(12,2) = Ri(1,2);
Ri(12,3) = Ri(1,3);

for j = 1:3
  Ri(j,4) = -alpha(1)*Ri(j,1)/(rho*L(j));
  Ri(j,5) = -alpha(1)*Ri(j,2)/(rho*L(j));
  Ri(j,6) = -alpha(1)*Ri(j,3)/(rho*L(j));

  Ri(j,7) = -alpha(2)*Ri(j,1)/(rho*L(j));
  Ri(j,8) = -alpha(2)*Ri(j,2)/(rho*L(j));
  Ri(j,9) = -alpha(2)*Ri(j,3)/(rho*L(j));

  Ri(j,10) = -alpha(3)*Ri(j,1)/(rho*L(j));
  Ri(j,11) = -alpha(3)*Ri(j,2)/(rho*L(j));
  Ri(j,12) = -alpha(3)*Ri(j,3)/(rho*L(j));
end

for j = 10:12
  Ri(j,4) = -alpha(1)*Ri(j,1)/(rho*L(j));
  Ri(j,5) = -alpha(1)*Ri(j,2)/(rho*L(j));
  Ri(j,6) = -alpha(1)*Ri(j,3)/(rho*L(j));

  Ri(j,7) = -alpha(2)*Ri(j,1)/(rho*L(j));
  Ri(j,8) = -alpha(2)*Ri(j,2)/(rho*L(j));
  Ri(j,9) = -alpha(2)*Ri(j,3)/(rho*L(j));

  Ri(j,10) = -alpha(3)*Ri(j,1)/(rho*L(j));
  Ri(j,11) = -alpha(3)*Ri(j,2)/(rho*L(j));
  Ri(j,12) = -alpha(3)*Ri(j,3)/(rho*L(j));
end

rhs = zeros(3,9);

%% rows 4 and 7
for j = 1:9
  rhs(1,j) = -2*(Ri(1,j+3)*R(4,1) +Ri(2,j+3)*R(4,2) +Ri(3,j+3)*R(4,3));
  rhs(2,j) = -2*(Ri(1,j+3)*R(7,1) +Ri(2,j+3)*R(7,2) +Ri(3,j+3)*R(7,3));
  rhs(3,j) = -2*(Ri(1,j+3)*R(10,1)+Ri(2,j+3)*R(10,2)+Ri(3,j+3)*R(10,3));
end
rhs(1,1) = rhs(1,1)+1;
rhs(2,4) = rhs(2,4)+1;
rhs(3,7) = rhs(3,7)+1;

for j = 1:9
  Ri(4,j+3) = R(4,4)*rhs(1,j)+R(7,4)*rhs(2,j)+R(10,4)*rhs(3,j);
  Ri(7,j+3) = R(4,7)*rhs(1,j)+R(7,7)*rhs(2,j)+R(10,7)*rhs(3,j);
end

%% rows 5 and 8
for j = 1:9
  rhs(1,j) = -2*(Ri(1,j+3)*R(5,1) +Ri(2,j+3)*R(5,2) +Ri(3,j+3)*R(5,3));
  rhs(2,j) = -2*(Ri(1,j+3)*R(8,1) +Ri(2,j+3)*R(8,2) +Ri(3,j+3)*R(8,3));
  rhs(3,j) = -2*(Ri(1,j+3)*R(11,1)+Ri(2,j+3)*R(11,2)+Ri(3,j+3)*R(11,3));
end
rhs(1,2) = rhs(1,2)+1;
rhs(2,5) = rhs(2,5)+1;
rhs(3,8) = rhs(3,8)+1;

for j = 1:9
  Ri(5,j+3) = R(5,5)*rhs(1,j)+R(8,5)*rhs(2,j)+R(11,5)*rhs(3,j);
  Ri(8,j+3) = R(5,8)*rhs(1,j)+R(8,8)*rhs(2,j)+R(11,8)*rhs(3,j);
end


%% rows 6 and 9
for j = 1:9
  rhs(1,j) = -2*(Ri(1,j+3)*R(6,1) +Ri(2,j+3)*R(6,2) +Ri(3,j+3)*R(6,3));
  rhs(2,j) = -2*(Ri(1,j+3)*R(9,1) +Ri(2,j+3)*R(9,2) +Ri(3,j+3)*R(9,3));
  rhs(3,j) = -2*(Ri(1,j+3)*R(12,1)+Ri(2,j+3)*R(12,2)+Ri(3,j+3)*R(12,3));
end
rhs(1,3) = rhs(1,3)+1;
rhs(2,6) = rhs(2,6)+1;
rhs(3,9) = rhs(3,9)+1;

for j = 1:9
  Ri(6,j+3) = R(6,6)*rhs(1,j)+R(9,6)*rhs(2,j)+R(12,6)*rhs(3,j);
  Ri(9,j+3) = R(6,9)*rhs(1,j)+R(9,9)*rhs(2,j)+R(12,9)*rhs(3,j);
end


