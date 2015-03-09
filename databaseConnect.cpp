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

void playVideo(MYSQL *connect, char query[]){
	string playQuery;
	std::string que = query;
	std::string video_name;
	std::string video_ext;
	int counter = 0;
	char sql[100] = "";
	MYSQL_RES *res; 
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	int num_fields;

	if(strstr(query, "videos")){
		playQuery = "play video from videos where video_name";

		for(int i=0;i<playQuery.length();i++){
			if(que[i] == playQuery[i]){
				counter++;
			}
		}

		if(counter == playQuery.length()){
			que = query;

			que.replace(0,10, "select video_name, video_ext");

			for(int i=0;i<que.length();i++){
				sql[i] = que[i];
			}

			if(mysql_query(connect, sql) == 0){
				res = mysql_store_result(connect);
				num_fields = mysql_num_fields(res);
				row = mysql_fetch_row(res);
				video_name = row[0];
				video_ext = row[1];

				if(res != NULL)
				   mysql_free_result(res);

				std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
				VideoCapture cap(path.append(video_name).append(".").append(video_ext));
				if(!cap.isOpened()){
					cout << "Error opening video stream or file" << endl;
				}
				bool stop(false);

				while(!stop)
				{
					Mat image;
					cap >> image;

					imshow( "Face Detection", image );

					if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
					{
						cout << "esc key is pressed by user" << endl;
						break; 
					}
				}


			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}

		}
		else{
			printf("The PLAY statement template is: PLAY video FROM videos WHERE video_name = <video_name>\n");
		}
		
	}
	else if(strstr(query, "videoclips")){
		playQuery = "play video from videoclips where videoclip_name";

		for(int i=0;i<playQuery.length();i++){
			if(que[i] == playQuery[i]){
				counter++;
			}
		}

		if(counter == playQuery.length()){
			que = query;

			que.replace(0,10, "select videoclip_name, videoclip_ext");

			for(int i=0;i<que.length();i++){
				sql[i] = que[i];
			}

			if(mysql_query(connect, sql) == 0){
				res = mysql_store_result(connect);
				num_fields = mysql_num_fields(res);
				row = mysql_fetch_row(res);
				video_name = row[0];
				video_ext = row[1];

				if(res != NULL)
				   mysql_free_result(res);

				std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
				VideoCapture cap(path.append(video_name).append(".").append(video_ext));
				if(!cap.isOpened()){
					cout << "Error opening video stream or file" << endl;
				}
				bool stop(false);

				while(!stop)
				{
					Mat image;
					cap >> image;

					imshow( "Face Detection", image );

					if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
					{
						cout << "esc key is pressed by user" << endl;
						break; 
					}
				}


			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}

		}
		else{
			printf("The PLAY statement template is: PLAY video FROM videoclips WHERE videoclip_name = <videoclip_name>\n");
		}
	}
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
	   for(int i = 0; i < num_fields; i++)
	   {
			  if(row[i] != NULL)
			   printf("%s\t", row[i]);
		   else
				cout << "NULL" << endl;
	   }
	}

	printf("\n");

	if(res != NULL)
		mysql_free_result(res);
}

