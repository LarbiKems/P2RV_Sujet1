#include "deplacementImage.h"


//! Calcul du decalage en pixel par rapport a la position de la tete (#Thal�s)
// Source pour la valeur de pixelParMetre: http://www.yourwebsite.fr/index.php/documents/287-relation-entre-pixel-et-taille-s-des-images
void calculDecalageFenetre(cv::Point3f positionTete,int &decalagePixelVertical, int &decalagePixelHorizontal, float distanceCameraPaysage, float pixelParMetre){
		decalagePixelHorizontal = pixelParMetre*distanceCameraPaysage * positionTete.x/positionTete.z;
		decalagePixelVertical =  pixelParMetre*distanceCameraPaysage * positionTete.y/positionTete.z;
}

//! cr�e les bon range pour l'image finale. Renvoie false si le d�coupage fait sortir de l'image obtenu par la cam�ra.
//Pour l'avenir : peut �tre impl�menter plut�t un affichage de pixels noirs quand on d�passe de l'image
bool decoupageImage(const cv::Range decoupageLigne, const cv::Range decoupageColonne, const  int decalagePixelHorizontal, const int decalagePixelVertical,cv::Range &decoupageLigne2, cv::Range &decoupageColonne2 , cv::Mat curImg){

		decoupageLigne2 = cv::Range (decoupageLigne.start+decalagePixelVertical,decoupageLigne.end+decalagePixelVertical);
		decoupageColonne2 = cv::Range (decoupageColonne.start+decalagePixelHorizontal,decoupageColonne.end+decalagePixelHorizontal);

		//Si on sort de l'image
		if ((decoupageLigne2.start <0) || (decoupageColonne2.start<0) || (decoupageLigne2.end > curImg.rows) || (decoupageColonne2.end > curImg.cols))
		{
			return false;
		}
		return true;
}


