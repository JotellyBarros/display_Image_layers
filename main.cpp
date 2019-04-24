//g++ -std=c++11 main.cpp -lstdc++ $(pkg-config --libs --cflags opencv)

#include <cstdio>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  // converts the image to hsv so that circle detection is more accurate
  Mat hsv_image;
  Mat bgr_image;

  bgr_image = imread("images/all_corrosion.jpeg", 1);
  Mat imgThreshold1, imgThreshold2, imgThreshold;

  //Structure color
  int chanel_r = 75;
  int chanel_g = 52;
  int chanel_b = 64;
  int variation_min = 30;
  int variation_max = 40;

  inRange(bgr_image,
          Scalar((chanel_b - variation_min), (chanel_g - variation_min), (chanel_r - variation_min)),
          Scalar((chanel_b + variation_max), (chanel_g + variation_max), (chanel_r + variation_max)),
          imgThreshold1);

  imgThreshold = imgThreshold1;

  Mat element_erode = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
  Mat element_dilate = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));

  /// Apply the erosion and dilation operations
  erode(imgThreshold, imgThreshold, element_erode);
  dilate(imgThreshold, imgThreshold, element_dilate);

  GaussianBlur(imgThreshold, imgThreshold, Size(9, 9), 2, 2);

  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;

  /// Find contours
  findContours(imgThreshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  for (int i = 0; i < contours.size(); i++)
  {
    drawContours(bgr_image, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
  }

  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", imgThreshold);
  imshow("Final result", bgr_image);
  waitKey(0);

  return 0;
}