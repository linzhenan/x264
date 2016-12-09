echo off
setlocal enabledelayedexpansion

set ffmpeg_path=ffmpeg
set filelist=filelist

::generate YUV
set framecnt=900
set videores=360x640
set videofps=15

::encode YUV
set bitrate=500k

set ffmpeg_cfg=-re -psnr ::input video at output framerate & calc PSNR
set ffmpeg_opt=-preset medium -b:v %bitrate%

set x264opts_opt=keyint=60:deblock=2,2:bframes=3:b-adapt=0:scenecut=0:weightp=1:rc-lookahead=20
set x264optsLv_Length=7
set x264optsLv0=trellis=0:ref=1:me=dia:subme=1:mixed-refs=0
set x264optsLv1=trellis=0:ref=1:me=dia:subme=2:mixed-refs=0
set x264optsLv2=trellis=0:ref=1:me=hex:subme=2:mixed-refs=0
set x264optsLv3=trellis=1:ref=1:me=hex:subme=3:mixed-refs=0
set x264optsLv4=trellis=1:ref=2:me=hex:subme=4:mixed-refs=0 ::default
set x264optsLv5=trellis=1:ref=2:me=hex:subme=6:mixed-refs=1
set x264optsLv6=trellis=1:ref=3:me=hex:subme=7:mixed-refs=1

for /f "delims=" %%i in (%filelist%) do (
	echo %%i
	%ffmpeg_path% -i %%i -vf fps=%videofps% -s %videores% -vframes %framecnt% -y %%i.yuv

	for /l %%j in (0,1,2) do (
		echo %%j
		%ffmpeg_path% %ffmpeg_cfg% -s %videores% -pix_fmt yuv420p -r %videofps% -i %%i.yuv -vcodec libx264 %ffmpeg_opt% -r %videofps% -x264opts %x264opts_opt%:!x264optsLv%%j! -y %%i.264
		%ffmpeg_path% -i %%i.264 -y %%i_dec.yuv
		%ffmpeg_path% -s %videores% -i %%i_dec.yuv -s %videores% -i %%i.yuv -lavfi "psnr" -f null -
	)

	del %%i.yuv
	::del %%i.264
	del %%i_dec.yuv
)

::del $filelist
