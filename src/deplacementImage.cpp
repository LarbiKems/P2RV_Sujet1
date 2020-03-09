

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
#if 0
void ChangementPointDeVue(cv::Mat img, cv::Mat projMatrix, Point3f headPosition, cv::Mat result)
{
	result.release();
	result = Mat(Size(img.cols, img.rows), CV_8UC3, Scalar(0, 0, 0));

	// Calcul de la matrice de transformation de la caméra associée à la tête

	// Rotation autour de Y
	float norm = std::sqrt(headPosition.dot(headPosition));
	float cosY = 0; // TODO;
	float sinY = 0; //TODO;
	Mat Ry = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosY, -sinY, 0, sinY, cosY);

	// Rotation autour de X
	float cosX = 0; // TODO;
	float sinX = 0; //TODO;
	Mat Rx = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosX, -sinX, 0, sinX, cosX);

	// Rotation autour de Z
	float cosZ = 0; // TODO;
	float sinZ = 0; //TODO;
	Mat Rz = (Mat_<float>(3, 3) << 1, 0, 0, 0, cosZ, -sinZ, 0, sinZ, cosZ);

	Mat Rot = Rx * Ry * Rz;

	// Matrice de transformation
	// TODO: Construire [R | headPosition]
	Mat headRTMatrix = (Mat_<float>(3, 4) << 0); // à remplir (pas sur des dimensions)

	Mat headProjMatrix = projMatrix * headRTMatrix;
	
	// TODO: choisir le même repère pour la création de matrice de transformation et la détection de la tête
	for (int u = 0; u < img.rows; u++)
	{
		for (int v = 0; v < img.cols; v++)
		{
			// Calcul Point réel (x,y,z) dans le repère de la caméra.
			Mat Puv = (Mat_<float>(2, 1) << u, v);

			Mat Pxyz = projMatrix.inv(DECOMP_LU ) * Puv;

			// Calcul de la position du point en question dans l'écran de la caméra associée à la tête
			Mat Pwz = headProjMatrix * Pxyz;
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
#endif
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
