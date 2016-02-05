# Type "make check" to run the regression tests
# NOTE: The file tests contains the list of test cases

common = ../../common

check: 
	ln -sf $(common)/src/check.p .
	./check.p -solver=tbm
	./check.p -solver=trb -testsFile=tests.trb


check-quiet:  
	ln -sf $(common)/src/check.p .
	./check.p -solver=tbm -verbose=0
	./check.p -solver=trb -testsFile=tests.trb -verbose=0

tbm:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) tbm