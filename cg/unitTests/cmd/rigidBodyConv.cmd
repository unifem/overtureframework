#
# Create the rigid body convergence tables.
#
tFinal: 1
dt0: 0.05
tPlot: 0.1
mass: 1
numResolutions: 3
output file: rigidBodyConv.tex
# -------------------------------------------
leapFrogTrapezoidal
order of accuracy: 2
# --
trigonometric motion
convergence rate
# --
free rotation 1
convergence rate
# --
falling sphere
convergence rate
# ------------------------------------------------
implicitRungeKutta
# --
trigonometric motion
order of accuracy: 2
convergence rate
#
order of accuracy: 3
convergence rate
#
order of accuracy: 4
convergence rate
#
added mass 1
convergence rate
#
# --
free rotation 1
convergence rate
# --
falling sphere
convergence rate
#
exit