function [L,U]=my_lu(A);
  [n,m]=size(A);
  if( n~=m )
    fprintf( 'error: require a square matrix\n' );
    return;
  end
  
  U=A;
  L=eye(n,n);
  for k=1:n-1
    if( U(k,k)== 0 )
      disp( 'Error: matrix is singular' );
      return
    end
    for i=k+1:n
      m=U(i,k)/U(k,k);
      for j=k+1:n
        U(i,j)=U(i,j)-m*U(k,j);
      end
      L(i,k)=m;
      U(i,k)=0.0;
    end
  end

return  
