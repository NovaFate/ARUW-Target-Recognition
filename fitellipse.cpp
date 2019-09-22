#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
RNG rng(12345);

// void thresh_callback(int, void* );
Mat thresh_callback();
/** @function main */
int main( int argc, char** argv )
{
   VideoCapture vid("TEST480.mp4");
   // Default resolution of the frame is obtained.The default resolution is system dependent. 
   int frame_width = vid.get(CV_CAP_PROP_FRAME_WIDTH); 
   int frame_height = vid.get(CV_CAP_PROP_FRAME_HEIGHT); 
   //  int counter = 0; 
   // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
    VideoWriter video("outcpp.avi", CV_FOURCC('M','J','P','G'), 10, Size(frame_width,frame_height)); 
   while(1) {
    // counter ++;
    // Capture frame-by-frame
    vid >> src;
  
    // If the frame is empty, break immediately
    if (src.empty()) {
      break;
    }

    video.write(thresh_callback());
    waitKey(0);
   }
  
    // When everything done, release the video capture object
     vid.release();
 
    // Closes all the frames
    destroyAllWindows();
  //    cout << "Num Frames : " << "\n";
  //  cout << counter << "\n";
  return(0);
}

/** @function thresh_callback */
// void thresh_callback(int, void* )
Mat thresh_callback()
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  cvtColor(src, src_gray, CV_BGR2GRAY);
  // int blockSize = 2;
  // int apertureSize = 3;
  //  double k = 0.04;

  // Detecting corners
  // cornerHarris(src_gray, src_gray, blockSize, apertureSize, k, BORDER_DEFAULT);
  // cvtColor(src_gray, src_gray, CV_BGR2GRAY);
  // split(src_gray, 1);
  // src_gray.convertTo(src_gray, CV_8UC1);
  // HoughLinesP(src_gray, contours, 1, CV_PI/180, 100, 0, 0 );
  // blur(src_gray, src_gray, Size(3,3));
   GaussianBlur(src_gray,src_gray, Size(5,5) , 0);
  // threshold(src_gray, threshold_output, 80, 255, THRESH_BINARY);
  // Detect edges using Threshold
  
  // less block = more thresh = more c
  adaptiveThreshold(src_gray, threshold_output, 251, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 17, 15); 
  
  // Find contours
  findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


  Mat element = getStructuringElement(0, Size(5, 5), Point(0, 0));

  /// Apply the specified morphology operation
  morphologyEx(threshold_output, threshold_output, MORPH_CLOSE, element);

  // Find the ellipses for each contour
  vector<RotatedRect> minEllipse(contours.size());
  // contour area is area of contour while contours[i].size computes number of contour points 
  for(int i = 0; i < contours.size(); i++) { 
	int cPts = contours[i].size();
	int cArea = contourArea(contours[i]);
	// cpts < 500 
        // > 300 > 150 area
       if(cPts > 150 && cArea > 150) { 
	   //	cout << cArea << "\n";
		minEllipse[i] = fitEllipse(contours[i]);
       } else { 
           contours.erase(contours.begin() + i);
	   i--;
       } 
  }
 
  //minEllipse.shrink_to_fit();			
  for(int a = 0; a < minEllipse.size(); a ++ ) {
       Scalar color = Scalar(159, 0, 255);
       // ellipse
       double width = minEllipse[a].size.width;
       double height = minEllipse[a].size.height;
       double area = minEllipse[a].size.area();
       //double angle = minEllipse[a].angle;
       if (width < 200 && height < 200 && area > 7000) {
	  //cout << minEllipse[a].center << "\n";
          // cout << "Width: " << "\n";
	  // cout << width << "\n";
	  // cout << "Height: " << "\n";
  	  // cout << height << "\n";
          // cout << "Angle: " << "\n";
	  // cout << angle << "\n";
       	  ellipse(threshold_output, minEllipse[a], color, 5, 8);
       }
  }

  hierarchy.clear();
  contours.clear();
  minEllipse.clear();
  imshow("Frame", threshold_output);  
  return threshold_output;
  // Mat resizedCropped;
  // Size size(800, 800);
  // resize(threshold_output, resizedCropped, size);
  // imshow( "Resized", threshold_output);
}
