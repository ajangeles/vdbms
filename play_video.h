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
						destroyWindow("Face Detection");
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
			printf("Statement error\n");
			printf("The PLAY statement template is: PLAY video FROM <videos/videoclips> WHERE <video_name/videoclip_name> = <video_name/videoclip_name>\n");
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
			printf("Statement error\n");
			printf("The PLAY statement template is: PLAY video FROM <videos/videoclips> WHERE <video_name/videoclip_name> = <video_name/videoclip_name>\n");
		}
	}
}