eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl program to create the polyFunction.f macros
# 

$fileName="polyFunction.h";
printf(" create file = $fileName \n");
  
open(FILE,">$fileName") || die "cannot open file $fileName!" ;
  
printf(FILE "! ====== This file created by  polyFunction.p =====\n");


for( $nd=1; $nd<=3; $nd++ )
{
printf(FILE "! ****** %i dimensions *********\n",$nd);

for( $degree=0; $degree<=6; $degree++ )
{
printf(FILE "! ****** degree %i *********\n",$degree);

printf(FILE "#beginMacro poly$nd\D$degree(xxx,yyy,zzz,rrr)\n");

$laplace="";
@lapX = ();
@assignX = ();

# only compute first and second derivatives here
# except we also want uxxx and uxxxx
$maxXDerivative=4; if( $maxXDerivative>$degree ){ $maxXDerivative=$degree; }
$maxDerivative=2; if( $maxDerivative>$degree ){ $maxDerivative=$degree; }

for( $dx=0; $dx<=$maxXDerivative; $dx++ ) # x derivatives
{
$ndy=$maxDerivative; if( $nd<2 || $dx>$maxDerivative ){ $ndy=0; }
for( $dy=0; $dy<=$ndy; $dy++ ) # y derivatives
{
$ndz=$maxDerivative; if( $nd<3 || $dx>$maxDerivative ){ $ndz=0; }
for( $dz=0; $dz<=$ndz; $dz++ ) # y derivatives
{

# split lines that are longer than this many chars so we don't have toom many continuation lines
# The Intel f77 only allows 99 continuation lines
# We get about 60 chars per line.
$maxNumberOfContinuationLines=80;
$charsPerLine=60;
$splitLength=$maxNumberOfContinuationLines*$charsPerLine;
$maxLength=$splitLength;

$poly="";  # holds polynomial 

$saveForTimeDerivative=0;
if( $dx==0 && $dy==0 && $dz==0 )
{
 $saveForTimeDerivative=1;
} 
$saveLaplace=0;
if( ($dx==2 && $dy+$dz==0) || ($dy==2 && $dx+$dz==0) || ($dz==2 && $dx+$dy==0) )
{
  $saveLaplace=1;
}
if( $dx==0 && $dy==0 && $dz==0 )
{
  printf(FILE "if( dx.eq.$dx.and.dy.eq.$dy.and.dz.eq.$dz )then\n");
  # printf(FILE "if( dx.eq.$dx.and.dy.eq.$dy.and.dz.eq.$dz.and.dt.eq.0 )then\n");
}
else
{
 printf(FILE "else if( dx.eq.$dx.and.dy.eq.$dy.and.dz.eq.$dz )then\n");
}
# output time polynomial
#print FILE "time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))\n";
printf(FILE "beginLoops(\$defineTime())\n");

$z="";
for( $n1=0; $n1<=$degree; $n1++ ) # loop over powers of x
{ 
  if( $n1>=$dx )
  {
    $xpow=$n1-$dx; $xpowm=$xpow-1;
    if( $xpow>0 ){$x = "x$xpow"; }else{$x=""; }
    $xfact=1;
    for( $mx=$n1; $mx>$xpow; $mx-- ){ $xfact=$xfact*$mx; }
    $n2b=$degree; if( $nd<2 ){ $n2b=0; }
    for( $n2=0; $n2<=$n2b; $n2++ ) # loop over powers of y
    {
      if( $n2>=$dy )
      {
        $ypow=$n2-$dy; $ypowm=$ypow-1;
        $yfact=1;
        for( $my=$n2; $my>$ypow; $my-- ){ $yfact=$yfact*$my; }
        if( $ypow>0 ){$y = "y$ypow";}else{$y="";}

        $n3b=$degree; if( $nd<3 ){ $n3b=0; }
        for( $n3=0; $n3<=$n3b; $n3++ ) # loop over powers of z
        {
          if( $n3>=$dz )
          {
            $zpow=$n3-$dz; $zpowm=$zpow-1;
            $zfact=1;
            for( $mz=$n3; $mz>$zpow; $mz-- ){ $zfact=$zfact*$mz; }

            if( $n1+$n2+$n3 > 0 )
            {
 
              $prod=$xfact*$yfact*$zfact;
              $fact="*$prod";
              if( $fact eq "*1" ){ $fact=""; }else{ $fact="$fact\.";}
              if( $zpow>0 ){$z = "z$zpow";}else{$z="";}

              $xm="";
              # if(    $xpowm==0 && $ypowm<0  && $zpowm<0  ){$xm="$x=xa(i1,i2,i3)";}
              # elsif( $xpowm<0  && $ypowm==0 && $zpowm<0  ){$xm="$y=ya(i1,i2,i3)";}
              # elsif( $xpowm<0  && $ypowm<0  && $zpowm==0 ){$xm="$z=za(i1,i2,i3)";}

              if(    $xpowm==0 && $ypowm<0  && $zpowm<0  ){$xm="xxx";}
              elsif( $xpowm<0  && $ypowm==0 && $zpowm<0  ){$xm="yyy";}
              elsif( $xpowm<0  && $ypowm<0  && $zpowm==0 ){$xm="zzz";}
              elsif( $xpowm>0  && $ypowm<0  && $zpowm<0  ){$xm="$x=x$xpowm*x1";}
              elsif( $xpowm<0  && $ypowm>0  && $zpowm<0  ){$xm="$y=y$ypowm*y1";}
              elsif( $xpowm<0  && $ypowm<0  && $zpowm>0  ){$xm="$z=z$zpowm*z1";}

              elsif( $xpowm==0 && $ypowm==0 && $zpowm<0  ){$xm="$x$y=x1*y1";}
              elsif( $xpowm>0  && $ypowm>=0 && $zpowm<0  ){$xm="$x$y=x$xpowm$y*x1";}
              elsif( $xpowm>=0 && $ypowm>0  && $zpowm<0  ){$xm="$x$y=$x" . "y$ypowm*y1";}

              elsif( $xpowm==0 && $zpowm==0 && $ypowm<0  ){$xm="$x$z=x1*z1";}
              elsif( $xpowm>0  && $zpowm>=0 && $ypowm<0  ){$xm="$x$z=x$xpowm$z*x1";}
              elsif( $xpowm>=0 && $zpowm>0  && $ypowm<0  ){$xm="$x$z=$x" . "z$zpowm*z1";}

              elsif( $ypowm==0 && $zpowm==0 && $xpowm<0  ){$xm="$y$z=y1*z1";}
              elsif( $ypowm>0  && $zpowm>=0 && $xpowm<0  ){$xm="$y$z=y$ypowm$z*y1";}
              elsif( $ypowm>=0 && $zpowm>0  && $xpowm<0  ){$xm="$y$z=$y" . "z$zpowm*z1";}

              elsif( $xpowm==0 && $zpowm==0 && $ypowm<0  ){$xm="$x$z=x1*z1";}
              elsif( $xpowm>0  && $zpowm>=0 && $ypowm<0  ){$xm="$x$z=x$xpowm$z*x1";}
              elsif( $xpowm>=0 && $zpowm>0  && $ypowm<0  ){$xm="$x$z=$x" . "z$zpowm*z1";}

              elsif( $xpowm==0 && $ypowm==0 && $zpowm==0 ){$xm="$x$y$z=x1*y1*z1";}
              elsif( $xpowm>0  && $ypowm>=0 && $zpowm>=0 ){$xm="$x$y$z=x$xpowm$y$z*x1";}
              elsif( $xpowm>=0 && $ypowm>0  && $zpowm>=0 ){$xm="$x$y$z=$x" . "y$ypowm$z*y1";}
              elsif( $xpowm>=0 && $ypowm>=0 && $zpowm>0  ){$xm="$x$y$z=$x$y" . "z$zpowm*z1";}

              elsif( $xpowm<0  && $ypowm<0 && $zpowm<0  ){ $xm=""; }
              else{ print "ERROR: unknown case: $xpowm $ypowm $zpowm \n"; exit(1) }

              if( $xm ne "" )
              { 
                print FILE "$xm \n";
                # keep track of unique x1y2= statements for the laplace operator
                if( $saveLaplace==1 ){ @match=grep{ $_ eq $xm} @lapX; if( $#match ==-1 ){push(@lapX,$xm);} }
                if( $saveForTimeDerivative==1 ){ @match=grep{ $_ eq $xm} @assignX; if( $#match ==-1 ){push(@assignX,$xm);} }
              }
 
              # $num = length($poly);
              # if( $num > 1000 ){ printf(" len = $num maxLength=$maxLength\n"); }
              if( length($poly)> $maxLength )
              {
                printf(" split line... nd=$nd, dx=$dx dy=$dy dz=$dz (splitLength=$splitLength)\n");
		$poly = $poly . ")\n rrr=(rrr";
                $maxLength=$maxLength+$splitLength;
              }


              if( $x ne "" || $y ne "" || $z ne "" )
              {
                # printf("+c(%i,%i,%i,n)*%s%s%s%s",$n1,$n2,$n3,$x,$y,$z,$fact);
                $poly = $poly . "+c($n1,$n2,$n3,n)*$x$y$z$fact";
              }
              else
              {
                # printf("+c(%i,%i,%i,n)%s%s%s%s",$n1,$n2,$n3,$x,$y,$z,$fact);
                $poly = $poly . "c($n1,$n2,$n3,n)$fact";
              }
              

	    }
	  }
	}
      }
    }
#  printf("\n");
  }
} # n1

if( ($dx==2 && $dy+$dz==0) || ($dy==2 && $dx+$dz==0) || ($dz==2 && $dx+$dy==0) )
{
  if( length($laplace)>10 && (length($laplace)+length($poly)> $splitLength) )
  {
    # The laplace line may get too long since we append other strings -- this will split too
    # often but good enough for now.
    printf(" split laplace line... nd=$nd, dx=$dx dy=$dy dz=$dz (splitLength=$splitLength)\n");
    $laplace = "$laplace )\n rrr=(rrr+$poly";   # The laplace operator combines the xx + yy + zz operators
  }
  else
  {
    $laplace = "$laplace+$poly";   # The laplace operator combines the xx + yy + zz operators
  }
} 

if( $dx==0 && $dy==0 && $dz==0 )
{
  $poly = "rrr=(c(0,0,0,n)" . $poly;
}
else
{
  $poly = "rrr=(" .$poly;
}
print FILE "$poly\)*time" . "\n";
printf(FILE "endLoops()\n");

#- # output time derivative
#- if( $dx==0 && $dy==0 && $dz==0 )
#- {
#-  printf(FILE "else if( dx.eq.$dx.and.dy.eq.$dy.and.dz.eq.$dz.and.dt.ge.1 )then\n");
#- # print FILE "time=(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)))))\n";  # this is the first time derivative
#-  printf(FILE "beginLoops(\$defineTimeDerivative())\n");
#-  for( $ii=0; $ii<=$#assignX; $ii++ )
#-  {
#-    print FILE "$assignX[$ii]\n";
#-  #  print "$assignX[$ii]\n";
#-  }
#-  print FILE "$poly\)*time" . "\n";
#-  printf(FILE "endLoops()\n");
#- }


} # dz
}
} # dx

  

if( $laplace ne "" )
{
 printf(FILE "else if( laplace.eq.1 )then\n");
# print FILE "time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))\n";
 printf(FILE "beginLoops(\$defineTime())\n");
 for( $ii=0; $ii<=$#lapX; $ii++ )
 {
   print FILE "$lapX[$ii]\n";
 }

 printf(FILE "rrr=($laplace\)*time\n");
 printf(FILE "endLoops()\n");
}
printf(FILE "else\n");
printf(FILE "beginLoops(time=0.)\n");
printf(FILE "rrr=0.\n");
printf(FILE "endLoops()\n");


printf(FILE "end if\n");
printf(FILE "#endMacro\n");

} # degree
} # dimension


close(FILE);
# 
#  c(0,0,0,n) 
# +c(1,0,0,n)*x  +c(0,1,0,n)*y 
# +c(2,0,0,n)*x2 +c(1,1,0,n)*xy +c(0,2,0,n)*y2
# +c(3,0,0,n)*x3 +c(2,1,0,n)*x2y+c(1,2,0,n)*xy2 +c(0,3,0,n)*y3
# +c(4,0,0,n)*x4 +c(3,1,0,n)*x3y+c(2,2,0,n)*x2y2+c(1,3,0,n)*xy3 +c(0,4,0,n)*y4
# +c(5,0,0,n)*x5 +c(4,1,0,n)*x4y+c(3,2,0,n)*x3y2+c(2,3,0,n)*x2y3+c(1,4,0,n)*xy4+c(0,5,0,n)*y5
# +c(6,0,0,n)*x6 +c(5,1,0,n)*x5y+c(4,2,0,n)*x4y2+c(3,3,0,n)*x3y3+c(2,4,0,n)*x2y4+c(1,5,0,n)*xy5+c(0,6,0,n)*y6


