#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "my_global.h" 
#include "mysql.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <direct.h>

using namespace cv;
using namespace std;

#include "insert_video.h"
#include "detect_face.h"
#include "play_video.h"
#include "select_video.h"
#include "update_video.h"
#include "delete_video.h"

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "admin"
#define DATABASE "video_dbms"

int main()
{
    MYSQL *connect;
	MYSQL *connect2;
	MYSQL_RES *res;
	int num_rows;
	
    connect=mysql_init(NULL); 
	connect2=mysql_init(NULL); 
  
    if(!connect)   
    {
        fprintf(stderr,"MySQL Initialization Failed");
        return 1;
    }
	
	connect2=mysql_real_connect(connect2,SERVER,USER,PASSWORD,NULL,0,NULL,0);
	mysql_query(connect2, "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'video_dbms'");
	res = mysql_store_result(connect2);
	num_rows = mysql_num_rows(res);
	
	if(num_rows == 0){
		mysql_query(connect2, "CREATE DATABASE video_dbms");
		mysql_query(connect2, "USE video_dbms");
		mysql_query(connect2, "CREATE TABLE videos(video_id INT NOT NULL AUTO_INCREMENT, video_name VARCHAR(100) NOT NULL UNIQUE, video_ext VARCHAR(5) NOT NULL, video_path VARCHAR(100) NOT NULL, date_added TIMESTAMP DEFAULT CURRENT_TIMESTAMP, is_valid CHAR(1) DEFAULT 'N', position VARCHAR(50), detect_count INT DEFAULT 0, new_name VARCHAR(100), PRIMARY KEY (video_id))");
		mysql_query(connect2, "CREATE TABLE videoclips(videoclip_id INT NOT NULL AUTO_INCREMENT, videoclip_name VARCHAR(100) UNIQUE, video_id INT, date_added TIMESTAMP DEFAULT CURRENT_TIMESTAMP, is_valid CHAR(1) DEFAULT 'Y', position VARCHAR(50), new_name VARCHAR(100), PRIMARY KEY (videoclip_id))");
		connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
	}
	else{
		connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
	}

	struct stat info;
	const char *pathname = "C:/Video Database";
	if(stat( pathname, &info ) != 0){
		mkdir("C:/Video Database");
		mkdir("C:/Video Database/Videos");
		mkdir("C:/Video Database/Video Clips");
	}

	char query[1000];
	char lowQuery[1000];

	printf("videodbms> ");
	gets(query);

	while(!strstr(query,"exit")){
		for(int i = 0; i < sizeof(query); i++){
		  if(query[i] == '\\'){
			query[i] = '/';
		  }
		  lowQuery[i] = tolower(query[i]);
		}
		
		if(strstr(lowQuery, "detect")){
			detectFaces(connect, lowQuery);
		}
		else if (strstr(lowQuery, "play")){
			playVideo(connect, lowQuery);
		}
		else if(strstr(lowQuery, "clear")){
			system("cls");
		}
		else if (strstr(lowQuery, "insert")){
			if(strstr(lowQuery, "videos")){
				if(mysql_query(connect, query) == 0){
					insertVideo(connect);
				}
				else{
					printf(mysql_error(connect));
					printf("\n");
				}
			}
			else{
				printf("Invalid query.\n");
			}
		}
		else if (strstr(lowQuery, "select")){
			if(mysql_query(connect, query) == 0){
				selectVideo(connect, query);
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else if (strstr(lowQuery, "update")){
			updateVideo(connect, query);
		}
		else if (strstr(lowQuery, "delete")){
			deleteVideo(connect, query);
		}
		else{
			printf("Invalid query\n");
		}

		printf("videodbms> ");
		gets(query);
	}

    mysql_close(connect);   
    return 0;
}