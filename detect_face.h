void detectFaces(MYSQL *connect, char query[]){
	std::string que = query;
	MYSQL_RES *res; 
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	int num_fields;
	int num_rows;
	int key;
	int is_cancel = 0;
	int vidCount = 0;
	char sql[100] = "";
	std::string video_name;
	std::string video_ext;
	std::string video_path;
	std::string position;
	std::string detect_count;
	std::string video_id;
	std::string posTemp;
	char position2[20] = "";
	char qstring[100];
	const char *vn;

	que.replace(0, 15, "update videos set");

	for(int i=0;i<que.length();i++){
		sql[i] = que[i];
	}

	if(mysql_query(connect, sql) == 0){
	
		mysql_query(connect, "SELECT video_name, video_ext, video_path, position, detect_count, video_id FROM videos WHERE position is not null");
		res = mysql_store_result(connect);

		num_fields = mysql_num_fields(res);
		num_rows = mysql_num_rows(res);

		for(int j=0;j<num_rows;j++){
			row = mysql_fetch_row(res);

			vidCount ++;
			printf("Detecting %d of %d video(s)...\n", vidCount, num_rows);

			video_name = row[0];
			video_ext = row[1];
			video_path = row[2];
			position = row[3];
			detect_count = row[4];
			video_id = row[5];

			int count = 0;

			for(int k=0;k<position.length();k++){
				if(position[k] != ' '){
					position2[count] = tolower(position[k]);
					count++;
				}
			}

			std::vector<std::string> arr; //we'll put all of the tokens in here 
			std::string  temp;

			posTemp = position2;

		    while (posTemp.find(",", 0) != std::string::npos)
		    { 
		  	  size_t  pos = posTemp.find(",", 0); 
			  temp = posTemp.substr(0, pos);    
			  posTemp.erase(0, pos + 1);        
			  arr.push_back(temp);             
		    }

		    arr.push_back(posTemp);          

			VideoCapture cap(video_path.append(video_name).append(".").append(video_ext)); 
			if(!cap.isOpened()){
				cout << "Error opening video stream or file" << endl;
			}
			else{
				bool stop(false);

				int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
				int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
				std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
				
				VideoWriter video;
				if(std::stoi(detect_count) == 0){
					video.open(path.append(video_name.append(".avi")),CV_FOURCC('M','J','P','G'),30, Size(480,272),true);
				}
				else{
					video.open(path.append(video_name.append("_").append(detect_count)).append(".avi"),CV_FOURCC('M','J','P','G'),30, Size(480,272),true);
				}

				video_name = row[0];

				float frameCount = 0;
				float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

				CascadeClassifier face_cascade;
				face_cascade.load( "C:/OpenCV2410/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml" );

				int fw_diff = frame_width/3;
				int fh_diff = frame_height/3;

				int fw_diff2 = frame_width/2;
				int fh_diff2 = frame_height/2;

				int nw_x1, nc_x1, ne_x1, wc_x1, c_x1, ec_x1, sw_x1, sc_x1, se_x1, n_x1, s_x1, e_x1, w_x1;
				int nw_x2, nc_x2, ne_x2, wc_x2, c_x2, ec_x2, sw_x2, sc_x2, se_x2, n_x2, s_x2, e_x2, w_x2;
				int nw_y1, nc_y1, ne_y1, wc_y1, c_y1, ec_y1, sw_y1, sc_y1, se_y1, n_y1, s_y1, e_y1, w_y1;
				int nw_y2, nc_y2, ne_y2, wc_y2, c_y2, ec_y2, sw_y2, sc_y2, se_y2, n_y2, s_y2, e_y2, w_y2;

				nw_x1 = wc_x1 = sw_x1 = 0;
				nw_x2 = wc_x2 = sw_x2 = fw_diff;

				nc_x1 = c_x1 = sc_x1 = nw_x2+1;
				nc_x2 = c_x2 = sc_x2 = (nc_x1 + fw_diff) - 1;

				ne_x1 = ec_x1 = se_x1 = nc_x2+1; 
				ne_x2 = ec_x2 = se_x2 = frame_width;

				nw_y1 = nc_y1 = ne_y1 = 0;
				nw_y2 = nc_y2 = ne_y2 = fh_diff;

				wc_y1 = c_y1 = ec_y1 = nw_y2 + 1;
				wc_y2 = c_y2 = ec_y2 = (wc_y1 + fh_diff) - 1;

				sw_y1 = sc_y1 = se_y1 = wc_y2 + 1;
				sw_y2 = sc_y2 = se_y2 = frame_height;

				n_y1 = 0;
				n_y2 = fh_diff2;

				s_y1 = n_y2 + 1;
				s_y2 = frame_height;

				s_x1 = n_x1 = 0;
				s_x2 = n_x2 = frame_width;

				w_x1 = 0;
				w_x2 = fw_diff2;

				e_x1 = w_x2 + 1;
				e_x2 = frame_width;

				w_y1 = e_y1 = 0;
				w_y2 = e_y2 = frame_height;

				int is_image = 0;
				while(frameCount<totalFrame)
				{
					Mat image;

					cap >> image;

					cv::resize(image,image,cv::Size(480,272));
					
					frameCount++;

					if((frameCount/totalFrame*100) <= 100){
						printf("\r%.2f%% ", (frameCount/totalFrame*100));
						cout << "of " << video_name << "." << video_ext << " detected.";
					}

					std::vector<Rect> faces;
					face_cascade.detectMultiScale( image, faces, 1.1, 6, CV_HAAR_DO_CANNY_PRUNING, Size(30, 30) );
		
					for( int i = 0; i < faces.size(); i++ )
					{
						Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
						Point pt2(faces[i].x, faces[i].y);
			
						if(std::find(arr.begin(), arr.end(), "nw") != arr.end()){
							if(((pt1.x > nw_x1 && pt1.x < nw_x2) && (pt1.y > nw_y1 && pt1.y < nw_y2)) || ((pt2.x > nw_x1 && pt2.x < nw_x2) && (pt2.y > nw_y1 && pt2.y < nw_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					   if(std::find(arr.begin(), arr.end(), "nc") != arr.end()){
							if(((pt1.x > n_x1 && pt1.x < n_x2) && (pt1.y > n_y1 && pt1.y < n_y2)) && ((pt2.x > n_x1 && pt2.x < n_x2) && (pt2.y > n_y1 && pt2.y < n_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
			  		    if(std::find(arr.begin(), arr.end(), "ne") != arr.end()){
							if(((pt1.x > ne_x1 && pt1.x < ne_x2) && (pt1.y > ne_y1 && pt1.y < ne_y2)) || ((pt2.x > ne_x1 && pt2.x < ne_x2) && (pt2.y > ne_y1 && pt2.y < ne_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					    if(std::find(arr.begin(), arr.end(), "wc") != arr.end()){
							if(((pt1.x > w_x1 && pt1.x < w_x2) && (pt1.y > w_y1 && pt1.y < w_y2)) || ((pt2.x > w_x1 && pt2.x < w_x2) && (pt2.y > w_y1 && pt2.y < w_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					    if(std::find(arr.begin(), arr.end(), "c") != arr.end()){
							if(((pt1.x > c_x1 && pt1.x < c_x2) && (pt1.y > c_y1 && pt1.y < c_y2)) || ((pt2.x > c_x1 && pt2.x < c_x2) && (pt2.y > c_y1 && pt2.y < c_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					    if(std::find(arr.begin(), arr.end(), "ec") != arr.end()){
							if(((pt1.x > e_x1 && pt1.x < e_x2) && (pt1.y > e_y1 && pt1.y < e_y2)) || ((pt2.x > e_x1 && pt2.x < e_x2) && (pt2.y > e_y1 && pt2.y < e_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					    if(std::find(arr.begin(), arr.end(), "sw") != arr.end()){
							if(((pt1.x > sw_x1 && pt1.x < sw_x2) && (pt1.y > sw_y1 && pt1.y < sw_y2)) || ((pt2.x > sw_x1 && pt2.x < sw_x2) && (pt2.y > sw_y1 && pt2.y < sw_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					    if(std::find(arr.begin(), arr.end(), "sc") != arr.end()){
							if(((pt1.x > s_x1 && pt1.x < s_x2) && (pt1.y > s_y1 && pt1.y < s_y2)) || ((pt2.x > s_x1 && pt2.x < s_x2) && (pt2.y > s_y1 && pt2.y < s_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
					    if(std::find(arr.begin(), arr.end(), "se") != arr.end()){
							if(((pt1.x > se_x1 && pt1.x < se_x2) && (pt1.y > se_y1 && pt1.y < se_y2)) || ((pt2.x > se_x1 && pt2.x < se_x2) && (pt2.y > se_y1 && pt2.y < se_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
						if(std::find(arr.begin(), arr.end(), "n") != arr.end()){
							if(((pt1.x > n_x1 && pt1.x < n_x2) && (pt1.y > n_y1 && pt1.y < n_y2)) || ((pt2.x > n_x1 && pt2.x < n_x2) && (pt2.y > n_y1 && pt2.y < n_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
						if(std::find(arr.begin(), arr.end(), "s") != arr.end()){
							if(((pt1.x > s_x1 && pt1.x < s_x2) && (pt1.y > s_y1 && pt1.y < s_y2)) || ((pt2.x > s_x1 && pt2.x < s_x2) && (pt2.y > s_y1 && pt2.y < s_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
						if(std::find(arr.begin(), arr.end(), "e") != arr.end()){
							if(((pt1.x > e_x1 && pt1.x < e_x2) && (pt1.y > e_y1 && pt1.y < e_y2)) || ((pt2.x > e_x1 && pt2.x < e_x2) && (pt2.y > e_y1 && pt2.y < e_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
						if(std::find(arr.begin(), arr.end(), "w") != arr.end()){
							if(((pt1.x > w_x1 && pt1.x < w_x2) && (pt1.y > w_y1 && pt1.y < w_y2)) || ((pt2.x > w_x1 && pt2.x < w_x2) && (pt2.y > w_y1 && pt2.y < w_y2))){
								rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
								is_image = 1;
							}
						}
						if(std::find(arr.begin(), arr.end(), "all") != arr.end()){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}

						if(position != "nw" || position != "nc" || position != "ne" || position != "wc" || position != "c" ||  position != "ec" || position != "sw" || position != "sc" || position != "se" || position != "n" || position != "w" || position != "e" || position != "s"){
							printf("%s is an invalid position.\n", position);
						}
					}

					if(is_image == 1){
						video.write(image);
						is_image = 0;
					}
		
					if(kbhit()){
						key = getch();
						if(key == 27){
							is_cancel = 1;
							break;	 
						}
					}
				}

				cap.release();

				strcpy(qstring, "INSERT INTO videoclips (videoclip_name, video_id, position) VALUES ('");
				if(std::stoi(detect_count) == 0){
					vn = video_name.c_str();
				}
				else{
					vn = video_name.append("_").append(detect_count).c_str();
				}
			
				strcat(qstring, vn);
				strcat(qstring, "', ");

				const char *video_id2 = video_id.c_str();;
				
				strcat(qstring, video_id2);
				strcat(qstring, ", '");
				if(std::find(arr.begin(), arr.end(), "nw") != arr.end()){
					strcat(qstring, "NW ");
				}
				if(std::find(arr.begin(), arr.end(), "nc") != arr.end()){
					strcat(qstring, "NC ");
				}
				if(std::find(arr.begin(), arr.end(), "ne") != arr.end()){
					strcat(qstring, "NE ");
				}
				if(std::find(arr.begin(), arr.end(), "n") != arr.end()){
					strcat(qstring, "N ");
				}
				if(std::find(arr.begin(), arr.end(), "wc") != arr.end()){
					strcat(qstring, "WC ");
				}
				if(std::find(arr.begin(), arr.end(), "c") != arr.end()){
					strcat(qstring, "C ");
				}
				if(std::find(arr.begin(), arr.end(), "ec") != arr.end()){
					strcat(qstring, "EC ");
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
				if(std::find(arr.begin(), arr.end(), "sc") != arr.end()){
					strcat(qstring, "SC ");
				}
				if(std::find(arr.begin(), arr.end(), "se") != arr.end()){
					strcat(qstring, "SE ");
				}
				if(std::find(arr.begin(), arr.end(), "s") != arr.end()){
					strcat(qstring, "S ");
				}
				strcat(qstring, "')");

				mysql_query(connect, qstring);

				int det_count = std::stoi(detect_count);
				det_count++;

				strcpy(qstring, "UPDATE videos set detect_count = ");
				strcat(qstring, (char *) det_count);
				strcat(qstring, "where video_name = '");
				strcat(qstring, vn);
				strcat(qstring, "')");

				mysql_query(connect, qstring);

				strcpy(qstring, "UPDATE videos set position = NULL where video_name = '");
				strcat(qstring, vn);
				strcat(qstring, "')");
				
				printf("%s", qstring);

				mysql_query(connect, qstring);
			}
		}

		if(is_cancel == 1){
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

			printf("%s", path2);
			if(remove(path2)!= 0){
				perror("Error deleting file");
			}
		}

	}	
	else{
		printf(mysql_error(connect));
		printf("\n");
	}
}