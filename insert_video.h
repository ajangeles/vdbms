void insertVideo(MYSQL *connect){
	std::string video_name;
	std::string video_ext;
	std::string video_path;

	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;
	int num_rows;
	int vidCount = 0;
	int key;
	int is_cancel = 0;

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

				if((frameCount/totalFrame*100) <= 100){
					printf("\r%.2f%%", (frameCount/totalFrame*100));
				}

				if(kbhit()){
					key = getch();
					if(key == 27){
						is_cancel = 1;
						break;	 
					}
				}
			}
			
			if(is_cancel == 1){
				mysql_query(connect, "DELETE FROM videos WHERE is_valid = 'N'");
				cout << "\nESC key is pressed" << endl;
				break;
			}

			printf("\n");

			cap.release();
			
			strcpy(qstring, "UPDATE videos SET is_valid = 'Y' where video_name = '");
			vn = video_name.c_str();
			strcat(qstring, vn);
			strcat(qstring, "'");

			mysql_query(connect, qstring);
		}
	}

	if(is_cancel == 1){
		std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
		path.append(video_name).append(".").append(video_ext);
		char path2[100] = "";

		for(int i=0;i<path.length();i++){
			path2[i] = path[i];
		}

		if(remove(path2)!= 0){
			perror("Error deleting file");
		}
	}
}