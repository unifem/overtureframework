function CC = getCMat2D( rho,mu,lambda,alpha,f )

  CC = zeros(6,6);

t1 = 0.1e1 / rho;
t2 = alpha(1) * t1;
t3 = alpha(2) * t1;
t4 = f(1,1) ^ 2;
t5 = lambda * t4;
t7 = mu * t4;
t9 = f(2,1) ^ 2;
t10 = lambda * t9;
t11 = t10 / 0.2e1;
t12 = f(1,2) ^ 2;
t13 = lambda * t12;
t14 = t13 / 0.2e1;
t15 = f(2,2) ^ 2;
t16 = lambda * t15;
t17 = t16 / 0.2e1;
t18 = mu * t9;
t19 = mu * t12;
t22 = lambda * f(1,2);
t24 = mu * f(1,1);
t25 = t24 * f(1,2);
t27 = mu * f(2,1);
t28 = t27 * f(2,2);
t29 = t22 * f(1,1) + 0.2e1 * t25 + t28;
t34 = t24 * f(2,1);
t37 = mu * f(2,2) * f(1,2);
t39 = alpha(1) * (f(1,1) * lambda * f(2,1) + 0.2e1 * t34 + t37);
t40 = lambda * f(2,2);
t43 = t40 * f(1,1) + t27 * f(1,2);
t48 = t22 * f(2,1) + t24 * f(2,2);
t53 = t5 / 0.2e1;
t54 = mu * t15;
t59 = t40 * f(2,1) + 0.2e1 * t28 + t25;
t72 = alpha(2) * (t34 + t22 * f(2,2) + 0.2e1 * t37);
CC(1,1) = 0.0e0;
CC(1,2) = 0.0e0;
CC(1,3) = -t2;
CC(1,4) = 0.0e0;
CC(1,5) = -t3;
CC(1,6) = 0.0e0;
CC(2,1) = 0.0e0;
CC(2,2) = 0.0e0;
CC(2,3) = 0.0e0;
CC(2,4) = -t2;
CC(2,5) = 0.0e0;
CC(2,6) = -t3;
CC(3,1) = alpha(1) * (0.3e1 / 0.2e1 * t5 + 0.3e1 * t7 + t11 - lambda + t14 + t17 + t18 - mu + t19) + alpha(2) * t29;
CC(3,2) = t39 + alpha(2) * t43;
CC(3,3) = 0.0e0;
CC(3,4) = 0.0e0;
CC(3,5) = 0.0e0;
CC(3,6) = 0.0e0;
CC(4,1) = t39 + alpha(2) * t48;
CC(4,2) = alpha(1) * (0.3e1 / 0.2e1 * t10 + 0.3e1 * t18 + t53 - lambda + t14 + t17 + t7 - mu + t54) + alpha(2) * t59;
CC(4,3) = 0.0e0;
CC(4,4) = 0.0e0;
CC(4,5) = 0.0e0;
CC(4,6) = 0.0e0;
CC(5,1) = alpha(1) * t29 + alpha(2) * (t7 + 0.3e1 / 0.2e1 * t13 + 0.3e1 * t19 + t53 + t11 - lambda + t17 + t54 - mu);
CC(5,2) = alpha(1) * t48 + t72;
CC(5,3) = 0.0e0;
CC(5,4) = 0.0e0;
CC(5,5) = 0.0e0;
CC(5,6) = 0.0e0;
CC(6,1) = alpha(1) * t43 + t72;
CC(6,2) = alpha(1) * t59 + alpha(2) * (t18 + 0.3e1 / 0.2e1 * t16 + 0.3e1 * t54 + t53 + t11 - lambda + t14 + t19 - mu);
CC(6,3) = 0.0e0;
CC(6,4) = 0.0e0;
CC(6,5) = 0.0e0;
CC(6,6) = 0.0e0;


return


