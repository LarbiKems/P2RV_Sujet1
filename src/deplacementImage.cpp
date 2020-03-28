
#include "keyboard.h"
#include "deplacementImage.h"
using namespace cv;
Mat Puv = *(new Mat(3, 1, CV_32F));
Mat Pxyz = *(new Mat(3, 1, CV_32F));
Mat xyz1 = *(new Mat(4, 1, CV_32F));
Mat Pwz = *(new Mat(2, 1, CV_32F));
Mat Ry = *(new Mat(3, 3, CV_32F));
Mat Rx = *(new Mat(3, 3, CV_32F));
Mat Rz = *(new Mat(3, 3, CV_32F));
Mat Rot = *(new Mat(3, 3, CV_32F));
Mat headRTMatrix = *(new Mat(4, 4, CV_32F));
Mat FM = *(new Mat(3, 4, CV_32F));
Mat projMatrixInv = *(new Mat(3, 3, CV_32F));

void ChangementPointDeVue(cv::Mat img, cv::Mat projMatrix, Point3f headPosition, cv::Mat result)
{
	float focal_length = 0.5;
	// result.release();
	// result = Mat(img.rows, img.cols, CV_8UC3, Vec3b(0,0,0));
	for(int i=0; i<result.rows; i++) {
		for (int j=0; j<result.cols; j++) {
			result.at<Vec3b>(i,j) = Vec3b(0,0,0);
		}
	}
	// Calcul de la matrice de transformation de la caméra associée à la tête

	/*! Matrice de rotation */
	// Rotation autour de Y
	float normY = std::sqrt(std::pow(headPosition.x, 2) + std::pow(headPosition.z, 2));
	float cosY = headPosition.z / normY;
	float sinY = headPosition.x / normY;
	Ry = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosY, -sinY, 0, sinY, cosY);

	// Rotation autour de X
	float normX = std::sqrt(std::pow(headPosition.y, 2) + std::pow(headPosition.z, 2));
	float cosX = headPosition.z / normX;
	float sinX = headPosition.y / normX;
	Rx = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosX, -sinX, 0, sinX, cosX);

	// Rotation autour de Z --> pour l'instant on considère pas de rotation
	float cosZ = 1; // TODO or not TODO?;
	float sinZ = 0; //TODO or not TODO?;
	Rz = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosZ, -sinZ, 0, sinZ, cosZ);

	Rot = Rx * Ry * Rz;
	/*! Matrice de transformation */
	// [R | headPosition]
	headRTMatrix = Mat(4, 4, CV_32FC1, 0.0f); // à remplir (pas sur des dimensions)
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float a = Rot.at<float>(i, j);
			headRTMatrix.at<float>(i, j) = a;
		}
	}

	headRTMatrix.at<float>(0, 3) = headPosition.x;
	headRTMatrix.at<float>(1, 3) = headPosition.y;
	headRTMatrix.at<float>(2, 3) = headPosition.z;
	headRTMatrix.at<float>(3, 3) = 1;
	/*! Matrice avec la distance focale */
	FM = (Mat_<float>(3, 4) << focal_length, 0, 0, 0,
		  0, focal_length, 0, 0,
		  0, 0, 1, 0);

	// Mat headProjMatrix = Mat(3, 4, CV_32FC1);
	// headProjMatrix = projMatrix * headRTMatrix;
	projMatrixInv = projMatrix.inv(DECOMP_LU);
	// TODO: choisir le même repère pour la création de matrice de transformation et la détection de la tête

	for (int u = 0; u < img.rows; u++)
	{
		for (int v = 0; v < img.cols; v++)
		{
			//std::cout << "devut" << std::endl;
			// Calcul Point réel (x,y,z) dans le repère de la caméra.
			Puv = (Mat_<float>(3, 1) << u, v, 1);
			Pxyz = projMatrixInv * Puv;
			float s = Puv.at<float>(2, 0);

			float X = Pxyz.at<float>(0, 0) / focal_length;
			float Y = Pxyz.at<float>(1, 0) / focal_length;
			float Z = Pxyz.at<float>(2, 0);
			xyz1 = (Mat_<float>(4, 1) << X, Y, Z, 1);
			// Calcul de la position du point en question dans l'écran de la caméra associée à la tête
			Pwz = projMatrix * FM * headRTMatrix * xyz1;
			int w = (int)Pwz.at<float>(0);
			int z = (int)Pwz.at<float>(1);

			// Copie du pixel u,v correspondant dans la matrice de sortie à la place w, z
			if (w < result.rows && w > -1 && z > -1 && z < result.cols)
			{
				result.at<Vec3b>(w, z) = img.at<Vec3b>(u, v);
			}
		}
	}
}
