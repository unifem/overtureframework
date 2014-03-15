# Make for the cmd directory to make it easier to re-make things in the parent directory
common = ../../common

cgins:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgins

defop.f : defop.bf; bpp -quiet -clean -I$(Overture)/include $<

defop.o : defop.f; pgf77 -o defop.o -c defop.f

clean:; 



