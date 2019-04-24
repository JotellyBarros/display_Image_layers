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
  Mat src;

  src = imread("/home/jotellybarros/Documents/corroded_n1.jpeg", 1);
  cvtColor(src, hsv_image, COLOR_BGR2HSV);

  Mat imgThreshold1, imgThreshold2, imgThreshold;
  inRange(src,
          Scalar(30, 30, 40),
          Scalar(80, 110, 160),
          imgThreshold1);

  // inRange(src,
  //         Scalar(30, 30, 40),
  //         Scalar(80, 110, 160),
  //         imgThreshold2);

  //imgThreshold = max(imgThreshold1, imgThreshold2); // combining the two thresholds
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
    drawContours(src, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
  }

  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", imgThreshold);
  imshow("Final result", src);

  waitKey(0);

  return 0;
}

//https://stackoverflow.com/questions/43981903/using-inrange-in-opencv-to-detect-colors-in-a-range

// Mat redFilter(const Mat &src)
// {
//   int H_MIN = 0;
// 	int H_MAX = 255;

// 	int S_MIN = 0;
// 	int S_MAX = 0;

// 	int V_MIN = 0;
// 	int V_MAX = 0;

//   assert(src.type() == CV_8UC3);

//   Mat redOnly;
//   inRange(src, Scalar (H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), redOnly);

//   return redOnly;
// }

// int main()
// {

//   Mat image;
//   image = imread("/home/jotellybarros/Documents/aaa.png", CV_LOAD_IMAGE_COLOR); // Read the file

//   if (!image.data) // Check for invalid input
//   {
//     cout << "Could not open or find the image" << std::endl;
//     return -1;
//   }

//   namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
//   imshow("Original Image", image);

//   Mat redOnly = redFilter(image);

//   imshow("redOnly", redOnly);
//   waitKey(0); //Wait for a keystroke in the window

//   return 0;
// }
// //g++ -std=c++11 main.cpp -lstdc++ $(pkg-config --libs --cflags opencv)
