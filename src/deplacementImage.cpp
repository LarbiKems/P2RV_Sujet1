

#include "deplacementImage.h"

#ifdef BENJI_ET_SES_FONCTIONS_A_LA_CON
//! Touche appuy�e pour le d�bogage. D�place l'image
void appuiTouche(int &decalagePixelVertical, int &decalagePixelHorizontal){

		if (GetKeyState(VK_UP)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelVertical++;
		if (GetKeyState(VK_DOWN)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelVertical--;
		if (GetKeyState(VK_LEFT)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelHorizontal++;
		if (GetKeyState(VK_RIGHT)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelHorizontal--;
}
#endif


//! Calcul du decalage en pixel par rapport a la position de la tete (#Thal�s)
void calculDecalageFenetre(cv::Point3f positionTete,int &decalagePixelVertical, int &decalagePixelHorizontal, float distanceCameraPaysage, float pixelParMetre){
		decalagePixelHorizontal = pixelParMetre*distanceCameraPaysage * positionTete.x/positionTete.z;
		decalagePixelVertical =  pixelParMetre*distanceCameraPaysage * positionTete.y/positionTete.z;
		std::cout << "decalagePixelHorizontal "<<pixelParMetre*distanceCameraPaysage * positionTete.x/positionTete.y<< std::endl;
}



//! cr�e les bon range pour l'image finale. Renvoie false si le d�coupage fait sortir de l'image obtenu par la cam�ra.
//Pour l'avenir : peut �tre impl�menter plut�t un affichage de pixels noirs quand on d�passe de l'image
bool decoupageImage(const cv::Range decoupageLigne, const cv::Range decoupageColonne, const  int decalagePixelHorizontal, const int decalagePixelVertical,cv::Range &decoupageLigne2, cv::Range &decoupageColonne2 , cv::Mat curImg){

		decoupageLigne2 = cv::Range (decoupageLigne.start+decalagePixelVertical,decoupageLigne.end+decalagePixelVertical);
		decoupageColonne2 = cv::Range (decoupageColonne.start+decalagePixelHorizontal,decoupageColonne.end+decalagePixelHorizontal);

		//Si on sort de l'image
		if ((decoupageLigne2.start <0) || (decoupageColonne2.start<0) || (decoupageLigne2.end > curImg.rows) || (decoupageColonne2.end > curImg.cols))
		{
			//Debogage
			
			return false;
		}
		std::cout<< "Ligne: " << decoupageLigne2.start <<", " << decoupageLigne2.end << std::endl;
		std::cout<< "Colonne: " << decoupageColonne2.start <<", " << decoupageColonne2.end << std::endl;
		return true;
}


