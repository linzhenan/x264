/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}

/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
	FILE *f;long len;char *data;
	
	f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	doit(data);
	free(data);
}

/* Used by some code below as an example datatype. */
struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };

/* Create a bunch of objects as demonstration. */
void create_objects()
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out;int i;	/* declare a few. */
	/* Our "days of the week" array: */
	const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	/* Our matrix: */
	int numbers[3][3]={{0,-1,0},{1,0,0},{0,0,1}};
	/* Our "gallery" item: */
	int ids[4]={116,943,234,38793};
	/* Our array of "records": */
	struct record fields[2]={
		{"zip",37.7668,-1.223959e+2,"","SAN FRANCISCO","CA","94107","US"},
		{"zip",37.371991,-1.22026e+2,"","SUNNYVALE","CA","94085","US"}};

	/* Here we construct some JSON standards, from the JSON site. */
	
	/* Our "Video" datatype: */
	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
	cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"type",		"rect");
	cJSON_AddNumberToObject(fmt,"width",		1920);
	cJSON_AddNumberToObject(fmt,"height",		1080);
	cJSON_AddFalseToObject (fmt,"interlace");
	cJSON_AddNumberToObject(fmt,"frame rate",	24);
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);	/* Print to text, Delete the cJSON, print it, release the string. */

	/* Our "days of the week" array: */
	root=cJSON_CreateStringArray(strings,7);

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

	/* Our matrix: */
	root=cJSON_CreateArray();
	for (i=0;i<3;i++) cJSON_AddItemToArray(root,cJSON_CreateIntArray(numbers[i],3));

/*	cJSON_ReplaceItemInArray(root,1,cJSON_CreateString("Replacement")); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);


	/* Our "gallery" item: */
	root=cJSON_CreateObject();
	cJSON_AddItemToObject(root, "Image", img=cJSON_CreateObject());
	cJSON_AddNumberToObject(img,"Width",800);
	cJSON_AddNumberToObject(img,"Height",600);
	cJSON_AddStringToObject(img,"Title","View from 15th Floor");
	cJSON_AddItemToObject(img, "Thumbnail", thm=cJSON_CreateObject());
	cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
	cJSON_AddNumberToObject(thm,"Height",125);
	cJSON_AddStringToObject(thm,"Width","100");
	cJSON_AddItemToObject(img,"IDs", cJSON_CreateIntArray(ids,4));

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

	/* Our array of "records": */

	root=cJSON_CreateArray();
	for (i=0;i<2;i++)
	{
		cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
		cJSON_AddStringToObject(fld, "precision", fields[i].precision);
		cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
		cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
		cJSON_AddStringToObject(fld, "Address", fields[i].address);
		cJSON_AddStringToObject(fld, "City", fields[i].city);
		cJSON_AddStringToObject(fld, "State", fields[i].state);
		cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
		cJSON_AddStringToObject(fld, "Country", fields[i].country);
	}
	
