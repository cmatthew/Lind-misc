#! /bin/bash
set -o errexit
mkdir -p tmp
cd tmp
body="<html>
<body>

<h1>My First Heading</h1>

<p>My first paragraph.</p>

</body>
</html>
"
echo $body > index2.html

python -m SimpleHTTPServer &
server=$!
cd ..
loc=`pwd`
cd ~/lind/misc/wget/wget-1.13-lind/src/
make
cd $loc

wget_path=~/lind/misc/wget/wget-1.13-lind/src/

command="lind ${wget_path}wget"
pwd 
$command -t 1 -O lind_file.html http://127.0.0.1:8000/index2.html 
command="wget"
$command -t 1 -O real_file.html http://127.0.0.1:8000/index2.html 


if diff real_file.html linddata.2; then
  echo "Wget test Passed"
  
else
  echo "WGet test failed"
fi
echo "<Lind HTML:>"
cat linddata.2
echo "</Lind HTML:>"

kill -9 $server
killall python2.6
killall sel_ldr
ls

rm -rf etc tmp lind.log lind.metadata linddata.* *.html

