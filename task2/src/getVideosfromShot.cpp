#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <dirent.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include "hist.h"
#include <pthread.h>
#include <dirent.h>

using namespace cv;
using namespace std;
char xmlDir[100]= "../data/xmls/";
struct dirent **ptr_xml;

vector<double> score_vector;
vector<double> startpos_vector;
vector<double> endpos_vector;

void findShot(Mat shot, char *xmlFile); //given input "shot", it will find the max score, and its start/end time in xmlFile
int findMax(vector<double> &vec);// { //return index of max value

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("usage = %s <shot.mp4> \n", argv[0]);
		return 0;
	}
	printf("Playing shot and computing its feature vector \n");

	VideoCapture capture(argv[1]); //open shot.mp4
	if(!capture.isOpened()){
		printf("Failed to open shot video \n");
		return 0;
	}
	printf("Number of frames in this shot = %f\n",capture.get(CV_CAP_PROP_FRAME_COUNT));
	double frameCount = capture.get(CV_CAP_PROP_FRAME_COUNT);
	double repFrameIdx = frameCount/2; //get the middle frame
	capture.set(CV_CAP_PROP_POS_FRAMES,repFrameIdx); //set the position of video to that frame number
	Mat shotFrame, shotVector, frameVector;
	capture >> shotFrame; //get the frame
//	imshow("shotFrame",shotFrame);
//	waitKey(0);
	shotVector = calculateHistogram(shotFrame); //find histogram of frame
	printf("Vector of shot computed cols= %d rows= %d\n",shotVector.cols, shotVector.rows);


	/* find shotVector in xmlFiles */
	int numXmls = scandir(xmlDir, &ptr_xml, 0, alphasort);
	for(int i=2; i< numXmls; i++) {
		char xmlFullPath[100];
		sprintf(xmlFullPath,"%s%s",xmlDir,ptr_xml[i]->d_name);
		printf("%s\n",xmlFullPath);
		findShot(shotVector, xmlFullPath);
	}
	printf("score_vector = %ld\n",score_vector.size());
	printf("startpos_vector = %ld\n",startpos_vector.size());
	printf("endpos_vector = %ld\n",endpos_vector.size());
	
	for(int i=0; i<score_vector.size(); i++) {
		int idx = findMax(score_vector); //findmax, 2nd max, 3rd max etc ..
		printf("video index = %d \t startpos = %f\t endpos %f\n",idx, startpos_vector[idx], endpos_vector[idx]);	
	}
	

	return 0;
}
void findShot(Mat shot, char *xmlFile) { //given input "shot", it will find the max score, and its start/end time in xmlFile
	FileStorage fs (xmlFile, FileStorage::READ); //open xml file to read
	int totalshots;
	double currentScore, maxScore, startPos, endPos;
	fs["totalshots"] >> totalshots; //get total shots of this file
//	printf("totalshots = %d\n",totalshots);
	char startPosBuff[100];
	char shotBuff[100];
	char endPosBuff[100];
	Mat  xmlshot;

	for(int i=0; i<totalshots; i++) {
		sprintf(shotBuff, "shot%d",i);
		fs[shotBuff]>>xmlshot; //pick shotvector from xml file
		currentScore = compareHist(shot,  xmlshot, 0); //compute score
		if(i == 0) {
			maxScore = currentScore;
			sprintf(startPosBuff, "start%d",i);
			sprintf(endPosBuff, "end%d",i);
			fs[startPosBuff]>>startPos; //pick start position
			fs[endPosBuff]>>endPos; //pick end position
		}
		else {
			if(currentScore > maxScore) {
				maxScore = currentScore;
				sprintf(startPosBuff, "start%d",i);
				sprintf(endPosBuff, "end%d",i);
				fs[startPosBuff]>>startPos; //pick start posoiton
				fs[endPosBuff]>>endPos; //pick end psotion

			}
		}
	}
	printf("maxScore = %f\t startPos = %f\t endPos = %f\n", maxScore, startPos, endPos);
	score_vector.push_back(maxScore);
	startpos_vector.push_back(startPos);
	endpos_vector.push_back(endPos); //store maxscore and start/end position of shot here

	/*VideoCapture cap("../data/videos/vid2.mp4");
	if(!cap.isOpened())
		printf("ERROR ! :D \n");

	Mat frame;
	double fps = cap.get(CV_CAP_PROP_FPS);
	cap >> frame;
	imshow("frame1", frame);

	cap.set(CV_CAP_PROP_POS_FRAMES, 700/fps);
	cap >> frame;
	imshow("frame2", frame);
	waitKey(0); */
	
	return;
}
int findMax(vector<double> &vec) { //return index of max value
	double max = vec[0];
	int max_index=0;
	for(int i=0; i<vec.size(); i++){
		if(vec[i] > max) {
			max = vec[i];
			max_index = i;
		}
	}	
	vec[max_index] = 0;
	return max_index; //return;
}
