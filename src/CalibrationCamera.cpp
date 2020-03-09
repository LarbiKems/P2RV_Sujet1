// OpenCVCalibration.cpp

cv::Mat calibrateCamera(VideoCapture cap)
{
	
	int numeroCamera = 0; 
	int numCornersHor = 9;
	int numCornersVer = 6;
	int nombreImageCalibration = 1;
	int compteurImageCalibration = 0;
	Mat image; // The current image
	Mat gray_image; // The image converted into grayscale (see if we use it)
	namedWindow("OpenCV Calibration", CV_WINDOW_AUTOSIZE); 	// Creating a window to display the images


	cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
	cv::Mat distCoeffs;
	vector<cv::Mat> rvecs;
	vector<cv::Mat> tvecs;
	vector<vector<cv::Point3f>> object_points;
	vector<vector<cv::Point2f>> image_points;
	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;

	//Images caméra en direct

	//Tant qu'une caméra n'est pas ouverte, on harcèle l'utilisateur
	while (!cap.isOpened())
	{
		cout << "Quel numero de camera voulez-vous ?" << endl;
		cin >> numeroCamera;
		cout << "Quel nombre de coins horizontaux a l'interieur de l'echiquier voulez-vous ?" << endl;
		cin >> numCornersHor;
		cout << "Quel nombre de coins verticaux a l'interieur de l'echiquier voulez-vous ?" << endl;
		cin >> numCornersVer;
		cout << "Quel nombre d'image pour la calibration voulez-vous ?" << endl;
		cin >> nombreImageCalibration;
		if (numeroCamera==-1){break;}
		cap.open(numeroCamera);
	}

	bool couleurGris = false;

	while (compteurImageCalibration<nombreImageCalibration){
		// Getting the new image from the camera
		cap >> image;

		// Showing the image in the window
		key=waitKey(1);

		//Détection d'un échiquier
		cv::Size board_sz = cv::Size(numCornersHor, numCornersVer);
		vector<cv::Point2f> corners;
		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
		//Calibration d'une caméra
		if (found) {
			compteurImageCalibration++; //si c'est trouvé, alors on a une image en plus pour la calibration
			cvtColor(image,gray_image,CV_BGR2GRAY);
			cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(image, board_sz, corners, found);
			image_points.push_back(corners);
			vector<cv::Point3f> ligne;
			for (int i=0;i<numCornersHor;i++){
				for (int j=0;j<numCornersVer;j++){
					Point3f point(i,j,0);
					ligne.push_back(point);
				}
			}
			object_points.push_back(ligne);
		}

		//Affichage
		if (key=='g'){couleurGris = !couleurGris;}
		if (!couleurGris){
			imshow("OpenCV Calibration",image);
		}
		else{
			cvtColor(image,gray_image,CV_BGR2GRAY);
			imshow("OpenCV Calibration",gray_image);
		}

		if (key==ESC_KEY){break;}
	}

	calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
	// Destroying the windows
	destroyWindow("OpenCV Calibration");
	
	return intrinsic;
}