#echo off

#ffmpeg_path="ffmpeg"
ffmpeg_path="/system/ffmpeg"

#filelist=".filelist"
filelist="/sdcard/testset/filelist_ndk"

#find . -name *.[fF][lL][vV] > $filelist
#find . -name *.[mM][oO][vV] >> $filelist

#generate YUV
framecnt="900"
videores="360x640"
videofps="15"

#encode YUV
bitrate="500k"

#ffmpeg_cfg="-threads 1"
ffmpeg_cfg="-re" #input video at output framerate
ffmpeg_opt="-preset medium -b:v "${bitrate}

#x264opts_opt="keyint=60:deblock=2,2:threads=1:lookahead-threads=1:bframes=3:b-adapt=0:scenecut=0:weightp=1:rc-lookahead=20"
x264opts_opt="keyint=60:deblock=2,2:bframes=3:b-adapt=0:scenecut=0:weightp=1:rc-lookahead=20"
x264opts_lv=(
"trellis=0:ref=1:me=dia:subme=1:mixed-refs=0"
"trellis=0:ref=1:me=dia:subme=2:mixed-refs=0"
"trellis=0:ref=1:me=hex:subme=2:mixed-refs=0"
"trellis=1:ref=1:me=hex:subme=3:mixed-refs=0"
"trellis=1:ref=2:me=hex:subme=4:mixed-refs=0" #default
"trellis=1:ref=2:me=hex:subme=6:mixed-refs=1"
"trellis=1:ref=3:me=hex:subme=7:mixed-refs=1"
)

for filename in $(cat $filelist)
do
	${ffmpeg_path} -i ${filename} -vf fps=${videofps} -s ${videores} -vframes ${framecnt} -y ${filename}.yuv

	#for ((i=0;i<=5;i++))
	for i in 0 1 2 3 4 5 6
	do
		#lv=$[$i+1]
		lv=${i}
		
		date	
		
		#${ffmpeg_path} ${ffmpeg_cfg} -s ${videores} -pix_fmt yuv420p -r ${videofps} -i ${filename}.yuv -vcodec libx264 ${ffmpeg_opt} -r ${videofps} -x264opts ${x264opts_opt}:${x264opts_lv[i]} -y ${filename}.264
		${ffmpeg_path} ${ffmpeg_cfg} -s ${videores} -pix_fmt yuv420p -r ${videofps} -re -i ${filename}.yuv -vcodec libx264 ${ffmpeg_opt} -r ${videofps} -x264opts ${x264opts_opt}:${x264opts_lv[i]} -y ${filename}.264
		date

		${ffmpeg_path} -i ${filename}.264 -y ${filename}_dec.yuv
		${ffmpeg_path} -s ${videores} -i ${filename}_dec.yuv -s ${videores} -i ${filename}.yuv -lavfi "psnr" -f null -
	done

	rm ${filename}.yuv
	rm ${filename}.264
	rm ${filename}_dec.yuv
done

#rm $filelist
