#! /bin/sh
echo $1
cat $1 | sed 's%0\.7\.2a%\$APP_PPP_VERSION_NUMBER%g' > temp
mv temp $1
