#**************************************************************
#** perl module for CgINS grid generation and solver runs
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 100121 - initial version
#**************************************************************
#**************************************************************
package CgINS;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.h
use SquareMapping;
use StretchTransform;
use SmoothedPolygonMapping;
use ReparameterizationTransform;

BEGIN {
        use Exporter   ();
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;
        @ISA         = qw(Exporter);
#        @EXPORT      = qw( &processBuildingFile);#
        %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT = qw(  );
        @EXPORT_OK   = qw( );
}

END { };

# the following line must be here or else the import will fail
1; 
