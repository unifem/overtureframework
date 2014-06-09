rho    = 1;
mu     = 1;
lambda = 1;

alpha = rand(2,1);
alpha = alpha./(norm(alpha));

f = eye(2,2)+1e-5*rand(2,2);

CC = getCMat2D( rho,mu,lambda,alpha,f );

BB = getBMat2D( alpha,CC );

[V,D] = eig( BB );
