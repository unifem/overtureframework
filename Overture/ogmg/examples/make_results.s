#! /bin/csh -f
#
# Make the .results files for the various examples
#

echo ' Make square.results...'

echo ' +++ sq10 +++ ' > square.results
square.s >> square.results << EOF
sq10
EOF

echo ' +++ sq10F +++ ' >> square.results
square.s >> square.results << EOF
sq10F
EOF

echo ' +++ sq10n +++ ' >> square.results
square.s >> square.results << EOF
sq10n
EOF

echo ' +++ sq10nF +++ ' >> square.results
square.s >> square.results << EOF
sq10nF
EOF

echo ' +++ sq10m +++ ' >> square.results
square.s >> square.results << EOF
sq10m
EOF

echo ' +++ sq10mF +++ ' >> square.results
square.s >> square.results << EOF
sq10mF
EOF

echo ' Make cis2.results...'
echo ' +++ cis +++ ' > cis2.results
cis2.s >> cis2.results << EOF
cis
EOF

echo ' +++ cisn +++ ' >> cis2.results
cis2.s >> cis2.results << EOF
cisn
EOF

echo ' +++ cism +++ ' >> cis2.results
cis2.s >> cis2.results << EOF
cism
EOF

echo ' Make cis4.results...'
echo ' +++ cis +++ ' > cis4.results
cis4.s >> cis4.results << EOF
cis
EOF

echo ' +++ cisn +++ ' >> cis4.results
cis4.s >> cis4.results << EOF
cisn
EOF

echo ' +++ cisn +++ ' >> cis4.results
cis4.s >> cis4.results << EOF
cism
EOF

echo ' Make cube.results...'
echo ' +++ cube10 +++ ' > cube.results
cube.s >> cube.results << EOF
cube10
EOF

echo ' +++ cube10F +++ ' >> cube.results
cube.s >> cube.results << EOF
cube10F
EOF

echo ' +++ cube10n +++ ' >> cube.results
cube.s >> cube.results << EOF
cube10n
EOF

echo ' +++ cube10nF +++ ' >> cube.results
cube.s >> cube.results << EOF
cube10nF
EOF

echo ' +++ cube10m +++ ' >> cube.results
cube.s >> cube.results << EOF
cube10m
EOF

echo ' +++ cube10F +++ ' >> cube.results
cube.s >> cube.results << EOF
cube10mF
EOF

echo ' Make sib.results...'
echo ' +++ sib +++ ' > sib.results
sib.s >> sib.results << EOF
sib
EOF

echo ' +++ sibF +++ ' >> sib.results
sib.s >> sib.results << EOF
sibF
EOF

echo ' +++ sibn +++ ' >> sib.results
sib.s >> sib.results << EOF
sibn
EOF

#echo ' +++ sibnF +++ ' >> sib.results
#sib.s >> sib.results << EOF
#sibnF
#EOF

echo ' +++ sibm +++ ' >> sib.results
sib.s >> sib.results << EOF
sibm
EOF

#echo ' +++ sibmF +++ ' >> sib.results
#sib.s >> sib.results << EOF
#sibmF
#EOF

