common = ../../common

# The convergence rate script is called conv.p
all:  
	ln -sf $(common)/src/conv.p .

tbm:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) tbm


# tbm convergence runs
tbmRuns:
	conv.p tbm.conv -model=linearBeam -tz=trig -bcn=c -tf=1. -cg=line -ng=4
	conv.p tbm.conv -model=linearBeam -tz=trig -bcn=p -tf=1. -cg=line -ng=4
	conv.p tbm.conv -model=linearBeam -tz=trig -bcn=f -tf=1. -cg=line -ng=4
	conv.p tbm.conv -model=linearBeam -tz=trig -bcn=p -tf=1. -Em=0. -T=1. -cg=line -cfl=.25 -suffix=string -ng=4
	conv.p tbm.conv -model=linearBeam -tz=sw -bcn=p -tf=1. -Em=1. -T=0. -cg=line -ng=5
	conv.p tbm.conv -model=linearBeam -tz=trig -bcn=per -tf=1. -rhos=10. -cg=line -cfl=.25 -ng=4