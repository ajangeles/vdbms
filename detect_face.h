void detectFaces(){
	/*VideoCapture cap("C:\\Users\\Kevin\\Videos\\2.mp4"); 
	if(!cap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	bool stop(false);

	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("D:/out.avi",CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height),true);

	CascadeClassifier face_cascade;
	face_cascade.load( "C:/OpenCV2410/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml" );

	while(!stop)
	{
		Mat image;
		cap >> image;

		std::vector<Rect> faces;
		face_cascade.detectMultiScale( image, faces, 1.1, 6, CV_HAAR_DO_CANNY_PRUNING, Size(30, 30) );

		for( int i = 0; i < faces.size(); i++ )
		{
			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
			Point pt2(faces[i].x, faces[i].y);
 
			rectangle(image, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
		}

		video.write(image);
		imshow( "Face Detection", image );

		if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}

	cap.release();*/
	printf("DETECT!");
}