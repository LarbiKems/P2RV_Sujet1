#include "faceDetection.hpp"
#include <cmath>
#include "keyboard.h"

using namespace std;

// /!\ le using namespace cv ne doit pas être mis dans un .h, car ça ne fonctionne pas sur Windows...
using namespace cv;

vector<Rect> *faces = new vector<Rect>();
// Structure qui regroupe toutes les variables nécessaire à la détection de la tête
struct
{
	// True if the xml file was already loaded
	bool xml_files_loaded;// = false;

	// Classifier used to detect the eyes on the picture
	CascadeClassifier cascade;

	// Distance between the two eyes, in cm
	float dist_btw_eyes_cm;// = 6.0;

	// Coefficient to compute the depth of the head detected
	float depth_coeff;// = 50 / (6.0 / 80);

	// Distance chosen to calibrate in cm
	float calibrationDistance;// = 50.0;

	// True if the headDetector was calibrated
	bool calibrated;// = false;
} headDetector;

void init_faceDetection() {
	headDetector.xml_files_loaded = false;
	headDetector.dist_btw_eyes_cm = 6.0;
	headDetector.depth_coeff = 50 / (6.0/80);
	headDetector.calibrationDistance = 50.0;
	headDetector.calibrated = false;
}
void setEyeDistance(float dist)
{
	headDetector.dist_btw_eyes_cm = dist;
}

void calibrateFaceCamera(VideoCapture faceCamera) {
	Mat currentFaceImage;
	bool headDetectorCalibrated = false; //! True si la profondeur a été calibrée

	

	/*! Première étape: calibration de la caméra "face" */
	float dist_btw_eyes;
	cout << "Distance entre les deux yeux (cm): " << endl;
	cin >> dist_btw_eyes;
	setEyeDistance(dist_btw_eyes);
	cout << "Placez-vous à 50cm de la caméra, au centre de l'image, et appuyez sur c" << endl;

	string imageName = "Calibration de la camera frontale";
	namedWindow(imageName, CV_WINDOW_AUTOSIZE);

	bool calibrating = false;
	char key = 'a';
	//! Boucle de calibration
	while ((key != KEY_ESCAPE) && (key != ENTER_KEY))
	{
		faceCamera >> currentFaceImage;

		// Effet mirroir
		flip(currentFaceImage, currentFaceImage, 1);
		Point3f temp;
		// Affiche de l'image courante et dessin des yeux si ils sont détectés

		if ((key == 'c') || calibrating)
		{
			if (key != 'e')
			{
				headDetectorCalibrated = calibrateDepth(currentFaceImage);

				if (headDetectorCalibrated)
				{
					cout << "Calibration effectuée. \n\t- Appuyer sur Entrée pour confirmer.\n\t- Appuyer sur c pour refaire la calibration" << endl;
					calibrating = false;
				}
				else
				{
					if (key == 'c')
					{
						cout << "Calibration (appuyer sur e pour annuler)..." << endl;
						calibrating = true;
					}
				}
			}
			else
			{
				cout << "Calibration annulée. Repositionnez-vous et appuyez sur c" << endl;
				bool detected = detectEyes(currentFaceImage, &temp, 1.0, true);
				calibrating = false;
			}
		}
		else
		{
			bool detected=detectEyes(currentFaceImage, &temp, 1.0, true);
		}
		imshow(imageName, currentFaceImage);

		key = waitKey(1);
	}

	if (!headDetectorCalibrated)
	{
		cout << "Calibration non effectuée. Les paramètres par défaut seront utilisés." << endl;
	}
	else
	{
		cout << "Calibration validée" << endl;
	}

	destroyWindow(imageName);
}

bool calibrateDepth(Mat &img)
{
	Point3f temp;
	return detectEyes(img, &temp, 1, true, false, true);
}

bool detectEyes(Mat &img, Point3f *relative_head_position, double scale, bool draw_eyes, bool draw_arrow, bool calibrate)
{

	faces->clear();
	// Load xml files if not loaded
	if (!headDetector.xml_files_loaded)
	{
		headDetector.xml_files_loaded = headDetector.cascade.load("resources\\haarcascade_eye.xml");
	}
	if (!headDetector.xml_files_loaded)
	{
		cout << "haarcascade_eye.xml not found" << endl;
		return false;
	}

	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale
	double fx = 1 / scale;

	// Resize the Grayscale Image
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);

	Mat temp_img = smallImg.clone();
	equalizeHist(smallImg, temp_img);

	// Detect faces of different sizes using cascade classifier
	headDetector.cascade.detectMultiScale(smallImg, *faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30), Size(60, 60));

	// Colors used for Drawing tool
	Scalar blue = Scalar(255, 0, 0);
	Scalar red = Scalar(0, 255, 0);

	// If 2 eyes were found
	if (faces->size() == 2)
	{

		Point p1, p2;
		int radius1, radius2;

		// Premier oeil
		Rect r = faces->at(0);
		p1.x = cvRound((r.x + r.width * 0.5) * scale);
		p1.y = cvRound((r.y + r.height * 0.5) * scale);
		radius1 = cvRound((r.width + r.height) * 0.25 * scale);

		// Second oeil
		r = faces->at(1);
		p2.x = cvRound((r.x + r.width * 0.5) * scale);
		p2.y = cvRound((r.y + r.height * 0.5) * scale);
		radius2 = cvRound((r.width + r.height) * 0.25 * scale);

		// Point entre les deux yeux
		Point center(0.5 * (p1.x + p2.x), 0.5 * (p1.y + p2.y));

		// Point au centre de l'image
		Point img_center(smallImg.cols / 2, smallImg.rows / 2);

		// Position relative en pixels,s en 2D (dans le plan xy)
		Point relative_pos = center - img_center;
		relative_pos.y *= -1;

		// Distance entre les deux yeux en pixels
		float dist_btw_eyes_px = norm(p1 - p2);

		// Mise en valeur des informations collectées sur l'image si besoin
		if (draw_arrow)
		{
			arrowedLine(img, img_center, center, red, 4);
		}
		if (draw_eyes)
		{
			circle(img, p1, radius1, blue, 3, 8, 0);
			circle(img, p2, radius2, blue, 3, 8, 0);
		}

		// Si la calibration de la profondeur est demandée
		if (calibrate)
		{
			headDetector.depth_coeff = headDetector.calibrationDistance / (headDetector.dist_btw_eyes_cm / dist_btw_eyes_px);
			headDetector.calibrated = true;
		}

		if (headDetector.calibrated)
		{
			/* Si le headDetector est calibré, on peut calculer la position de la tête */

			// Calcul du coefficient pour passer de pixels en cm dans le plan xy
			float coeff = headDetector.dist_btw_eyes_cm / dist_btw_eyes_px;

			relative_head_position->x = coeff * relative_pos.x;
			relative_head_position->y = coeff * relative_pos.y;
			relative_head_position->z = coeff * headDetector.depth_coeff;

			// La position de la tête a été trouvée, on renvoie true
			return true;
		}
	}

	return false;
}
