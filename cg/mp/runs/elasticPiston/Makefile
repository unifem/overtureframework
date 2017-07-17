#
#   Makefile for the elastic piston
#


ogen = /home/henshw/Overture.g/bin/ogen


grids: elasticPiston radial

elasticPiston:
	$(ogen) -noplot elasticPistonGrid -factor=1
	$(ogen) -noplot elasticPistonGrid -factor=2
	$(ogen) -noplot elasticPistonGrid -factor=4
	$(ogen) -noplot elasticPistonGrid -factor=8
	$(ogen) -noplot elasticPistonGrid -factor=16
	$(ogen) -noplot elasticPistonGrid -factor=1 -per=1
	$(ogen) -noplot elasticPistonGrid -factor=2 -per=1
	$(ogen) -noplot elasticPistonGrid -factor=4 -per=1
	$(ogen) -noplot elasticPistonGrid -factor=8 -per=1
	$(ogen) -noplot elasticPistonGrid -factor=16 -per=1

radial:
	$(ogen) -noplot radialElasticPistonGrid -interp=e -factor=1
	$(ogen) -noplot radialElasticPistonGrid -interp=e -factor=2
	$(ogen) -noplot radialElasticPistonGrid -interp=e -factor=4
	$(ogen) -noplot radialElasticPistonGrid -interp=e -factor=8
	$(ogen) -noplot radialElasticPistonGrid -interp=e -factor=16
