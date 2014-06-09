function [x,y] = makeThing( x0,y0,r_avg,dr,alpha,k,N );

  % makeThing( 0,0,1,1/2,1,2,1001 );

  theta = linspace(0,2*pi,N);

  x = (r_avg+dr*sin(k*theta)).*cos(theta);
  y = (r_avg+dr*sin(k*theta)).*sin(theta);

  r2 = x.^2+y.^2;
  rotlim1 = r_avg-dr;

  phi = alpha*(r2-rotlim1^2);

  for i = 1:N
    R = [cos(phi(i)),-sin(phi(i));sin(phi(i)),cos(phi(i))];
    tmp = R*[x(i);y(i)];
    x(i) = tmp(1);
    y(i) = tmp(2);
  end

  x = x+x0;
  y = y+y0;

  plot( x,y,'kx-' );

return;
