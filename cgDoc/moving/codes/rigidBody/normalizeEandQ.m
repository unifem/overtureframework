%
% Normalize the rotation matrix and the quaternion
%
function yv = normalizeEandQ( yv0 )

  globalDeclarations;  % declare global variables here

  yv = yv0; 

  qv = yv(mq:mq+3);
  qNorm = sqrt( qv'*qv );
  qv = qv./qNorm;

  % normalize and orthogonalize E
  m=me; 
  for j=1:3
    ea(1:3,j) = yv(m:m+2); m=m+3;
 
    % Normalize
    eNorm = sqrt( ea(1,j)^2 + ea(2,j)^2 + ea(3,j)^2 );
    ea(1:3,j)= ea(1:3,j)./eNorm;
   
    % make this row orthogonal to the previous rows
    for k=1:j-1
      if k<j 
        eDot = ea(1,j)*ea(1,k)+ea(2,j)*ea(2,k)+ea(3,j)*ea(3,k);
        ea(1:3,j) = ea(1:3,j) - eDot*ea(1:3,k);
      end;
    end;
   
    % Normalize
    eNorm = sqrt( ea(1,j)^2 + ea(2,j)^2 + ea(3,j)^2 );
    ea(1:3,j)= ea(1:3,j)./eNorm; 
  end;

  yv(mq:mq+3)=qv;
  m=me;
  for j=1:3
    yv(m:m+2)=ea(1:3,j); m=m+3;
  end;
