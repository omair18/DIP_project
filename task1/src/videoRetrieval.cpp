#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv/cv.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <algorithm>

#include "hist.h"

struct dirent **frames;
char frames_directory[100] = "../data/frames/";
char dataBase[100] = "../data/histograms.xml";
char video_directory[100] = "../data/videos/";
char vidName[100];
char outputvidName[100];
char frameName[100];
using namespace std;
using namespace cv;
#define TOTALFRAMES 12

int random_number(int min_num, int max_num);
Mat getHistogramOfVideo(char *arg);


void playVideo(char video[100]){
	VideoCapture cap(video); 

	if(!cap.isOpened()) {
		printf("Error opening video file\n");
		return;
	}
	Mat frame; 
	for(;;) {
		cap >> frame;
		if(!frame.data) 
			break;
		imshow(video, frame);
		waitKey(1);
	}

}

int main(int argc, char **argv) {

	if(argc < 2) {
		printf("\n usage %s <video> \n", argv[0]);
		return 0;
	}

	Mat hist_test = getHistogramOfVideo(argv[1]); //get histogram of test video 
	printf("hist_test = %d\n", hist_test.cols);
	FileStorage fs(dataBase, FileStorage::READ); //read histogram database

	vector<double> results; //vector storing the results of comparison
	int i=1;
	for(;;i++) {
		sprintf(vidName,"%s%d","vid",i);	
		Mat hist; 
		fs[vidName] >> hist;
		if(hist.cols == 0)
			break;

		double comp = compareHist(hist_test, hist, 0);
		printf("Comparison = %f\n",comp);

		results.push_back(comp); //store results
	}
	std::sort(results.begin(), results.end()); //sort according to comparisons
	std::reverse(results.begin(), results.end()); //reverse the sorted array 
	cout<<results[0]<<endl; //maximum will be on top
	/* again do comparisons, this time, when comparison will be equal to results[0], print that video name */
	i=1;
	for(;;i++) { 
		sprintf(vidName,"%s%d","vid",i);	
		Mat hist; 
		fs[vidName] >> hist;
		if(hist.cols == 0)
			break;
		double comp = compareHist(hist_test, hist, 0);
		printf("Comparison = %f\n",comp);
		if(comp == results[0]){
			sprintf(outputvidName,"%s%s.mp4",video_directory,vidName);
			char fname[50];
			sprintf(fname,"%s%s_001.jpg",frames_directory,vidName);
			//imshow(outputvidName,imread(fname));
			cout<<outputvidName;
			//waitKey(2);
			playVideo(outputvidName);
			break;
		}
	}

	return 0;
}

Mat getHistogramOfVideo(char *arg) {

	
	VideoCapture capture(arg); //get video from commandline 
	if ( !capture.isOpened()) {
		printf("Failed to open video \n");
		return Mat();
	}
	string window_name = "Task1. Video in Video out";
	namedWindow(window_name, CV_WINDOW_KEEPRATIO);

	double frames_per_sec = capture.get(CV_CAP_PROP_FPS);
	int frame_diff = frames_per_sec*15; //get frames after 15secs 
	int next_frame = 0;
	double total_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
	printf("total frames = %f\n", total_frames);
	printf("frames per sec = %f\n", frames_per_sec);
	Mat frame, hists;
	for (int i=0; i< TOTALFRAMES; i++) {
			if(i != 0) {
				next_frame = next_frame + frame_diff;
				//printf("i = %d\tnext_frame = %d\n",i,next_frame);
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
			imshow(window_name, frame);
			cv::waitKey(1);
	}

	return hists;

}
int random_number(int min_num, int max_num)
        {
            int result=0,low_num=0,hi_num=0;
            if(min_num<max_num)
            {
                low_num=min_num;
                hi_num=max_num+1; // this is done to include max_num in output.
            }else{
                low_num=max_num+1;// this is done to include max_num in output.
                hi_num=min_num;
            }
            srand(time(NULL));
            result = (rand()%(hi_num-low_num))+low_num;
            return result;
        }
