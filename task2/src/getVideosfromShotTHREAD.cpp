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
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;
char xmlDir[100]= "../data/xmls/";
struct dirent **ptr_xml;

vector<double> score_vector;
vector<double> startpos_vector;
vector<double> endpos_vector;
Mat shotVector; //matrix to store feature vector of input shot video
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *findShot(void *xmlFile); //given input "shot", it will find the max score, and its start/end time in xmlFile
int findMax(vector<double> &vec); //return index of max value

int main(int argc, char **argv) {
	if(argc < 3) {
		printf("usage = %s <shot.mp4> <database videos_mapfile.xml>\n", argv[0]);
		return 0;
	}
	printf("\nPlaying shot and computing its feature vector using MULTI-THREADING \n");

	VideoCapture capture(argv[1]); //open shot.mp4
	if(!capture.isOpened()){
		printf("Failed to open shot video \n");
		return 0;
	}
	printf("\nNumber of frames in this shot = %f\n",capture.get(CV_CAP_PROP_FRAME_COUNT));
	double frameCount = capture.get(CV_CAP_PROP_FRAME_COUNT);
	double repFrameIdx = frameCount/2; //get the middle frame
	capture.set(CV_CAP_PROP_POS_FRAMES,repFrameIdx); //set the position of video to that frame number
	Mat shotFrame, frameVector;
	capture >> shotFrame; //get the frame
//	imshow("shotFrame",shotFrame);
//	waitKey(0);
	shotVector = calculateHistogram(shotFrame); //find histogram of frame
//	printf("Vector of shot computed cols= %d rows= %d\n",shotVector.cols, shotVector.rows);
/*pthread stuff */
	pthread_t worker_threads[100];

	/* find shotVector in xmlFiles */
	int numXmls = scandir(xmlDir, &ptr_xml, 0, alphasort);
	int tid=0;
	for(int i=2; i< numXmls; i++) {
		char xmlFullPath[100];
		sprintf(xmlFullPath,"%s%s",xmlDir,ptr_xml[i]->d_name);
		//printf("%s\n",xmlFullPath);
		pthread_create(&worker_threads[tid++], NULL, findShot, (void *)&xmlFullPath); //each thread will work on exactly 1 xml file to find the relevant shot
		usleep(10000); // abit of wait 0.1 secs
	}
	for(int i=0; i<tid; i++) {
		pthread_join(worker_threads[i],NULL); //join all the spawned threads
	}

	/* retrieve Original VideoNames */
	std::string dirName;
	std::string origName;
	std::string extName;
	char origTag[100];
	char extTag[100];
	char fullfile[100];
	printf("\n RETRIEVED VIDEOS \n");	
	FileStorage rFs(argv[2],FileStorage::READ); //read Mapping file to retrieve original file name

	rFs["dir"] >> dirName; //get directory name
	for(int i=0; i<score_vector.size(); i++) {
		int idx = findMax(score_vector); //findmax, 2nd max, 3rd max etc ..
		sprintf(origTag,"orig%d",idx);
		sprintf(extTag,"extension%d",idx);
		rFs[origTag] >> origName; //get original Name 
		rFs[extTag] >> extName; //get its extension

		sprintf(fullfile,"%s%s.%s",dirName.c_str(),origName.c_str(), extName.c_str());
		printf("\n%s\t",fullfile);
		printf("Start Frame = %f\t End Frame %f\n",startpos_vector[idx], endpos_vector[idx]);
		//printf("video index = %d \t startpos = %f\t endpos %f\n",idx, startpos_vector[idx], endpos_vector[idx]);	
	}
	return 0;
}
void *findShot(void *file) { //given input "shot", it will find the max score, and its start/end time in xmlFile
	char *xmlFile = (char *)file;
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
		currentScore = compareHist(shotVector,  xmlshot, 0); //compute score
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
	printf("maxScore = %f\t startPos = %f\t endPos = %f\n\n", maxScore, startPos, endPos);

	pthread_mutex_lock(&mutex1);
	score_vector.push_back(maxScore);
	startpos_vector.push_back(startPos);
	endpos_vector.push_back(endPos); //store maxscore and start/end position of shot here
	pthread_mutex_unlock(&mutex1);
	return (void *)0;
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

