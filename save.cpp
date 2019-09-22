#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src;

void save_img(int counter);
int main( int argc, char** argv )
{
   int counter = 0;
   // video name
   VideoCapture vid("TEST480.mp4"); 
   int frame_width = vid.get(CV_CAP_PROP_FRAME_WIDTH); 
   int frame_height = vid.get(CV_CAP_PROP_FRAME_HEIGHT); 
   while(1) {
    // Capture frame-by-frame
    vid >> src;
  
    // If the frame is empty, break immediately
    if (src.empty()) {
      break;
    }
    
    save_img(counter);
    counter += 1;
    waitKey(1);
   }
  
    // Closes all the frames
    destroyAllWindows();

  return(0);
}

void save_img(int counter) {
   //fileName += counter;
   cout << fileName << "\n";
   // string name = ("frame%d.jpg", counter);
   // imwrite("frame%d.jpg", count, src);
   // imwrite( "../../savedimg%d.jpg", counter, src);
}



