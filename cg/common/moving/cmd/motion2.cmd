# Usage: 
#    motion motion1.cmd
#
# In this example we compose two motions:
#    (1) rotate about the z-axis
#    (2) translate with a siusoidal motion
#
final time: 1.
time step: .05
# first rotate about z-axis
edit motion for body: 0
  point on line: 0 0 0
  tangent to line: 0 0 1
  exit
# perform a sinusoidal translation along a line with tangent (1,1,0)
compose motion for body: 0 
  translate along a line
  point on line: 0 0 0
  tangent to line: 1 1 0
  edit time function
    sinusoidal function
    sinusoid parameters: 1,1,0 (b0,f0,t0)
    exit
  exit

