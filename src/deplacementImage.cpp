

#include "deplacementImage.h"
using namespace cv;
#ifdef BENJI_ET_SES_FONCTIONS_A_LA_CON
//! Touche appuy�e pour le d�bogage. D�place l'image
void appuiTouche(int &decalagePixelVertical, int &decalagePixelHorizontal)
{

	if (GetKeyState(VK_UP) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
		decalagePixelVertical++;
	if (GetKeyState(VK_DOWN) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
		decalagePixelVertical--;
	if (GetKeyState(VK_LEFT) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
		decalagePixelHorizontal++;
	if (GetKeyState(VK_RIGHT) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
		decalagePixelHorizontal--;
}
#endif

//! Calcul du decalage en pixel par rapport a la position de la tete (#Thal�s)
void calculDecalageFenetre(cv::Point3f positionTete, int &decalagePixelVertical, int &decalagePixelHorizontal, float distanceCameraPaysage, float pixelParMetre)
{
	decalagePixelHorizontal = 100000 * pixelParMetre * distanceCameraPaysage * positionTete.x / positionTete.y;
	decalagePixelVertical = 100000 * pixelParMetre * distanceCameraPaysage * positionTete.z / positionTete.y;
	std::cout << "decalagePixelHorizontal " << 100000 * pixelParMetre * distanceCameraPaysage * positionTete.x / positionTete.y << std::endl;
}

void ChangementPointDeVue(cv::Mat img, cv::Mat projMatrix, Point3f headPosition, cv::Mat result)
{
	float focal_length = 0.5;
	result.release();
	result = Mat(Size(img.cols, img.rows), CV_8UC3, Scalar(0, 0, 0));

	// Calcul de la matrice de transformation de la caméra associée à la tête

	/*! Matrice de rotation */
	// Rotation autour de Y
	float normY = std::sqrt(std::pow(headPosition.x, 2) + std::pow(headPosition.z, 2));
	float cosY = headPosition.z / normY; 
	float sinY = headPosition.x / normY; 
	Mat Ry = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosY, -sinY, 0, sinY, cosY);

	// Rotation autour de X
	float normX = std::sqrt(std::pow(headPosition.y, 2) + std::pow(headPosition.z, 2));
	float cosX = headPosition.z / normX; 
	float sinX = headPosition.y / normX; 
	Mat Rx = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosX, -sinX, 0, sinX, cosX);

	// Rotation autour de Z --> pour l'instant on considère pas de rotation
	float cosZ = 1; // TODO or not TODO?;
	float sinZ = 0; //TODO or not TODO?;
	Mat Rz = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosZ, -sinZ, 0, sinZ, cosZ);

	Mat Rot = Rx * Ry * Rz;

	/*! Matrice de transformation */
	// [R | headPosition]
	Mat headRTMatrix = Mat(3,4, CV_32FC1, 0.0f); // à remplir (pas sur des dimensions)
	for(int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			float a = Rot.at<float>(i,j) ;
			headRTMatrix.at<float>(i,j) = a;
		}
	}
	headRTMatrix.at<float>(0,3) = headPosition.x;
	headRTMatrix.at<float>(1,3) = headPosition.y;
	headRTMatrix.at<float>(2,3) = headPosition.z;

	/*! Matrice avec la distance focale */
	Mat FM = (Mat_<float>(3, 4) << focal_length, 0, 0, 0, 
		0, focal_length, 0, 0, 
		0,0,1,0);

	Mat headProjMatrix = Mat(3,4, CV_32FC1);
	headProjMatrix = projMatrix * headRTMatrix;
	Mat projMatrixInv = projMatrix.inv(DECOMP_LU);
	// TODO: choisir le même repère pour la création de matrice de transformation et la détection de la tête
	for (int u = 0; u < img.rows; u++)
	{
		for (int v = 0; v < img.cols; v++)
		{
			// Calcul Point réel (x,y,z) dans le repère de la caméra.
			Mat Puv = (Mat_<float>(3, 1) << u, v,1);

			Mat Pxyz = projMatrixInv * Puv;
			float s = Puv.at<float>(2,0);

			float X= Pxyz.at<float>(0,0)/focal_length;
			float Y= Pxyz.at<float>(1,0)/focal_length;
			float Z= Pxyz.at<float>(2,0);

			Mat xyz1 = (Mat_<float>(4, 1) << X, Y,Z,1);
			// Calcul de la position du point en question dans l'écran de la caméra associée à la tête
			Mat Pwz = projMatrix*FM*headRTMatrix * xyz1;
			int w = (int)Pwz.at<float>(0);
			int z = (int)Pwz.at<float>(1);
			
			// Copie du pixel u,v correspondant dans la matrice de sortie à la place w, z
			if (w < result.rows && z < result.cols)
			{
				result.at<Scalar>(w, z) = img.at<Scalar>(u, v);
			}
		}
	}
}

//! cr�e les bon range pour l'image finale. Renvoie false si le d�coupage fait sortir de l'image obtenu par la cam�ra.
//Pour l'avenir : peut �tre impl�menter plut�t un affichage de pixels noirs quand on d�passe de l'image
bool decoupageImage(const cv::Range decoupageLigne, const cv::Range decoupageColonne, const int decalagePixelHorizontal, const int decalagePixelVertical, cv::Range &decoupageLigne2, cv::Range &decoupageColonne2, cv::Mat curImg)
{

	decoupageLigne2 = cv::Range(decoupageLigne.start + decalagePixelVertical, decoupageLigne.end + decalagePixelVertical);
	decoupageColonne2 = cv::Range(decoupageColonne.start + decalagePixelHorizontal, decoupageColonne.end + decalagePixelHorizontal);

	//Si on sort de l'image
	if ((decoupageLigne2.start < 0) || (decoupageColonne2.start < 0) || (decoupageLigne2.end > curImg.rows) || (decoupageColonne2.end > curImg.cols))
	{
		//Debogage

		return false;
	}
	std::cout << "Ligne: " << decoupageLigne2.start << ", " << decoupageLigne2.end << std::endl;
	std::cout << "Colonne: " << decoupageColonne2.start << ", " << decoupageColonne2.end << std::endl;
	return true;
}
