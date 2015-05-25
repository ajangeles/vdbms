/* NOTE: The user's input is an detect statement. 
		 The user can only detect videos in the videos table
		  
	Basic Syntax:
		Single: DETECT face SET position = '<direction>' WHERE video_name = '<video_name>';
			Ex. DETECT face SET position = 'N, E, S, W' WHERE video_name = 'video1';
		Multiple: DETECT face SET position = '<direction>' WHERE video_name = '<video_name>', SET position = '<direction'> WHERE video_name = '<video_name>';
			Ex. DETECT face SET position = 'NE, CN, NW' where video_name = 'video1', SET position = 'SE, CS, SW' where video_name = 'video2';

		NOTE: 
			Directions: NE, CN, NW, CE, C, CW, SE, CS, SW, N, E, S, W, and ALL

*/

void detectFaces(MYSQL *connect, char query[]){
	std::string que = query;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_rows; 
	int key;
	int is_cancel = 0; //if the user pressed the ESC key is_cancel = 1
	int counter = 0;
	int vidCount = 0;
	int frameCount = 0;
	int hours = 0;
	int mins = 0;
	int secs = 0;
	int fps;
	int fps_count = 0;
	std::string video_name;
	std::string position;
	std::string detect_count;
	std::string video_id;
	std::string posTemp; //used in tokenizing query and position
	std::string TestStr; 
	std::string hour;
	std::string min;
	std::string sec;
	std::string tempChar;
	char qstring[100]; //another variable used to store the query 
	char qstring2[100]; //another variable used to store the query
	char qstring3[100]; //another variable used to store the query
	char qstring4[100]; //another variable used to store the query
	const char *vn; //another variable used to store the video_name
	int is_frame = 0; //if there is at least one frame with detected face is_frame = 1
	int is_error = 0; //if there is an error in the detect statement of the user is_error = 1 
	int is_pos = 0; //if there is an invalid position in the user's query is_pos = 1
	int is_noface = 0;
	std::vector<std::string> no_face; //array to store the videoclips name of those videoclips who have no frames with detected face
	std::vector<std::string> queries;

	if(strstr(query, "detect faces")){ //If the user's input has DETECT faces in it, it is invalid. The keyword should be DETECT face
		printf("Invalid query.\n");
	}
	else if(strstr(query, "detect face") && strstr(query, "position") && strstr(query, "video_name")){ //detect face, position, and video_name are the important words to be found in the user's query
		if(strstr(query, "video_id") || strstr(query, "video_ext") || strstr(query, "video_path") || strstr(query, "date_added") || strstr(query, "is_valid") || strstr(query, "detect_count")){ //the columns only allowed in the user's input are position and video_name
			printf("Invalid query.\n");
		}
		else{
			std::replace(que.begin(), que.end(), '"', '\''); //replace double quotes (") with single quotes (')
			if(que.find("\' ,",0) != std::string::npos){
				que.replace(que.find("\' ,"), sizeof("\' ,")-1, "\',"); //replace (' ,) with (',)
			}
			if(que.find("\',set",0) != std::string::npos){
				que.replace(que.find("\',set"), sizeof("\',set")-1, "\', set"); //replace (',set) with (', set)
			}
			if(que.find("\' ,set",0) != std::string::npos){
				que.replace(que.find("\' ,set"), sizeof("\' ,set")-1, "\', set"); //replace (' ,set) with (', set)
			}
	
			std::string temp;
			char temp2[100];

			posTemp = que;
			//separate multiple query of the detect face sql 
			while (posTemp.find("',",0) != std::string::npos)
			{ 
				size_t  pos = posTemp.find("',",0); 
				temp = posTemp.substr(0, pos+1);    
				posTemp.erase(0, pos + 2);
				counter++;
				if(counter == 1){
					temp.replace(0,11,"update videos"); //transform detect query into an update query
					strcpy(temp2, temp.c_str());
					if(mysql_query(connect, temp2) == 0){
						num_rows = mysql_affected_rows(connect);
						if(num_rows != 1){
							is_error = 1;
						}
						else{
							queries.push_back(temp);
						}
					}
					else{
						printf(mysql_error(connect));
						is_error = 1;
						printf("\n");
					}
				}
				else{
					temp.replace(0,0,"update videos");
					strcpy(temp2, temp.c_str());
					if(mysql_query(connect, temp2) == 0){
						num_rows = mysql_affected_rows(connect);
						if(num_rows != 1){
							is_error = 1;
						}
						else{
							queries.push_back(temp);
						}
					}
					else{
						printf(mysql_error(connect));
						is_error = 1;
						printf("\n");
					}
				}
			}

			if(counter == 0){ //the user's input is a single query
				posTemp.replace(0,11,"update videos");
				strcpy(temp2, posTemp.c_str());
				if(mysql_query(connect, temp2) == 0){
					num_rows = mysql_affected_rows(connect);
					if(num_rows != 1){
						is_error = 1;
					}
					else{
						queries.push_back(posTemp);
					}
				}
				else{
					printf(mysql_error(connect));
					is_error = 1;
					printf("\n");
				}
			}
			else{ //last single query in a multiple query
				posTemp.replace(0,0,"update videos");
				strcpy(temp2, posTemp.c_str());
				if(mysql_query(connect, temp2) == 0){
					num_rows = mysql_affected_rows(connect);
					if(num_rows != 1){
						is_error = 1;
					}
					else{
						queries.push_back(posTemp);
					}
				}
				else{
					printf(mysql_error(connect));
					is_error = 1;
					printf("\n");
				}
			}
			
			mysql_query(connect, "UPDATE videos SET position = NULL");

			
				if(is_error == 1){ //the user's input is an invalid query
					printf("Invalid query.\n");
					mysql_query(connect, "UPDATE videos SET position = null");
				}
				else{
					for(int s=0;s<queries.size();s++){
						char temp2[100] = "";
						strcpy(temp2, queries[s].c_str());
						mysql_query(connect, temp2);
						num_rows = queries.size();

						if(mysql_query(connect, "SELECT video_name, position, detect_count, video_id FROM videos WHERE position is not null") == 0){
							res = mysql_store_result(connect);

							row = mysql_fetch_row(res);

							video_name = row[0];
							position = row[1];
							detect_count = row[2];
							video_id = row[3];

							int count = 0;
							char position2[20] = "";

							//remove the spaces in the position string so that the only delimeter used in tokenizing it is comma (,)
							for(int k=0;k<position.length();k++){
								if(position[k] != ' '){
									position2[count] = tolower(position[k]);
									count++;
								}
							} 

							std::vector<std::string> arr; //we'll put all of the tokens in here
							std::string temp;

							posTemp = position2;
							while (posTemp.find(",", 0) != std::string::npos){ 
		  					  size_t  pos = posTemp.find(",", 0); 
							  temp = posTemp.substr(0, pos);    
							  posTemp.erase(0, pos + 1); 
							  //if the temp value is one of the directions
							  if(temp == "all" || temp == "nw" || temp == "cn" || temp == "ne" || temp == "cw" || temp == "c" || temp == "ce" || temp == "sw" || temp == "cs" || temp == "se" || temp == "n" || temp == "s" || temp == "e" || temp == "w"){
								arr.push_back(temp);
							  }
							  else{ 
								  is_pos = 1;
							  }
							}

							if(posTemp == "all" || posTemp == "nw" || posTemp == "cn" || posTemp == "ne" || posTemp == "cw" || posTemp == "c" || posTemp == "ce" || posTemp == "sw" || posTemp == "cs" || posTemp == "se" || posTemp == "n" || posTemp == "s" || posTemp == "e" || posTemp == "w"){
								arr.push_back(posTemp);   
							}
							else{
								is_pos = 1;
							}

							if(is_pos == 1){ //position for the video detected is invalid, will not detect video
								printf("Invalid position.\n");
								vn = video_name.c_str();
								strcpy(qstring, "UPDATE videos SET position = null where video_name = '");
								strcat(qstring, vn);
								strcat(qstring, "'");
								mysql_query(connect, qstring);
							}
							else{
								vidCount ++;
								printf("Detecting %d of %d video(s)...\n", vidCount, num_rows);

								std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";

								VideoCapture cap(path.append(video_name).append(".avi")); 
								if(!cap.isOpened()){
									cout << "Error opening video stream or file" << endl;
								}
								else{
									bool stop(false);
									fps = 30;

									int frame_width = 480;
									int frame_height = 272;
									std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
				
									VideoWriter video;
									if(std::stoi(detect_count) == 0){ 
										video.open(path.append(video_name.append(".avi")),CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true);
									}
									else{
										video.open(path.append(video_name.append("_").append(detect_count)).append(".avi"),CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true);
									}

									video_name = row[0];

									float frameCount = 0;
									float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

									CascadeClassifier face_cascade;
									face_cascade.load( "C:/OpenCV2410/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml" );

									//set the quadrant of each direction
									int fw_diff = frame_width/3;
									int fh_diff = frame_height/3;

									int fw_diff2 = frame_width/2;
									int fh_diff2 = frame_height/2;

									int nw_x1, cn_x1, ne_x1, cw_x1, c_x1, ce_x1, sw_x1, cs_x1, se_x1, n_x1, s_x1, e_x1, w_x1;
									int nw_x2, cn_x2, ne_x2, cw_x2, c_x2, ce_x2, sw_x2, cs_x2, se_x2, n_x2, s_x2, e_x2, w_x2;
									int nw_y1, cn_y1, ne_y1, cw_y1, c_y1, ce_y1, sw_y1, cs_y1, se_y1, n_y1, s_y1, e_y1, w_y1;
									int nw_y2, cn_y2, ne_y2, cw_y2, c_y2, ce_y2, sw_y2, cs_y2, se_y2, n_y2, s_y2, e_y2, w_y2;

									nw_x1 = cw_x1 = sw_x1 = 0;
									nw_x2 = cw_x2 = sw_x2 = fw_diff;

									cn_x1 = c_x1 = cs_x1 = nw_x2+1;
									cn_x2 = c_x2 = cs_x2 = (cn_x1 + fw_diff) - 1;

									ne_x1 = ce_x1 = se_x1 = cn_x2+1; 
									ne_x2 = ce_x2 = se_x2 = frame_width;

									nw_y1 = cn_y1 = ne_y1 = 0;
									nw_y2 = cn_y2 = ne_y2 = fh_diff;

									cw_y1 = c_y1 = ce_y1 = nw_y2 + 1;
									cw_y2 = c_y2 = ce_y2 = (cw_y1 + fh_diff) - 1;

									sw_y1 = cs_y1 = se_y1 = cw_y2 + 1;
									sw_y2 = cs_y2 = se_y2 = frame_height;

									n_y1 = 0;
									n_y2 = fh_diff2;

									s_y1 = n_y2;
									s_y2 = frame_height;

									s_x1 = n_x1 = 0;
									s_x2 = n_x2 = frame_width;

									w_x1 = 0;
									w_x2 = fw_diff2;

									e_x1 = w_x2;
									e_x2 = frame_width;

									w_y1 = e_y1 = 0;
									w_y2 = e_y2 = frame_height;

									int is_image = 0; //if there is a face detected in the video, include it in the videoclip
									is_frame = 0;
									hours = 0;
									mins = 0;
									secs = 0;
								
									while(frameCount<totalFrame){
										Mat image;

										cap >> image;

										cv::resize(image,image,cv::Size(480,272));

										frameCount++;
										fps_count++;

										if(fps_count == fps){
											secs++;
											if(secs == 60){
												mins++;
												secs = 0;
												if(mins == 60){
													hours++;
													mins = 0;
												}
											}
											fps_count = 0;
										}

										if((frameCount/totalFrame*100) <= 100){
											printf("\r%.2f%% ", (frameCount/totalFrame*100));
											cout << "of " << video_name << ".avi detected.";
										}

										std::vector<Rect> faces;
										face_cascade.detectMultiScale( image, faces, 1.2, 2, 0|CV_HAAR_SCALE_IMAGE, Size(20, 20),Size(200, 200) );

										for( int i = 0; i < faces.size(); i++ ){
											Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
											Point pt2(faces[i].x, faces[i].y);
										
											if(std::find(arr.begin(), arr.end(), "nw") != arr.end()){
												if(((pt1.x > nw_x1 && pt1.x < nw_x2) && (pt1.y > nw_y1 && pt1.y < nw_y2)) || ((pt2.x > nw_x1 && pt2.x < nw_x2) && (pt2.y > nw_y1 && pt2.y < nw_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "cn") != arr.end()){
												if(((pt1.x > cn_x1 && pt1.x < cn_x2) && (pt1.y > cn_y1 && pt1.y < cn_y2)) && ((pt2.x > cn_x1 && pt2.x < cn_x2) && (pt2.y > cn_y1 && pt2.y < cn_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
			  								if(std::find(arr.begin(), arr.end(), "ne") != arr.end()){
												if(((pt1.x > ne_x1 && pt1.x < ne_x2) && (pt1.y > ne_y1 && pt1.y < ne_y2)) || ((pt2.x > ne_x1 && pt2.x < ne_x2) && (pt2.y > ne_y1 && pt2.y < ne_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "cw") != arr.end()){
												if(((pt1.x > cw_x1 && pt1.x < cw_x2) && (pt1.y > cw_y1 && pt1.y < cw_y2)) || ((pt2.x > cw_x1 && pt2.x < cw_x2) && (pt2.y > cw_y1 && pt2.y < cw_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "c") != arr.end()){
												if(((pt1.x > c_x1 && pt1.x < c_x2) && (pt1.y > c_y1 && pt1.y < c_y2)) || ((pt2.x > c_x1 && pt2.x < c_x2) && (pt2.y > c_y1 && pt2.y < c_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "ce") != arr.end()){
												if(((pt1.x > ce_x1 && pt1.x < ce_x2) && (pt1.y > ce_y1 && pt1.y < ce_y2)) || ((pt2.x > ce_x1 && pt2.x < ce_x2) && (pt2.y > ce_y1 && pt2.y < ce_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "sw") != arr.end()){
												if(((pt1.x > sw_x1 && pt1.x < sw_x2) && (pt1.y > sw_y1 && pt1.y < sw_y2)) || ((pt2.x > sw_x1 && pt2.x < sw_x2) && (pt2.y > sw_y1 && pt2.y < sw_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "cs") != arr.end()){
												if(((pt1.x > cs_x1 && pt1.x < cs_x2) && (pt1.y > cs_y1 && pt1.y < cs_y2)) || ((pt2.x > cs_x1 && pt2.x < cs_x2) && (pt2.y > cs_y1 && pt2.y < cs_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "se") != arr.end()){
												if(((pt1.x > se_x1 && pt1.x < se_x2) && (pt1.y > se_y1 && pt1.y < se_y2)) || ((pt2.x > se_x1 && pt2.x < se_x2) && (pt2.y > se_y1 && pt2.y < se_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "n") != arr.end()){
												if(((pt1.x > n_x1 && pt1.x < n_x2) && (pt1.y > n_y1 && pt1.y < n_y2)) || ((pt2.x > n_x1 && pt2.x < n_x2) && (pt2.y > n_y1 && pt2.y < n_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "s") != arr.end()){
												if(((pt1.x > s_x1 && pt1.x < s_x2) && (pt1.y > s_y1 && pt1.y < s_y2)) || ((pt2.x > s_x1 && pt2.x < s_x2) && (pt2.y > s_y1 && pt2.y < s_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "e") != arr.end()){
												if(((pt1.x > e_x1 && pt1.x < e_x2) && (pt1.y > e_y1 && pt1.y < e_y2)) || ((pt2.x > e_x1 && pt2.x < e_x2) && (pt2.y > e_y1 && pt2.y < e_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "w") != arr.end()){
												if(((pt1.x > w_x1 && pt1.x < w_x2) && (pt1.y > w_y1 && pt1.y < w_y2)) || ((pt2.x > w_x1 && pt2.x < w_x2) && (pt2.y > w_y1 && pt2.y < w_y2))){
													rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
													is_image = 1;
													is_frame = 1;
												}
											}
											if(std::find(arr.begin(), arr.end(), "all") != arr.end()){
												rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
												is_image = 1;
												is_frame = 1;
											}
										}

										if(is_image == 1){ //include image in the videoclip
											hour = "0";
											min = "0";
											sec = "0";
											TestStr = "";

											if(hours == 0){
												hour = "00";
											}
											else if(hours < 10){
												hour.append(std::to_string(hours));
											}
											else{
												hour = std::to_string(hours);
											}

											if(mins == 0){
												min = "00";
											}
											else if(mins < 10){
												min.append(std::to_string(mins));
											}
											else{
												min = std::to_string(mins);
											}

											if(secs == 0){
												sec = "00";
											}
											else if(secs < 10){
												sec.append(std::to_string(secs));
											}
											else{
												sec = std::to_string(secs);	
											}
										
											TestStr.append(hour).append(":").append(min).append(":").append(sec);
											cv::rectangle(image, Point(370,240), Point(475,270), CV_RGB(0,0,0), CV_FILLED);
											putText(image, TestStr, Point(375,260), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(255,255,255),1,1); 
										
											//line(image,Point(nw_x2,nw_y1),Point(sw_x2,sw_y2),Scalar(0,0,0),2,8);
											//line(image,Point(cn_x2,cn_y1),Point(cs_x2,cs_y2),Scalar(0,0,0),2,8);
											//line(image,Point(nw_x1,nw_y2),Point(ne_x2,ne_y2),Scalar(0,0,0),2,8);
											//line(image,Point(cw_x1,cw_y2),Point(ce_x2,ce_y2),Scalar(0,0,0),2,8);

											//line(image,Point(n_x1,n_y2),Point(s_x2,s_y1),Scalar(0,0,0),2,8);
											//line(image,Point(e_x1,e_y1),Point(w_x2,w_y2),Scalar(0,0,0),2,8);

											video.write(image);
											frameCount++;
											is_image = 0;
										}
		
										if(kbhit()){ //check if ESC key is pressed
											key = getch();
											if(key == 27){
												is_cancel = 1;
												break;	 
											}
										}

									}

									if(is_cancel == 1){ //if ESC button is pressed
										mysql_query(connect, "UPDATE videos set position = null where position is not null");
										cout << "\nCancelled." << endl;
										break;
									}

									if(is_frame == 0){ //there is no frame with detected face
										if(std::stoi(detect_count) == 0){
											//no_face.push_back(video_name.append(".").append("avi"));
											video_name.append(".").append("avi");
										}
										else{
											//no_face.push_back(video_name.append("_").append(detect_count).append(".avi"));
											video_name.append("_").append(detect_count).append(".avi");
										}
										is_noface = 1;
										printf("\nNo face detected.");
									}
									else{
										video_name = row[0];

										cap.release();

										//If detection is complete, insert its data in the videoclips table
										strcpy(qstring, "INSERT INTO videoclips (videoclip_name, position) VALUES ('");
										if(std::stoi(detect_count) == 0){
											vn = video_name.c_str();
										}
										else{
											video_name.append("_").append(detect_count);
											vn = video_name.c_str();
										}
			
										strcat(qstring, vn);
										strcat(qstring, "', '");

										if(std::find(arr.begin(), arr.end(), "nw") != arr.end()){
											strcat(qstring, "NW ");
										}
										if(std::find(arr.begin(), arr.end(), "cn") != arr.end()){
											strcat(qstring, "CN ");
										}
										if(std::find(arr.begin(), arr.end(), "ne") != arr.end()){
											strcat(qstring, "NE ");
										}
										if(std::find(arr.begin(), arr.end(), "n") != arr.end()){
											strcat(qstring, "N ");
										}
										if(std::find(arr.begin(), arr.end(), "cw") != arr.end()){
											strcat(qstring, "CW ");
										}
										if(std::find(arr.begin(), arr.end(), "c") != arr.end()){
											strcat(qstring, "C ");
										}
										if(std::find(arr.begin(), arr.end(), "ce") != arr.end()){
											strcat(qstring, "CE ");
										}
										if(std::find(arr.begin(), arr.end(), "w") != arr.end()){
											strcat(qstring, "W ");
										}
										if(std::find(arr.begin(), arr.end(), "e") != arr.end()){
											strcat(qstring, "E ");
										}
										if(std::find(arr.begin(), arr.end(), "sw") != arr.end()){
											strcat(qstring, "SW ");
										}
										if(std::find(arr.begin(), arr.end(), "cs") != arr.end()){
											strcat(qstring, "CS ");
										}
										if(std::find(arr.begin(), arr.end(), "se") != arr.end()){
											strcat(qstring, "SE ");
										}
										if(std::find(arr.begin(), arr.end(), "s") != arr.end()){
											strcat(qstring, "S ");
										}
										if(std::find(arr.begin(), arr.end(), "all") != arr.end()){
											strcat(qstring, "ALL ");
										}
										strcat(qstring, "')");

										mysql_query(connect, qstring);
									
										cout << "\n" << video_name << ".avi created.";

										int vid_id = std::stoi(video_id);
										std::string vi = std::to_string(vid_id);
										const char *vid = vi.c_str();

										strcpy(qstring4, "UPDATE videoclips set video_id = ");
										strcat(qstring4, vid);
										strcat(qstring4, " where videoclip_name = '");
										strcat(qstring4, vn);
										strcat(qstring4, "'");

										mysql_query(connect, qstring4);

										//increment the detect count of the video detected
										int det_count = std::stoi(detect_count);
										det_count++;
										std::string s = std::to_string(det_count);
										const char *dc = s.c_str();

										video_name = row[0];
										vn = video_name.c_str();

										strcpy(qstring2, "UPDATE videos set detect_count = ");
										strcat(qstring2, dc);
										strcat(qstring2, " where video_name = '");
										strcat(qstring2, vn);
										strcat(qstring2, "'");

										mysql_query(connect, qstring2);

										//set position of detected video to null
										strcpy(qstring3, "UPDATE videos set position = null where video_name = '");
										strcat(qstring3, vn);
										strcat(qstring3, "'");
				
										mysql_query(connect, qstring3);
									
									}
								}
								printf("\n");
							}

							

						}	
						else{
							printf(mysql_error(connect));
							printf("\n");
						}

						if(is_noface == 1){ //delete the video/s in the directory who do not have a frame with detected face
								//for(int a=0;a<no_face.size();a++){
									std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";

									//path.append(no_face[a]);
									path.append(video_name);

									char path2[100] = "";

									for(int i=0;i<path.length();i++){
										path2[i] = path[i];
									}
									
									if(remove(path2)!= 0){
										perror("Error deleting file");
									}

									vn = video_name.c_str();

									strcpy(qstring, "");

									strcpy(qstring, "DELETE FROM videoclips where videoclip_name = '");
									strcat(qstring, vn);
									strcat(qstring, "'");
				
									mysql_query(connect, qstring);

									vn = row[0];

									strcpy(qstring2, "UPDATE videos set position = null where video_name = '");
									strcat(qstring2, vn);
									strcat(qstring2, "'");
									
									cout << qstring2;
									mysql_query(connect, qstring2);
								//}
							}
					}
							
							
							if(is_cancel == 1){ //delete the videoclip that is not completely detected
								std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";

								if(std::stoi(detect_count) == 0){
									path.append(video_name.append(".").append("avi"));
								}
								else{
									path.append(video_name.append("_").append(detect_count).append(".avi"));
								}
			
								char path2[100] = "";

								for(int i=0;i<path.length();i++){
									path2[i] = path[i];
								}

								if(remove(path2)!= 0){
									perror("Error deleting file");
								}
							}
				}
		}
	}
	else{
		printf("Invalid query.\n");
	}
}