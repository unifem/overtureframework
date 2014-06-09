%
% Fourth-order Runge-Kutta time-stepper
% 
function yn = rungeKutta4( yDot, y, t, dt );


k1 = dt*feval( yDot,t      ,y      );  % eval yDot(t,y)
k2 = dt*feval( yDot,t+.5*dt,y+.5*k1);
k3 = dt*feval( yDot,t+.5*dt,y+.5*k2);
k4 = dt*feval( yDot,t+   dt,y+   k3);

yn = y + ( k1 + 2.*(k2+k3) + k4)/6.;


