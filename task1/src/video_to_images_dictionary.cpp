#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include "hist.h"

using namespace cv;
using namespace std;

#define TOTALFRAMES 12
#define BIN_SIZE 256
Mat calculateHistogram(Mat img);
void drawHistrogram(vector<Mat> hists);
int process(int argc, char ** arg);

//hide the local functions in an anon namespace


int main(int ac, char** av)
{
    if (ac < 2)
    {
	    printf("usage = %s <video1> <video2>  \n",av[1]);
	    return 1;
    }
     return process(ac, av); // call 
}

int process(int argc, char ** arg)
{
	    FileStorage fs("../data/histograms.xml",FileStorage::WRITE);
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
		Mat hists;
		double frames_per_sec = capture.get(CV_CAP_PROP_FPS);
		int frame_diff = frames_per_sec*15; //get frames after 15secs 
		int next_frame = 0;
		double total_frames;
		total_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
		printf("total frames = %f\n", total_frames);
		printf("frames per sec = %f\n", capture.get(CV_CAP_PROP_FPS));
	
        	for (int i=0; i< TOTALFRAMES; i++)
		{
			if(i != 0) {
				next_frame = next_frame + frame_diff;
				printf("next_frame = %d\n",next_frame);
				if(next_frame > total_frames) {
					frame = Mat::zeros(frame.rows, frame.cols,CV_8UC3); //empty frame
				}
				else  {
					capture.set(CV_CAP_PROP_POS_FRAMES, next_frame);
					capture >> frame;
				}
			}	
			if(i == 0)
				capture >> frame;

			Mat hist_img = calculateHistogram(frame);	 // returns histogram
			if(i == 0) //first image
			{
				hconcat(hist_img,hists);
			}
			else {
				hconcat(hists,hist_img,hists);
				//cout<<hists.rows<<'\t'<<hists.cols<<endl;
			}
			sprintf(filename, "../data/frames/vid%d_%.3d.jpg",j, n++);
			printf("filename = %s\n",filename);
			imwrite(filename, frame);	 //save frame on disk
			imshow(window_name, frame);
			cv::waitKey(1); 
	       	}
		cout<<">>>>>>>>> hist frame = "<<hists.cols<<endl;
		string sc = string(arg[j]).substr(15,4);
		printf("string sc = %s\n",sc.c_str());
		fs<<sc<<hists;
    	}
	    	fs.release(); //close file
		return 0;
    }