void updateVideo(MYSQL *connect, char query[]){
	int count = 0;

	for(int i=0; i<strlen(query); i++){
		if(query[i] == ' '){
			 count++;
		 }
	}

	count++;

	if(strstr(query, "videos")){
		if(strstr(query, "video_id") || strstr(query, "video_ext") || strstr(query, "video_path") || strstr(query, "date_added") || strstr(query, "is_valid")){
			printf("The UPDATE statement template is: UPDATE videos SET video_name = <new_name> WHERE video_name = <old_name>\n");
		}
		else if(strstr(query, "video_name") && count == 10){
			if(mysql_query(connect, query) == 0){
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
	else if(strstr(query, "videoclips")){
		if(strstr(query, "videoclip_id") || strstr(query, "videoclip_ext") || strstr(query, "videoclip_path") || strstr(query, "video_id") || strstr(query, "date_added") || strstr(query, "is_valid")){
			printf("The UPDATE statement template is: UPDATE videoclips SET videoclip_name = <new_name> WHERE videoclip_name = <old_name>\n");
		}
		else if(strstr(query, "videoclip_name") && count == 10){
			if(mysql_query(connect, query) == 0){
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
}

void deleteVideo(MYSQL *connect, char query[]){
	std::string que = query;
	std::string video_name;
	std::string video_ext;
	char que2[100] = "";
	char sql2[100] = "";
	char path2[100] = "";
	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;
		
	if (strstr(query, "videos")){
		que.replace(0, 6, "SELECT video_name, video_ext");

		for(int i=0;i<que.length();i++){
			que2[i] = que[i];
		}
	
		if(mysql_query(connect, que2) == 0){
			res = mysql_store_result(connect);
			num_fields = mysql_num_fields(res);
			row = mysql_fetch_row(res);
			video_name = row[0];
			video_ext = row[1];

			if(res != NULL)
			   mysql_free_result(res);

			std::string sql = "UPDATE videos SET is_valid = 'N' where video_name = '";
			sql.append(video_name).append("'");

			for(int i=0;i<sql.length();i++){
				sql2[i] = sql[i];
			}

			if(mysql_query(connect, sql2) == 0){
				std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
				path.append(video_name).append(".").append(video_ext);

				for(int i=0;i<path.length();i++){
					path2[i] = path[i];
				}

				if(remove(path2)!= 0){
					perror("Error deleting file");
				}
				else{
					puts("File successfully deleted");
					if(mysql_query(connect, query) == 0){
					}
					else{
						printf(mysql_error(connect));
						printf("\n");
					}
				}
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else{
			printf(mysql_error(connect));
			printf("\n");
		}
	}
	else if (strstr(query, "videoclips")){
		que.replace(0, 6, "SELECT videoclip_name, videoclip_ext");

		for(int i=0;i<que.length();i++){
			que2[i] = que[i];
		}
	
		if(mysql_query(connect, que2) == 0){
			res = mysql_store_result(connect);
			num_fields = mysql_num_fields(res);
			row = mysql_fetch_row(res);
			video_name = row[0];
			video_ext = row[1];

			if(res != NULL)
			   mysql_free_result(res);

			std::string sql = "UPDATE videoclips SET is_valid = 'N' where videoclip_name = '";
			sql.append(video_name).append("'");

			for(int i=0;i<sql.length();i++){
				sql2[i] = sql[i];
			}

			if(mysql_query(connect, sql2) == 0){
				std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
				path.append(video_name).append(".").append(video_ext);

				for(int i=0;i<path.length();i++){
					path2[i] = path[i];
				}

				if(remove(path2)!= 0){
					perror("Error deleting file");
				}
				else{
					puts("File successfully deleted");
					if(mysql_query(connect, query) == 0){
					}
					else{
						printf(mysql_error(connect));
						printf("\n");
					}
				}
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else{
			printf(mysql_error(connect));
			printf("\n");
		}
	}
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

		for(int i = 0; i < sizeof(query); i++){
		  query[i] = tolower(query[i]);
		}

		if(strstr(query, "detect")){
			detectFaces();
		}
		else if (strstr(query, "play")){
			playVideo(connect, query);
		}
		else if (strstr(query, "insert")){
			if(mysql_query(connect, query) == 0){
				insertVideo(connect);
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else if (strstr(query, "select")){
			if(mysql_query(connect, query) == 0){
				selectVideo(connect, query);
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else if (strstr(query, "update")){
			updateVideo(connect, query);
		}
		else if (strstr(query, "delete")){
			deleteVideo(connect, query);
		}
		else{
			printf("Invalid query");
		}
	}
 
    mysql_close(connect);   
    return 0;
}