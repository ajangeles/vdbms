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