/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#pragma once
#include <iostream>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// Initialisation des variables pour la détection de face
void init_faceDetection();

/*!
  \brief Detects the eyes on an image and return the relative position from the center of the image
  \param[in] img Image on which we want to detect the eyes
  \param[in][out] relative_head_position Vector from the center of the image to the middle of the eyes
  \param[in] scale Scale of the image
  \param[in] draw_eyes True if we want to draw a circle arround the eyes
  \param[in] draw_arrow True if we want to draw an arrow from the center of the image to the center of the eyes
  \param[in] calibrate True if we want to calibrate the depth
*/
bool detectEyes(cv::Mat &img, cv::Point3f *relative_head_position, double scale = 1.0, bool draw_eyes = false, bool draw_arrow = false, bool calibrate = false);

/*!
  \brief Sets the dist_btw_eyes value of the headDetector
  \param[in] dist Distance between the two eyes, in cm
*/
void setEyeDistance(float dist);

/*!
  \brief Try to calibrate the depth with the image. Returns true if success
  \param[in] img Image we use for depth calibration
*/
bool calibrateDepth(cv::Mat &img);
