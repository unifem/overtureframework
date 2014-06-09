function x = solve2(a)
  x = zeros(2,1);
  if( abs(a(1,1)) > abs(a(2,2)) )
    x(2) = sqrt(a(1,1)^2/(a(1,2)^2+a(1,1)^2));
    %x(2) = 1;
    x(1) = -a(1,2)*x(2)/a(1,1);
  else
    x(1) = sqrt(a(2,2)^2/(a(2,2)^2+a(2,1)^2));
    %x(1) = 1;
    x(2) = -a(2,1)*x(1)/a(2,2);
  end

