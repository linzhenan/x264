#https://console-cloud.megvii.com/documents/4888373

#echo

api_key="J-7aHkzFQqdl0HtP31EalRzKuMMzNSR3"
api_secret="Swu5hXw9tCzbZInab7J9qAiVLc57XOt0"
#image_filename="image-001.jpeg"
image_filename=$1

#echo $api_key
#echo $api_secret
#echo $image_path

echo $image_path

#:<<!
curl -X POST "https://api.megvii.com/facepp/v3/detect" -F "api_key=$api_key" \
-F "api_secret=$api_secret" \
-F "image_file=@$image_filename" \
-F "return_landmark=1" \
-F "return_attributes=gender,age"
#!
