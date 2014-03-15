common = ../../common

# The convergence rate script is called conv.p
all:  
	ln -sf $(common)/src/conv.p .
	ln -sf ../cmd/cnsDomain.h .
	ln -sf ../cmd/insDomain.h .
	ln -sf ../cmd/adDomain.h .


cgmp:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgmp

