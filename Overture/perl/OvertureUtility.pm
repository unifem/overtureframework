#**************************************************************
#**************************************************************
#** Kyle Chand
#** 110309 - initial version
#**************************************************************
#**************************************************************
package OvertureUtility;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;
        @ISA         = qw(Exporter);
        @EXPORT      = qw( intmg setMultigridLevels getMultigridLevels );
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $ml;
our $ml2;

sub setMultigridLevels($) {
    $ml = shift;
    $ml2 = 2**$ml;
}

sub getMultigridLevels() { return $ml; }

sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }


END {}
1;
#print "pbc = $periodic_bc\n";
