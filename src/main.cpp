#include "faceDetection.hpp"

int main(int argc, char** argv) {

  Mat image;
  image = imread("lenna.png", CV_LOAD_IMAGE_COLOR);
  detectAndDraw(image, 1);
  namedWindow("Lena", WINDOW_AUTOSIZE);
  imshow("Lena", image);
  waitKey(0);
  /** Boucle affichage des images des caméras */

  // Image Avant

  // Image arrière

  // Traitement des images

  // Image avant traitée

  // Image arrière traitée
  return 0;
}
