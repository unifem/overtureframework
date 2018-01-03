%
% Return the added mass matrices as a function of t
%
function [A11 , A12 , A21, A22 ] = getAddedMassMatrices( t )

globalDeclarations;  % declare global variables here 

% Added mass matrices:
%   m v'     = -A11*v - A12*omega + f
%   A omega' = -A21*v - A22*omega + g 

if addedMass==1 
  z = 2. + .25*sin(2.*pi*t) ; % "impedance" for added mass 
  A11 = z*[ 1. 0. 0.; 0. 2. 0.; 0. 0. 3.];
  A12 = z*[ 0. .1 0.; .2 0. 0.; 0. 0. .3];
  A21 = A12'; 
  A22 = z*[ 3. 0. 0.; 0. 2. 0.; 0. 0. 1.];
else
  A11 = zeros(3,3); 
  A12 = zeros(3,3); 
  A21 = zeros(3,3); 
  A22 = zeros(3,3); 
end;