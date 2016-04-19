#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <algorithm>

#include "hist.h"

struct dirent **shots; 
char shots_directory[100] = "../data/shots/";
char video_directory[100] = "../data/videos/";
char shots_hist_directory[100] = "../data/shots/";

void playVideo(char video[]);

int main (int argc, char **argv) 
{
	if(argc < 2) {
		printf("Usage: %s <image> \n", argv[0]);
		return 0;
	}

	int n = scandir(shots_hist_directory, &shots, 0, alphasort); //open directory to get xml files 
	if (n <= 2) {
		printf("Run saveShots program first !! \n");
		return 0;
	}
	else { //get all histograms and compare
		Mat img = imread(argv[1]);
		imshow("Test Image", img);
		waitKey(0);
		Mat hist_img = calculateHistogram(img); // read and calculate histogram of image
		double score, maxScore; 
		char histName[100]; 
		char FileName[100]; 
		int i;
		for(i=2; i< n; i++) {
			sprintf(histName, "%s%s",shots_directory, shots[i]->d_name);
			string sc = string(shots[i]->d_name);
			std::size_t indx = sc.find(".xml");
			if(indx != -1) {// if found
				//printf("xml file = %s", histName);
				FileStorage fs(histName, FileStorage::READ); //open xml file to read histogram 
				Mat hist_data;
				if(fs.isOpened()) {
					fs["shot"] >> hist_data; //get histogram here
				}
				if(i == 2) { //if first time 
					score = compareHist(hist_img, hist_data, 0); //compare both histograms 
					maxScore = score; //max score 	
					strcpy(FileName, shots[i]->d_name); //save file name
				}
				else {
					score = compareHist(hist_img, hist_data, 0); //compare both histograms 
					if (score > maxScore) {
						maxScore = score;
						strcpy(FileName, shots[i]->d_name); //save that filename
					}
			
				}
				//printf("\t score = %f\n", score);
			}
			//once out of loop, means all histograms have been compared
		}
		printf("FileName = %s\t Score = %f\n", FileName, maxScore);
		string fname = string(FileName);
		size_t pos = fname.find("_"); //hist1_001.xml
		string str = fname.substr(pos);  //_001.xml
		fname.erase(pos, str.size()); //hist1
		str = "hist";
		fname.erase(0, str.size()); 
		int vidIndex = std::stoi(fname); //get 1
		printf("fname = %s\t vidIndex = %d\n",fname.c_str(), vidIndex);
		char video_name[100];
		sprintf(video_name, "%svid%d.mp4",video_directory,vidIndex);
		printf("DETECTED VIDEO PATH = %s\n", video_name);
		
		playVideo(video_name);
	}
}


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
