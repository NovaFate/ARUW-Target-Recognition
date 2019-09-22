#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;

Mat thresh_callback();
int main( int argc, char** argv )
{
   VideoCapture vid("lol.MOV");

   // Default resolution of the frame is obtained.The default resolution is system dependent. 
   int frame_width = vid.get(CV_CAP_PROP_FRAME_WIDTH); 
   int frame_height = vid.get(CV_CAP_PROP_FRAME_HEIGHT);
   // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
   VideoWriter video("outcpp.avi", CV_FOURCC('M','J','P','G'), 10, Size(frame_width,frame_height));
   while(1) {
   
    vid >> src;

    // If the frame is empty, break immediately
    if (src.empty()) {
      break;
    }
    video.write(thresh_callback());
    waitKey(1);
   }

    // When everything done, release the video capture object
    vid.release();

    // Closes all the frames
    destroyAllWindows();
    return(0);
}

Mat thresh_callback()
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  cvtColor(src, src_gray, CV_BGR2GRAY);

  GaussianBlur(src_gray, src_gray, Size(1, 1) , 0);
  // Detect edges using Threshold

  // less block = more thresh = more c
  // 89 - 15 
  adaptiveThreshold(src_gray, threshold_output, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 69, 23);

  // Find contours
  findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  Mat element = getStructuringElement(0, Size(5, 5), Point(0, 0));

  /// Apply the specified morphology operation
  morphologyEx(threshold_output, threshold_output, MORPH_CLOSE, element);

  // Find the ellipses for each contour
  vector<RotatedRect> minEllipse(contours.size());
  vector<Point> contours2;
  // contour area is area of contour while contours[i].size computes number of contour points
  // drawContours(threshold_output, contours, -1, Scalar(159, 0, 255), CV_FILLED, 8, hierarchy);
  int index = 0;
  for(int i = 0; i < contours.size(); i++) {
        int cPts = contours[i].size();
        int cArea = contourArea(contours[i]);
        if(cPts > 100) {
        	RotatedRect temp = fitEllipse(contours[i]);
		// hierarchy [contour number][Next, Previous, First_Child, Parent]
		if (temp.size.width < 400 && temp.size.height < 400 && temp.size.area() > 2000 && hierarchy[i][2] > 0 && hierarchy[i][3] > 0 ) {
			ellipse2Poly(temp.center, Size2f(temp.size.width / 2, temp.size.height / 2), temp.angle, 0, 359, 50, contours2);
			if (matchShapes(contours[i], contours2, CV_CONTOURS_MATCH_I3, 10) < .1) {
			      minEllipse[index] = temp;
			      index ++;
                        }
       		}
	}
  }       
  for(int a = 0; a < index; a++) {
       Scalar color = Scalar(159, 0, 255);
       ellipse(src, minEllipse[a], color, 5, 8);
  }

  hierarchy.clear();
  contours.clear();
  minEllipse.clear();
  contours2.clear();
  imshow("Frame", src);
  return src;
}
