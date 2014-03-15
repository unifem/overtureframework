include ../../config/Makefile.$(VENDOR)

GenericCompositeGridOperators.C : GenericGridCollectionOperators.C; makeOperators.p


finishBoundaryConditions.C : finishBoundaryConditions.bC; bpp finishBoundaryConditions.bC

# test statement function macros
testSFMacros.f: testSFMacros.bf defineDiffMacrosOrder2f.h; bpp testSFMacros.bf

# test bpp style macros
testMacros.f: testMacros.bf defineDiffMacrosOrder2f.h defineParametricDerivMacros.h; bpp testMacros.bf


clean:
	rm -f *.o