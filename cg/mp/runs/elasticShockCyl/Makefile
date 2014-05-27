all = linkFiles diskDeform

linkFiles:
	ln -s $(Overture)/sampleGrids/diskDeform.cmd .


diskDeform:
	$(ogen) -noplot diskDeform -factor=1 -interp=e -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -name="diskDeformBig1e.hdf"
	$(ogen) -noplot diskDeform -factor=2 -interp=e -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -name="diskDeformBig2e.hdf"
	$(ogen) -noplot diskDeform -factor=4 -interp=e -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -name="diskDeformBig4e.hdf"
	$(ogen) -noplot diskDeform -factor=8 -interp=e -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -name="diskDeformBig8e.hdf"
	$(ogen) -noplot diskDeform -factor=16 -interp=e -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -name="diskDeformBig16e.hdf"