/*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

}

int main (int argc, const char * argv[]) {
	/* a bunch of json: */
	/*char text1[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	
	char text2[]="[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[]="[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
	char text4[]="{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[]="[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";*/
    char text6[] = "{\"image_id\": \"1+/E5Rw+/vzkvZL4UAEFGg==\", \"request_id\" : \"1480509727,feeb6216-64ac-4dd1-ad9a-7830b0c5584a\", \"time_used\" : 336, \"faces\" : [{\"landmark\": {\"mouth_upper_lip_left_contour2\": {\"y\": 152, \"x\" : 167}, \"contour_chin\" : {\"y\": 178, \"x\" : 179}, \"mouth_lower_lip_right_contour3\" : {\"y\": 159, \"x\" : 181}, \"contour_right9\" : {\"y\": 175, \"x\" : 188}, \"mouth_upper_lip_left_contour1\" : {\"y\": 151, \"x\" : 171}, \"right_eyebrow_lower_middle\" : {\"y\": 102, \"x\" : 192}, \"left_eyebrow_lower_middle\" : {\"y\": 106, \"x\" : 152}, \"mouth_upper_lip_left_contour3\" : {\"y\": 154, \"x\" : 169}, \"left_eyebrow_lower_left_quarter\" : {\"y\": 106, \"x\" : 147}, \"right_eyebrow_lower_left_quarter\" : {\"y\": 104, \"x\" : 186}, \"right_eyebrow_lower_right_quarter\" : {\"y\": 102, \"x\" : 198}, \"nose_contour_left1\" : {\"y\": 118, \"x\" : 168}, \"left_eyebrow_upper_left_quarter\" : {\"y\": 104, \"x\" : 147}, \"left_eye_bottom\" : {\"y\": 120, \"x\" : 155}, \"mouth_lower_lip_bottom\" : {\"y\": 162, \"x\" : 176}, \"contour_right7\" : {\"y\": 164, \"x\" : 203}, \"left_eyebrow_left_corner\" : {\"y\": 107, \"x\" : 142}, \"contour_right6\" : {\"y\": 157, \"x\" : 208}, \"right_eye_lower_right_quarter\" : {\"y\": 115, \"x\" : 195}, \"right_eye_bottom\" : {\"y\": 117, \"x\" : 190}, \"contour_left7\" : {\"y\": 165, \"x\" : 157}, \"contour_left6\" : {\"y\": 159, \"x\" : 152}, \"contour_left5\" : {\"y\": 151, \"x\" : 148}, \"contour_left4\" : {\"y\": 144, \"x\" : 145}, \"contour_left3\" : {\"y\": 136, \"x\" : 144}, \"contour_left2\" : {\"y\": 128, \"x\" : 142}, \"contour_left1\" : {\"y\": 119, \"x\" : 141}, \"left_eye_lower_left_quarter\" : {\"y\": 119, \"x\" : 151}, \"contour_right1\" : {\"y\": 114, \"x\" : 215}, \"contour_right3\" : {\"y\": 131, \"x\" : 215}, \"contour_right2\" : {\"y\": 123, \"x\" : 215}, \"contour_right5\" : {\"y\": 149, \"x\" : 212}, \"contour_right4\" : {\"y\": 140, \"x\" : 214}, \"contour_left9\" : {\"y\": 176, \"x\" : 169}, \"contour_left8\" : {\"y\": 171, \"x\" : 163}, \"nose_right\" : {\"y\": 139, \"x\" : 183}, \"right_eye_upper_right_quarter\" : {\"y\": 111, \"x\" : 194}, \"nose_contour_right3\" : {\"y\": 143, \"x\" : 179}, \"nose_contour_lower_middle\" : {\"y\": 145, \"x\" : 173}, \"right_eye_top\" : {\"y\": 111, \"x\" : 189}, \"right_eye_right_corner\" : {\"y\": 113, \"x\" : 198}, \"mouth_upper_lip_right_contour1\" : {\"y\": 150, \"x\" : 177}, \"mouth_upper_lip_right_contour2\" : {\"y\": 150, \"x\" : 183}, \"mouth_upper_lip_right_contour3\" : {\"y\": 153, \"x\" : 181}, \"left_eye_left_corner\" : {\"y\": 116, \"x\" : 148}, \"mouth_right_corner\" : {\"y\": 151, \"x\" : 188}, \"mouth_lower_lip_right_contour1\" : {\"y\": 154, \"x\" : 182}, \"contour_right8\" : {\"y\": 170, \"x\" : 196}, \"left_eyebrow_right_corner\" : {\"y\": 107, \"x\" : 163}, \"right_eye_center\" : {\"y\": 114, \"x\" : 189}, \"left_eye_upper_right_quarter\" : {\"y\": 114, \"x\" : 160}, \"mouth_upper_lip_top\" : {\"y\": 152, \"x\" : 174}, \"nose_left\" : {\"y\": 140, \"x\" : 165}, \"left_eye_upper_left_quarter\" : {\"y\": 113, \"x\" : 151}, \"left_eye_center\" : {\"y\": 116, \"x\" : 155}, \"left_eye_lower_right_quarter\" : {\"y\": 119, \"x\" : 159}, \"right_eyebrow_left_corner\" : {\"y\": 105, \"x\" : 179}, \"right_eye_left_corner\" : {\"y\": 117, \"x\" : 182}, \"mouth_lower_lip_top\" : {\"y\": 156, \"x\" : 175}, \"right_eyebrow_right_corner\" : {\"y\": 103, \"x\" : 204}, \"mouth_lower_lip_left_contour1\" : {\"y\": 156, \"x\" : 169}, \"left_eye_pupil\" : {\"y\": 116, \"x\" : 156}, \"mouth_left_corner\" : {\"y\": 154, \"x\" : 163}, \"right_eyebrow_upper_left_quarter\" : {\"y\": 101, \"x\" : 185}, \"right_eye_lower_left_quarter\" : {\"y\": 117, \"x\" : 186}, \"nose_tip\" : {\"y\": 140, \"x\" : 172}, \"right_eye_upper_left_quarter\" : {\"y\": 113, \"x\" : 185}, \"left_eyebrow_upper_middle\" : {\"y\": 103, \"x\" : 152}, \"mouth_lower_lip_right_contour2\" : {\"y\": 155, \"x\" : 185}, \"mouth_lower_lip_left_contour3\" : {\"y\": 160, \"x\" : 170}, \"nose_contour_left2\" : {\"y\": 135, \"x\" : 167}, \"nose_contour_left3\" : {\"y\": 143, \"x\" : 169}, \"nose_contour_right1\" : {\"y\": 117, \"x\" : 176}, \"nose_contour_right2\" : {\"y\": 134, \"x\" : 180}, \"mouth_lower_lip_left_contour2\" : {\"y\": 157, \"x\" : 166}, \"right_eyebrow_upper_right_quarter\" : {\"y\": 99, \"x\" : 198}, \"right_eyebrow_upper_middle\" : {\"y\": 99, \"x\" : 191}, \"left_eyebrow_lower_right_quarter\" : {\"y\": 107, \"x\" : 158}, \"left_eye_top\" : {\"y\": 113, \"x\" : 155}, \"left_eye_right_corner\" : {\"y\": 118, \"x\" : 163}, \"left_eyebrow_upper_right_quarter\" : {\"y\": 104, \"x\" : 158}, \"right_eye_pupil\" : {\"y\": 113, \"x\" : 190}, \"mouth_upper_lip_bottom\" : {\"y\": 154, \"x\" : 175}}, \"attributes\" : {\"gender\": {\"value\": \"Female\"}, \"age\" : {\"value\": 34}}, \"face_rectangle\" : {\"width\": 84, \"top\" : 89, \"left\" : 135, \"height\" : 84}, \"face_token\" : \"efe3569d76492f686ad85471ef5c15d6\"}]}";
	/* Process each json textblock by parsing, then rebuilding: */
	/*doit(text1);
	doit(text2);	
	doit(text3);
	doit(text4);
	doit(text5);*/
    doit(text6);
	/* Parse standard testfiles: */
/*	dofile("../../tests/test1"); */
/*	dofile("../../tests/test2"); */
/*	dofile("../../tests/test3"); */
/*	dofile("../../tests/test4"); */
/*	dofile("../../tests/test5"); */

	/* Now some samplecode for building objects concisely: */
	//create_objects();
	
	return 0;
}
