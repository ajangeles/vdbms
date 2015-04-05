void detectFaces(MYSQL *connect, char query[]){
	std::string que = query;
	MYSQL_RES *res; 
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	int num_fields;
	char sql[100] = "";
	std::string video_name;
	std::string video_ext;
	std::string video_path;
	std::string position;

	que.replace(0, 16, "update");

	for(int i=0;i<que.length();i++){
		sql[i] = que[i];
	}

	if(mysql_query(connect, sql) == 0){
	
		mysql_query(connect, "SELECT video_name, video_ext, video_path, position FROM videos WHERE position is not null");
		res = mysql_store_result(connect);
		num_fields = mysql_num_fields(res);
		row = mysql_fetch_row(res);
		video_name = row[0];
		video_ext = row[1];
		video_path = row[2];
		position = row[3];
		if(res != NULL)
		   mysql_free_result(res);

		VideoCapture cap(video_path.append(video_name).append(".").append(video_ext)); 
		if(!cap.isOpened()){
			cout << "Error opening video stream or file" << endl;
		}
		else{
			bool stop(false);

			int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
			int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
			std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
			VideoWriter video(path.append(video_name.append("_").append("a")).append(".avi"),CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true);

			float frameCount = 0;
			float totalFrame = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

			CascadeClassifier face_cascade;
			face_cascade.load( "C:/OpenCV2410/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml" );

			int fw_diff = frame_width/3;
			int fh_diff = frame_height/3;

			int nw_x1, n_x1, ne_x1, w_x1, c_x1, e_x1, sw_x1, s_x1, se_x1;
			int nw_x2, n_x2, ne_x2, w_x2, c_x2, e_x2, sw_x2, s_x2, se_x2;
			int nw_y1, n_y1, ne_y1, w_y1, c_y1, e_y1, sw_y1, s_y1, se_y1;
			int nw_y2, n_y2, ne_y2, w_y2, c_y2, e_y2, sw_y2, s_y2, se_y2;

			nw_x1 = w_x1 = sw_x1 = 0;
			nw_x2 = w_x2 = sw_x2 = fw_diff;

			n_x1 = c_x1 = s_x1 = nw_x2+1;
			n_x2 = c_x2 = s_x2 = (n_x1 + fw_diff) - 1;

			ne_x1 = e_x1 = se_x1 = n_x2+1; 
			ne_x2 = e_x2 = se_x2 = frame_width;

			nw_y1 = n_y1 = ne_y1 = 0;
			nw_y2 = n_y2 = ne_y2 = fh_diff;

			w_y1 = c_y1 = e_y1 = nw_y2 + 1;
			w_y2 = c_y2 = e_y2 = (w_y1 + fh_diff) - 1;

			sw_y1 = s_y1 = se_y1 = w_y2 + 1;
			sw_y2 = s_y2 = se_y2 = frame_height;

			int is_image = 0;
			while(!stop)
			{
				Mat image;
				cap >> image;
				frameCount++;

				std::cout << std::setprecision(2);
				std::cout << (frameCount/totalFrame*100) << '%';
				std::cout.flush(); // see wintermute's comment
				std::cout << '\r';

				std::vector<Rect> faces;
				face_cascade.detectMultiScale( image, faces, 1.1, 6, CV_HAAR_DO_CANNY_PRUNING, Size(30, 30) );
		
				for( int i = 0; i < faces.size(); i++ )
				{
					Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
					Point pt2(faces[i].x, faces[i].y);
			
					if(position == "northwest"){
						if(((pt1.x > nw_x1 && pt1.x < nw_x2) && (pt1.y > nw_y1 && pt1.y < nw_y2)) || ((pt2.x > nw_x1 && pt2.x < nw_x2) && (pt2.y > nw_y1 && pt2.y < nw_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "north"){
						printf("n");
						if(((pt1.x > n_x1 && pt1.x < n_x2) && (pt1.y > n_y1 && pt1.y < n_y2)) && ((pt2.x > n_x1 && pt2.x < n_x2) && (pt2.y > n_y1 && pt2.y < n_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "northeast"){
						if(((pt1.x > ne_x1 && pt1.x < ne_x2) && (pt1.y > ne_y1 && pt1.y < ne_y2)) || ((pt2.x > ne_x1 && pt2.x < ne_x2) && (pt2.y > ne_y1 && pt2.y < ne_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "west"){
						if(((pt1.x > w_x1 && pt1.x < w_x2) && (pt1.y > w_y1 && pt1.y < w_y2)) || ((pt2.x > w_x1 && pt2.x < w_x2) && (pt2.y > w_y1 && pt2.y < w_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "center"){
						if(((pt1.x > c_x1 && pt1.x < c_x2) && (pt1.y > c_y1 && pt1.y < c_y2)) || ((pt2.x > c_x1 && pt2.x < c_x2) && (pt2.y > c_y1 && pt2.y < c_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "east"){
						if(((pt1.x > e_x1 && pt1.x < e_x2) && (pt1.y > e_y1 && pt1.y < e_y2)) || ((pt2.x > e_x1 && pt2.x < e_x2) && (pt2.y > e_y1 && pt2.y < e_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "southwest"){
						if(((pt1.x > sw_x1 && pt1.x < sw_x2) && (pt1.y > sw_y1 && pt1.y < sw_y2)) || ((pt2.x > sw_x1 && pt2.x < sw_x2) && (pt2.y > sw_y1 && pt2.y < sw_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "south"){
						if(((pt1.x > s_x1 && pt1.x < s_x2) && (pt1.y > s_y1 && pt1.y < s_y2)) || ((pt2.x > s_x1 && pt2.x < s_x2) && (pt2.y > s_y1 && pt2.y < s_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else if(position == "southeast"){
						if(((pt1.x > se_x1 && pt1.x < se_x2) && (pt1.y > se_y1 && pt1.y < se_y2)) || ((pt2.x > se_x1 && pt2.x < se_x2) && (pt2.y > se_y1 && pt2.y < se_y2))){
							rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
							is_image = 1;
						}
					}
					else{
						printf("%s is an invalid position.\n", position);
					}

			
				}

				if(is_image == 1){
					video.write(image);
					is_image = 0;
				}
		
				if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
					cout << "esc key is pressed by user" << endl;
					break; 
				}
			}

			cap.release();
		}


	}
	else{
		printf(mysql_error(connect));
		printf("\n");
	}

}