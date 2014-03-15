#**************************************************************
#**************************************************************
#** Kyle Chand
#** 070712 - initial version
#**************************************************************
#**************************************************************
package StandardBoundaryConditions;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;
        @ISA         = qw(Exporter);
        @EXPORT      = qw( $interpolation_bc $periodic_bc $no_bc );
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $interpolation_bc = 0;
our $no_bc = 0;
our $periodic_bc = -1;

END {}
1;
#print "pbc = $periodic_bc\n";
