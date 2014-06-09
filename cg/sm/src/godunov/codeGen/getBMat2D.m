function BB = getBMat2D( alpha,CC )

  BB = zeros(2,2);

BB(1,1) = alpha(1) * CC(3,1) + alpha(2) * CC(5,1);
BB(1,2) = alpha(1) * CC(3,2) + alpha(2) * CC(5,2);
BB(2,1) = alpha(1) * CC(4,1) + alpha(2) * CC(6,1);
BB(2,2) = alpha(1) * CC(4,2) + alpha(2) * CC(6,2);

return;
