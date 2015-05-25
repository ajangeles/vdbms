/*
	NOTE: The user's input is an play statement. 
		  The user can only play one video/videoclips at a time.
		  
	Basic Syntax:
		videos table: PLAY video FROM videos WHERE video_name = '<video_name>';
			Ex. PLAY video FROM videos WHERE video_name = 'video1';
		videoclips table: PLAY video FROM videoclips WHERE videoclip_name = '<videoclip_name>';
			Ex. PLAY video FROM videoclips WHERE videoclip_name = 'videoclip1';
					
	This function:
	1. Checks if there is the PLAY VIDEO keyword in the user's input.
	2. Checks whether the user wants to play a video or videoclip.
	3. Check whether the user's input contains exactly the string 'PLAY video FROM videos/videoclips WHERE video_name/videoclip_name'.
	4. Get the details of the video the user wants to play. (SELECT statement)
	4. Play video or video clip.
	5. If ESC key is pressed, stop video.

*/

void playVideo(MYSQL *connect, char query[]){
	string playQuery;
	std::string que = query;
	std::string video_name;
	std::string videoclip_name;
	int counter = 0;
	char sql[100] = "";
	MYSQL_RES *res; 
	MYSQL_ROW row;
	int num_rows;

	if(strstr(query, "play video")){ //check if the user's input has PLAY VIDEO keyword in it
		if(strstr(query, "videos") && strstr(query, "=")){ //check if the user wants to play video in the videos table AND this function only accepts the '=' operator
			playQuery = "play video from videos where video_name";

			if(playQuery.length() > que.length()){
				printf("Invalid query.\n");
			}
			else{
				for(int i=0;i<playQuery.length();i++){
					if(que[i] == playQuery[i]){
						counter++;
					}
				}

				if(counter == playQuery.length()){
					que = query;

					que.replace(0,10, "select video_name"); //convert the play statement into a select statement

					for(int i=0;i<que.length();i++){
						sql[i] = que[i];
					}

					if(mysql_query(connect, sql) == 0){
						res = mysql_store_result(connect);
						num_rows = mysql_num_rows(res);
					
						if(num_rows == 1){ //there must only be one row generated
							row = mysql_fetch_row(res);
							video_name = row[0];

							if(res != NULL)
							   mysql_free_result(res);

							std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
							VideoCapture cap(path.append(video_name).append(".").append("avi")); //open video
							if(!cap.isOpened()){
								cout << "Error opening video stream or file" << endl;
							}
							else{
								bool stop(false);

								float frameCount = 0; 
								float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

								while(frameCount<totalFrame)
								{
									Mat image; 
									cap >> image; //get video frames
									frameCount++;

									imshow("VIDEO DBMS", image); //play video

									if (waitKey(10) == 27) //ESC key is pressed
									{
										cout << "Cancelled." << endl;
										destroyWindow("VIDEO DBMS"); //close video
										break; 
									}
								}

								if(frameCount==totalFrame){
									destroyWindow("VIDEO DBMS"); //close video
								}

							}
						}
						else{ 
							printf("Empty set or more than 1 row is generated.\n");
						}
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
		}
		else if(strstr(query, "videoclips") && strstr(query, "=")){ //check if the user wants to play video in the videoclips table AND this function only accepts the '=' operator
			playQuery = "play video from videoclips where videoclip_name";

			if(playQuery.length() > que.length()){
				printf("Invalid query.\n");
			}
			else{
				for(int i=0;i<playQuery.length();i++){
					if(que[i] == playQuery[i]){
						counter++;
					}
				}

				if(counter == playQuery.length()){
					que = query;

					que.replace(0,10, "select videoclip_name"); //convert the play statement into a select statement

					for(int i=0;i<que.length();i++){
						sql[i] = que[i];
					}

					if(mysql_query(connect, sql) == 0){
						res = mysql_store_result(connect);
						num_rows = mysql_num_rows(res);
					
						if(num_rows == 1){ //there must only be one row generated
							row = mysql_fetch_row(res);
							videoclip_name = row[0];

							if(res != NULL)
							   mysql_free_result(res);

							std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
							VideoCapture cap(path.append(videoclip_name).append(".").append("avi")); //open video
							if(!cap.isOpened()){
								cout << "Error opening video stream or file" << endl;
							}
							else{
								float frameCount = 0; 
								float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

								while(frameCount<totalFrame)
								{
									Mat image; 
									cap >> image; //get video frames
									frameCount++;

									imshow( "VIDEO DBMS", image ); //play video

									if (waitKey(10) == 27) //ESC key is pressed
									{
										cout << "Cancelled." << endl;
										destroyWindow("VIDEO DBMS"); //close video
										break; 
									}
								}

								if(frameCount==totalFrame){
									destroyWindow("VIDEO DBMS"); //close video
								}

							}
						}
						else{ 
							printf("Empty set or more than 1 row is generated.\n");
						}
					}
					else{ 
						printf(mysql_error(connect));
						printf("\n");
					}
				}
			}
		}
		else{ //videos or videoclips not found in user query
			printf("Invalid query.\n");
		}
	}
	else{ //'PLAY VIDEO' not found in user query
		printf("Invalid query.\n");
	}
}