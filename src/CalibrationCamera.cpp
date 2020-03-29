// OpenCVCalibration.cpp
#include "CalibrationCamera.h"
#include "keyboard.h"
using namespace cv;
using namespace std;

cv::Mat calibrateMainCamera(VideoCapture cap, int nb_cam)
{

	/*! Si il n'y a pas de caméra principale, on fournit une matrice intrinsèque par défaut */
	if (nb_cam == 1) {
		int fx = 1;
		int fy = 1;
		int cx = 200;
		int cy = 200;
		return (Mat_<float>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
	}

	int key = 0;
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

	bool couleurGris = false;

	while (compteurImageCalibration < nombreImageCalibration) {
		// Getting the new image from the camera
		cap >> image;

		// Showing the image in the window
		key = waitKey(1);

		//D�tection d'un �chiquier
		cv::Size board_sz = cv::Size(numCornersHor, numCornersVer);
		vector<cv::Point2f> corners;
		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		//Calibration d'une cam�ra
		if (found) {
			compteurImageCalibration++; //si c'est trouv�, alors on a une image en plus pour la calibration
			cvtColor(image, gray_image, CV_BGR2GRAY);
			cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(image, board_sz, corners, found);
			image_points.push_back(corners);
			vector<cv::Point3f> ligne;
			for (int i = 0; i < numCornersHor; i++) {
				for (int j = 0; j < numCornersVer; j++) {
					Point3f point(i, j, 0);
					ligne.push_back(point);
				}
			}
			object_points.push_back(ligne);
		}

		//Affichage
		if (key == 'g') { couleurGris = !couleurGris; }
		if (!couleurGris) {
			imshow("OpenCV Calibration", image);
		}
		else {
			cvtColor(image, gray_image, CV_BGR2GRAY);
			imshow("OpenCV Calibration", gray_image);
		}

		if (key == KEY_ESCAPE) { break; }
	}
	calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
	// Destroying the windows
	destroyWindow("OpenCV Calibration");
	
	return intrinsic;
}