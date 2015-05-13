/*
	NOTE: The user's input is an insert statement. 
		  The user can only insert data on the videos table.
		  
	Basic Syntax:
		Single:   INSERT INTO videos (video_name, video_ext, video_path) VALUES ('<video_name>', '<video_ext>', '<video_path>');
			Ex. INSERT INTO videos (video_name, video_ext, video_path) VALUES ('video1', 'mp4', 'C:/Users/Kevin/Desktop');
		Multiple: INSERT INTO videos (video_name, video_ext, video_path) VALUES ('<video_name>', '<video_ext>', '<video_path>'), ('<video_name>', '<video_ext>', '<video_path>'), ...;
			Ex. INSERT INTO videos (video_name, video_ext, video_path) VALUES ('video1', 'mp4', 'C:/Users/Kevin/Desktop'), ('video2', 'mp4', 'C:/Users/Kevin/Desktop');
	
	This function:
	1. first retrieves the rows where the value of its is_valid column is 'N'.
		(If the value of the row's is_valid column is 'N', it means that the video is not yet copied into the server)
	2. will open the videos one by one then copy the videos into the server frame by frame.
		2.1 if the video is not found, it will delete its record in the database
		2.2 if a user pressed the ESC button while copying, the remaining videos that are not completely copied/copied yet will be deleted in the database.
		2.3 the video that is not copied completely will be deleted in the server.
	3. Update the value of the is_valid column of the completely copied video into 'Y'.

	P.S: The format of the output video is AVI.
*/

void insertVideo(MYSQL *connect){
	//this is the variables used to store the retrieved data from the database
	std::string video_name;  
	std::string video_ext;
	std::string video_path;

	MYSQL_RES *res; //store the result of the query
	MYSQL_ROW row;  //array used to access the result of the query
	int num_fields;
	int num_rows;
	int vidCount = 0; //used in printf("Inserting %d of %d video(s)...\n", vidCount, num_rows);
	int key; //if ESC key is pressed
	int is_cancel = 0; //boolean 0-not cancelled 1-cancelled

	char qstring[100];
	const char *vn;
	const char *p;

	mysql_query(connect, "SELECT video_name, video_ext, video_path FROM videos WHERE is_valid = 'N'");
    res = mysql_store_result(connect);
	num_fields = mysql_num_fields(res);
	num_rows = mysql_num_rows(res);

	for(int i=0;i<num_rows;i++){
		row = mysql_fetch_row(res);

		vidCount ++;
		printf("Inserting %d of %d video(s)...\n", vidCount, num_rows);

		video_name = row[0];
		video_ext = row[1];
		video_path = row[2];

		if(!(video_path.back() == '/')){ //convert the value of the video_path. Change '\' into '/' and end it with '/'
			video_path.append("/");
		}

		VideoCapture cap(video_path.append(video_name).append(".").append(video_ext)); 

		if(!cap.isOpened()){ //delete the data of the video in the database if it is not found
			printf("Video not found or is an unsupported format\n");
			strcpy(qstring, "DELETE FROM videos WHERE video_name = '");
			vn = video_name.c_str();
			strcat(qstring, vn);
			strcat(qstring, "'");

			mysql_query(connect, qstring);
		}
		else{
			bool stop(false);

			int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); //video width
			int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //video height
			std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/"; //path of the video directory in the server
			VideoWriter video(path.append(video_name).append(".avi"),CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true); //the inserted video will be in avi format.


			float frameCount = 0; //used in computing the percentage of insertion of a video
			float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

			while(frameCount<totalFrame)
			{
				Mat image;

				cap >> image;

				video.write(image);
				frameCount++;

				if((frameCount/totalFrame*100) <= 100){ //show percentage of the video currently inserted
					printf("\r%.2f%% ", (frameCount/totalFrame*100));
					cout << "of " << video_name << "." << video_ext << " inserted.";
				}

				if(kbhit()){ //ESC key is pressed
					key = getch();
					if(key == 27){
						is_cancel = 1;
						break;	 
					}
				}
			}

			if(is_cancel == 1){ //if the user cancelled the insert operation, delete the not completely inserted/not inserted yet videos in the database
				mysql_query(connect, "DELETE FROM videos WHERE is_valid = 'N'");
				cout << "\nOperation cancelled." << endl;
				break;
			}

			printf("\n");

			cap.release();
			
			//the video was completely inserted in the server. Update record.
			strcpy(qstring, "UPDATE videos SET is_valid = 'Y' where video_name = '");
			vn = video_name.c_str();
			strcat(qstring, vn);
			strcat(qstring, "'");

			mysql_query(connect, qstring);
		}
	}

	if(is_cancel == 1){ //delete the not completely inserted video in the server
		std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
		path.append(video_name).append(".").append("avi");
		char path2[100] = "";

		for(int i=0;i<path.length();i++){
			path2[i] = path[i];
		}

		if(remove(path2)!= 0){
			perror("Error deleting file");
		}
	}

	if(res != NULL)
		mysql_free_result(res);
}