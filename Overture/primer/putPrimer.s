#! /bin/csh -f
#

echo "Copy primer files into the Overture.g/primer..."

set primer  = "$HOME/Overture.g/primer"

cp {example}{1,2,3,4,5,6,7,8,9}.C                $primer
cp {mappedGridExample}{1,2,3,3CC,4,5,6}.C        $primer
cp {mgExample}{1,2}.C                            $primer
cp ChannelMapping.{h,C}                          $primer
cp {move1,getDt,wave}.C                          $primer
cp plot.s                                        $primer
cp {lins}.C                                      $primer
cp cshrc.{sun,sgi64}                             $primer
cp Makefile.{sun,sgi64,linux}                    $primer




