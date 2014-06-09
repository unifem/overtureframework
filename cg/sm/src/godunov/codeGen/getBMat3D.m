function BB = getBMat3D( alpha,CC )

  BB = zeros(3,3);

  BB(1,1) = alpha(1) * CC(4,1) + alpha(2) * CC(7,1) + alpha(3) * CC(10,1);
  BB(1,2) = alpha(1) * CC(4,2) + alpha(2) * CC(7,2) + alpha(3) * CC(10,2);
  BB(1,3) = alpha(1) * CC(4,3) + alpha(2) * CC(7,3) + alpha(3) * CC(10,3);
  BB(2,1) = alpha(1) * CC(5,1) + alpha(2) * CC(8,1) + alpha(3) * CC(11,1);
  BB(2,2) = alpha(1) * CC(5,2) + alpha(2) * CC(8,2) + alpha(3) * CC(11,2);
  BB(2,3) = alpha(1) * CC(5,3) + alpha(2) * CC(8,3) + alpha(3) * CC(11,3);
  BB(3,1) = alpha(1) * CC(6,1) + alpha(2) * CC(9,1) + alpha(3) * CC(12,1);
  BB(3,2) = alpha(1) * CC(6,2) + alpha(2) * CC(9,2) + alpha(3) * CC(12,2);
  BB(3,3) = alpha(1) * CC(6,3) + alpha(2) * CC(9,3) + alpha(3) * CC(12,3);


return;
