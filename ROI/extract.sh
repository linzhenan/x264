filename=$1
samplerate=$2
ffmpeg -s 352x288 -i $filename -r $samplerate -q:v 2 -f image2 image-%3d.jpg
