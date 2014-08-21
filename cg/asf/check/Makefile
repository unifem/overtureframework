# Type "make check" to run the regression tests
# NOTE: The file tests contains the list of test cases

common = ../../common
check:
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgasf

check-quiet:
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgasf -verbose=0

cgasf:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgasf
