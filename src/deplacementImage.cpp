

#include "deplacementImage.h"

//! Touche appuyée pour le débogage. Déplace l'image
void appuiTouche(int &decalagePixelVertical, int &decalagePixelHorizontal){

		if (GetKeyState(VK_UP)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelVertical++;
		if (GetKeyState(VK_DOWN)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelVertical--;
		if (GetKeyState(VK_LEFT)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelHorizontal++;
		if (GetKeyState(VK_RIGHT)& 0x8000/*Check if high-order bit is set (1 << 15)*/) decalagePixelHorizontal--;
}


//! Calcul du decalage en pixel par rapport a la position de la tete (#Thalès)
void calculDecalageFenetre(float positionTete[3],int &decalagePixelVertical, int &decalagePixelHorizontal, float distanceCameraPaysage, float pixelParMetre){
		decalagePixelHorizontal = distanceCameraPaysage * positionTete[0]/positionTete[1]/pixelParMetre;
		decalagePixelVertical =  distanceCameraPaysage * positionTete[2]/positionTete[1]/pixelParMetre;
}



//! crée les bon range pour l'image finale. Renvoie false si le découpage fait sortir de l'image obtenu par la caméra.
//Pour l'avenir : peut être implémenter plutôt un affichage de pixels noirs quand on dépasse de l'image
bool decoupageImage(const cv::Range decoupageLigne, const cv::Range decoupageColonne, const  int decalagePixelHorizontal, const int decalagePixelVertical,cv::Range &decoupageLigne2, cv::Range &decoupageColonne2 , cv::Mat curImg){

		decoupageLigne2 = cv::Range (decoupageLigne.start+decalagePixelVertical,decoupageLigne.end+decalagePixelVertical);
		decoupageColonne2 = cv::Range (decoupageColonne.start+decalagePixelHorizontal,decoupageColonne.end+decalagePixelHorizontal);

		//Si on sort de l'image
		if ((decoupageLigne2.start <0) || (decoupageColonne2.start<0) || (decoupageLigne2.end > curImg.rows) || (decoupageColonne2.end > curImg.cols))
		{
			//Debogage
			std::cout<< decoupageLigne2.start << std::endl <<decoupageColonne2.start << std::endl << decoupageLigne2.end << std::endl << decoupageColonne2.end << std::endl;
			return false;
		}
		return true;
}


