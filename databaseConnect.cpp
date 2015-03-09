#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "my_global.h" 
#include "mysql.h"
#include <stdio.h>
#include <conio.h>
#include<string.h>
#include <iostream>

using namespace cv;
using namespace std;

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "admin"
#define DATABASE "video_dbms"
 
void detectFaces(){
	/*VideoCapture cap("C:\\Users\\Kevin\\Videos\\2.mp4"); 
	if(!cap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	bool stop(false);

	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("D:/out.avi",CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true);

	CascadeClassifier face_cascade;
	face_cascade.load( "C:/OpenCV2410/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml" );

	while(!stop)
	{
		Mat image;
		cap >> image;

		std::vector<Rect> faces;
		face_cascade.detectMultiScale( image, faces, 1.1, 6, CV_HAAR_DO_CANNY_PRUNING, Size(30, 30) );

		for( int i = 0; i < faces.size(); i++ )
		{
			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
			Point pt2(faces[i].x, faces[i].y);
 
			rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
		}

		video.write(image);
		imshow( "Face Detection", image );

		if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}

	cap.release();*/
	printf("DETECT!");
}

void insertVideo(MYSQL *connect){
	std::string video_name;
	std::string video_ext;
	std::string video_path;

	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;

	mysql_query(connect, "SELECT video_name FROM videos WHERE is_valid = 'N'");
    res = mysql_store_result(connect);
	num_fields = mysql_num_fields(res);
	row = mysql_fetch_row(res);
	video_name = row[0];
	if(res != NULL)
       mysql_free_result(res);

	mysql_query(connect, "SELECT video_ext FROM videos WHERE is_valid = 'N'");
    res = mysql_store_result(connect);
	num_fields = mysql_num_fields(res);
	row = mysql_fetch_row(res);
	video_ext = row[0];
	if(res != NULL)
       mysql_free_result(res);

	mysql_query(connect, "SELECT video_path FROM videos WHERE is_valid = 'N'");
    res = mysql_store_result(connect);
	num_fields = mysql_num_fields(res);
	row = mysql_fetch_row(res);
	video_path = row[0];
	if(res != NULL)
       mysql_free_result(res);

	VideoCapture cap(video_path.append(video_name).append(".").append(video_ext)); 
	if(!cap.isOpened()){
		mysql_query(connect, "DELETE FROM videos WHERE is_valid = 'N'");
	}
	else{
		bool stop(false);

		int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
		int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
		std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
		VideoWriter video(path.append(video_name).append(".avi"),CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true);

		float frameCount = 0;
		float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

		while(frameCount<=totalFrame)
		{
			Mat image;
			cap >> image;
			video.write(image);
			frameCount++;

			std::cout << (int) /*(frameCount/totalFrame)*100*/ frameCount/totalFrame*100 << '%';
			std::cout.flush(); // see wintermute's comment
			std::cout << '\r';
		}

		cap.release();
		mysql_query(connect, "UPDATE videos SET is_valid = 'Y' where is_valid = 'N'");
	}
}

void selectVideo(MYSQL *connect, char query[]){

}

void deleteVideo(MYSQL *connect, char query[]){
	/*std::string query2 = query;
	std::string video_name;

	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;

	query2.replace(query2.begin(), query2.end(),"SELECT *");
	
	res = mysql_store_result(connect);
	num_fields = mysql_num_fields(res);
	row = mysql_fetch_row(res);
	video_name = row[0];
	if(res != NULL)
       mysql_free_result(res);

	std::string sql = "UPDATE videos SET is_valid = 'N' where video_name = '";
	mysql_query(connect, sql.append(video_name).append("'"));


		
		if (strstr(query, "videos")){
			insertVideo(connect);
			printf("INSERT SUCCESSFULY.");
		}
		else if (strstr(query, "videoclips")){
			deleteVideo(connect, query);
			printf("DELETE SUCCESSFULY.");	
		}*/
}

int main()
{
    MYSQL *connect;
    connect=mysql_init(NULL); 
  
    if(!connect)   
    {
        fprintf(stderr,"MySQL Initialization Failed");
        return 1;
    }
 
    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);

	char query[100];

	while(query != "exit"){
		printf("videodbms> ");
		gets(query);

		if(strstr(query, "DETECT")){
			detectFaces();
		}
		else if (strstr(query, "INSERT")){
			if(mysql_query(connect, query) == 0){
				insertVideo(connect);
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else if (strstr(query, "SELECT")){
			if(mysql_query(connect, query) == 0){
				MYSQL_RES *res; 
				MYSQL_ROW row;
				MYSQL_FIELD *field;

				res = mysql_store_result(connect);
				int num_fields = mysql_num_fields(res);

				for(int i=0;field = mysql_fetch_field(res); i++) {
					printf("%s\t", field->name);
				}

				printf("\n");

				while ((row = mysql_fetch_row(res)))
				{
				   // Print all columns
				   for(int i = 0; i < num_fields; i++)
				   {
					   // Make sure row[i] is valid!
					   if(row[i] != NULL)
						   printf("%s\t", row[i]);
					   else
							cout << "NULL" << endl;

					   // Also, you can use ternary operator here instead of if-else
					   // cout << row[i] ? row[i] : "NULL" << endl;
				   }
				}

				printf("\n");

				if(res != NULL)
					mysql_free_result(res);

			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		/*else if (strstr(query, "DELETE")){
			deleteVideo(connect, query);
			printf("DELETE SUCCESSFULY.");
		}*/
	}
 
    mysql_close(connect);   
    return 0;
}