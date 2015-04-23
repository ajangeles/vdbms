#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "my_global.h" 
#include "mysql.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

#include "insert_video.h"
#include "detect_face.h"
#include "play_video.h"
#include "select_video.h"
#include "edit_video.h"
#include "delete_video.h"

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "admin"
#define DATABASE "video_dbms"

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

	char query[1000];
	char lowQuery[1000];

	while(query != "exit"){
		printf("videodbms> ");
		gets(query);

		for(int i = 0; i < sizeof(query); i++){
		  if(query[i] == '\\'){
			query[i] = '/';
		  }
		  lowQuery[i] = tolower(query[i]);
		}
		
		if(strstr(lowQuery, "detect")){
			detectFaces(connect, query);
		}
		else if (strstr(lowQuery, "play")){
			playVideo(connect, query);
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
			editVideo(connect, query);
		}
		else if (strstr(lowQuery, "delete")){
			deleteVideo(connect, query);
		}
		else{
			printf("Invalid query\n");
		}
	}
 
    mysql_close(connect);   
    return 0;
}