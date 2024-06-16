ffmpeg -loop 1 -i $1 -c:v libx264 -t 15 -pix_fmt yuv420p -vf scale=1920:1080 $2
