for jpgname in `find *.jpg`
do
	logname=${jpgname:0:9}.log
	./detect.sh $jpgname > $logname
done
