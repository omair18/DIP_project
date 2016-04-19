#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>

using namespace cv;
using namespace std;

#define TOTALFRAMES 12

//hide the local functions in an anon namespace
    int process(int argc, char ** arg)
    {
	    for (int j=1; j< argc; j++) {
		VideoCapture capture(arg[j]); //try to open string, this will attempt to open it as a video file
		if (!capture.isOpened())
		{
			cerr << "Failed to open a video device or video file!\n" << endl;
			return 1;
		} 
		int n=0;
        	char filename[200];
        	string window_name = "video | q or esc to quit";
        	namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
        	Mat frame;
	
		double frames_per_sec = capture.get(CV_CAP_PROP_FPS);
		int frame_diff = frames_per_sec*15; //get frames after 15secs 
		int next_frame = 0;
		printf("total frames = %f\n",capture.get(CV_CAP_PROP_FRAME_COUNT));
		printf("frames per sec = %f\n", capture.get(CV_CAP_PROP_FPS));
	
        	for (int i=0; i< TOTALFRAMES; i++)
		{
			if(i != 0) {
				next_frame = next_frame + frame_diff;
				printf("next_frame = %d\n",next_frame);
				capture.set(CV_CAP_PROP_POS_FRAMES, next_frame);
			}	
			capture >> frame;
			sprintf(filename, "../data/frames/vid%d_%.3d.jpg",j, n++);
			printf("filename = %s\n",filename);
			imshow(window_name, frame);
			cv::waitKey(5); 
		

			imwrite(filename, frame);	
	       	}
    	}
		return 0;
    }
Mat computeDescriptors(Mat frame) {
	/*SurfFeatureDetector detector(500);
	vector <KeyPoint> keypoints;
	//detector.detect(frame, keypoints);

	SurfDescriptorExtractor extractor; */
	Mat descriptors;
	return descriptors;
	//extractor.compute(frame, keypoints, descriptors);
}
int main(int ac, char** av)
{
    if (ac < 2)
    {
	    printf("usage = %s <video1> <video2>  \n",av[1]);
	    return 1;
    }
     return process(ac, av); // call 
}
