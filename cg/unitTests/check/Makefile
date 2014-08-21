# Type "make check" to run the regression tests
# NOTE: The file tests contains the list of test cases

common = ../../common

check: 
	ln -sf $(common)/src/check.p .
	./check.p -solver=tbm

check-quiet:  
	ln -sf $(common)/src/check.p .
	./check.p -solver=tbm -verbose=0

tbm:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) tbm