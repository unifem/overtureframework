# Parallel regression tests:

$program = "../ogmgt"; 
$checkFile = "ogmg.check";
# 
# You should define the env variable MPIRUN_CHECK to be the command to run parallel jobs with: 
# (%NP will be replaced with the number of processors)
# setenv MPIRUN_CHECK "/home/henshaw/mpi/mpich-1.2.7p1.pgf77-64/bin/mpirun -all-local -np %NP"
# setenv MPIRUN_CHECK "srun -N1 -n%NP -ppdebug"
#
# $mpirunCommand = "/home/henshaw/mpi/mpich-1.2.7p1.pgf77-64/bin/mpirun -all-local -np"; 
$mpirunCommand = $ENV{MPIRUN_CHECK};

@cmdFiles=(
 "sq",
 "sqp",
 "cic",
 "cic",
 "cicp",
 "valve",
 "valvep",
 "box",
 "boxp",
 "sib",
 "sib",
 "sibp",
# "ellipsoid", # fix mapS for ellipsoid
 "cicNM.check ../cmd/tz.cmd -noplot -maxit=6 -g=cice2.order2.ml2 -sm=rbj -opav=0 -bc=nmnnn",
 "cic4.check ../cmd/tz.cmd -noplot -maxit=6 -g=cice4.order4.ml3 -sm=rbj -numParallelGhost=4",
 "sib4.check ../cmd/tz.cmd -noplot -maxit=6 -g=sibe2.order4.ml3 -sm=rbj -numParallelGhost=4",
 "rhombus2.check ../cmd/tz.cmd -noplot -maxit=6 -g=rhombus2.order2.ml2 -tz=poly -debug=3 -bc1=n -bc2=n -bsm=lz1",
 "rhomboid2.check ../cmd/tz.cmd -noplot -maxit=6 -g=rhomboid2.order2.ml2 -tz=poly -debug=3 -bc3=n -bc4=n -bsm=lz2",
 "rhombus4.check ../cmd/tz.cmd -noplot -g=rhombus2.order4.ml2 -bsm=lz1 -maxit=6 -bc1=n -bc2=m -eqn=heat -numParallelGhost=4",
 "rhomboid4.check ../cmd/tz.cmd -noplot -g=rhomboid2.order4.ml2 -bsm=lz2 -maxit=6 -bc3=n -bc4=m -eqn=heat -numParallelGhost=4"
); 


# specify the number of processors to use in each of the above cases 
@numProc=(
   "1",  # sq 
   "1",  # sqp 
   "1",  # cic
   "2",  # cic
   "1",  # cicp
   "1",  # valve
   "1",  # valvep
   "1",  # box
   "1",  # boxp
   "1",  # sib
   "2",  # sib
   "1",  # sibp
#  "1",  # ellipsoid
   "1",  # cicNM
   "1",  # cic4
   "1",  # sib4
   "1",  # rhombus2
   "1",  # rhomboid2
   "1",  # rhombus4
   "1",  # rhomboid4
   "1",  #
   "1",  #
   "1",  #
   "1"
  );
