function xi = getEVSingle( beta )

beta
if( abs(beta(1,1)) > abs(beta(2,2)) )
  if( abs(beta(1,1)) > abs(beta(3,3)) )
    %% eliminate x1
    j=1;
    gamma = [-beta(2,1)*beta(1,2)/beta(1,1)+beta(2,2),-beta(2,1)*beta(1,3)/beta(1,1)+beta(2,3);-beta(3,1)*beta(1,2)/beta(1,1)+beta(3,2),-beta(3,1)*beta(1,3)/beta(1,1)+beta(3,3)];
    xi = zeros(3,1);
    xi(2:3) = solve2(gamma);
    xi(1) = -(beta(1,2)*xi(2)+beta(1,3)*xi(3))/beta(1,1);
  else
    %% eliminate x3
	 j=3;
    gamma = [-beta(1,3)*beta(3,1)/beta(3,3)+beta(1,1),-beta(1,3)*beta(3,2)/beta(3,3)+beta(1,2);-beta(2,3)*beta(3,1)/beta(3,3)+beta(2,1),-beta(2,3)*beta(3,2)/beta(3,3)+beta(2,2)];
    xi = zeros(3,1);
    xi(1:2) = solve2(gamma);
    xi(3) = -(beta(3,1)*xi(1)+beta(3,2)*xi(2))/beta(3,3);
  end
else
  if( abs(beta(2,2)) > abs(beta(3,3)) )
    %% eliminate x2
	 j=2;
	 gamma = [-beta(1,2)*beta(2,1)/beta(2,2)+beta(1,1),-beta(1,2)*beta(2,3)/beta(2,2)+beta(1,3);-beta(3,2)*beta(2,1)/beta(2,2)+beta(3,1),-beta(3,2)*beta(2,3)/beta(2,2)+beta(3,3)];
    xi = zeros(3,1);
    tmp = solve2(gamma);
    xi(1) = tmp(1);
    xi(3) = tmp(2);
    xi(2) = -(beta(2,1)*xi(1)+beta(2,3)*xi(3))/beta(2,2);
  else
    %% eliminate x3
	 j=3;
    gamma = [-beta(1,3)*beta(3,1)/beta(3,3)+beta(1,1),-beta(1,3)*beta(3,2)/beta(3,3)+beta(1,2);-beta(2,3)*beta(3,1)/beta(3,3)+beta(2,1),-beta(2,3)*beta(3,2)/beta(3,3)+beta(2,2)];
    xi = zeros(3,1);
    xi(1:2) = solve2(gamma);
    xi(3) = -(beta(3,1)*xi(1)+beta(3,2)*xi(2))/beta(3,3);
  end
end 

j

%xi = xi/norm(xi);

return

