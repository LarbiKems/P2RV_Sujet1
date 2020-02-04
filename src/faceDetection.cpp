#include "faceDetection.hpp"

#include <cmath>
using namespace std;
using namespace cv;

// PreDefined trained XML classifiers with facial features
bool xml_files_loaded = false;
CascadeClassifier cascade;

bool detectEyes( Mat& img, Point * relative_head_position,  double scale, bool draw_eyes, bool draw_arrow)
{

  // Load xml files if not loaded
  if(!xml_files_loaded) {
    // Change path before execution
    //cascade.load( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalcatface.xml" ) ;
    cascade.load( "resources/haarcascade_eye.xml" ) ;
    xml_files_loaded = true;
  }
  if (!xml_files_loaded) {
    cout << "haarcascade_eye.xml not found" << endl;
    return false;
  }

  vector<Rect> faces;
  Mat gray, smallImg;

  cvtColor( img, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
  double fx = 1 / scale;

  // Resize the Grayscale Image
  resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
  equalizeHist( smallImg, smallImg );

  // Detect faces of different sizes using cascade classifier
  cascade.detectMultiScale( smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30), Size(60, 60) );
  
  // Colors used for Drawing tool
  Scalar blue = Scalar(255, 0, 0); 
  Scalar red = Scalar(0, 255, 0); 

  // If 2 eyes were found
  if (faces.size() == 2) {
    
    Point p1, p2;
    int radius1, radius2;

    // Premier oeil
    Rect r = faces.at(0);
    p1.x = cvRound((r.x + r.width*0.5)*scale);
    p1.y = cvRound((r.y + r.height*0.5)*scale);
    radius1 = cvRound((r.width + r.height)*0.25*scale);

    // Second oeil
    r = faces.at(1);
    p2.x = cvRound((r.x + r.width*0.5)*scale);
    p2.y = cvRound((r.y + r.height*0.5)*scale);
    radius2 = cvRound((r.width + r.height)*0.25*scale);

    // Point entre les deux yeux
    Point center(0.5*(p1.x + p2.x),0.5*(p1.y + p2.y));

    // Point au centre de l'image
    Point img_center(smallImg.cols/2, smallImg.rows/2);

    (*relative_head_position) = center - img_center;

    if (draw_arrow) {
      arrowedLine(img, img_center, center, red, 4);
    }
    if (draw_eyes) {
      circle( img, p1, radius1, blue, 3, 8, 0 );
      circle( img, p2, radius2, blue, 3, 8, 0 );
    }
    return true;
      

  }

  return false;
}
