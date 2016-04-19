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
#include <sys/types.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;

#define BIN_SIZE 256
int histindex=0;
void *saveShots(void * arg);
char shot_directory[100] = "../data/xmls/";
struct dirent **shots;
struct vidData {
	std::string dir;
	std::string origName;
	std::string newName;
	std::string extension;
};
int main(int argc, char** argv) //todo.. use threads for each video
{
    if (argc < 2)
    {
	    printf("usage = %s <videosMappingFile.xml> \n",argv[0]);
	    return 1;
    }
    int rc;

    FileStorage fs(argv[1],FileStorage::READ);
    int numvids;
    //char vidDir[100]; //video directory name
    std::string vidDir;
    char origTag[100];
    char newTag[100];
    std::string filename;
    char extTag[100];
    std::string extname;
    fs["total"] >> numvids;
    fs["dir"] >> vidDir; //get directory name from mapping file
    pthread_t threads[100];
    int j=0;


    struct vidData vdata;
    vdata.dir = vidDir;
    for(j=0; j<numvids; j++) {
     		//saveShots(argv[j]); // call 
		sprintf(origTag,"orig%d",j);	 //get orig0,orig1
		sprintf(newTag,"new%d",j);	 //get orig0,orig1
		sprintf(extTag,"extension%d",j);

		fs[origTag]>> vdata.origName;
		fs[newTag]>> vdata.newName;
		fs[extTag]>>vdata.extension;
		rc = pthread_create(&threads[j],NULL, saveShots,(void *)&vdata);	
		usleep(10000);
		//saveShots((void *)fullfile);
    }  
    printf("Total threads = %d\n",j);
    for(int i=0; i<j; i++) //
    {
	    pthread_join(threads[i],NULL);
    }
    return 0;
}
std::string getFileName(char *arg) {
	std::string file = arg;
	file = file.substr(15);
	std::size_t pos = file.find("."); //video.mp4
	std::string sbstr = file.substr(pos); //sbstr = .mp4
	file.erase(pos,sbstr.size());
	return file;
}
void *saveShots(void *argv) {
		struct vidData *arg = (struct vidData*)argv;
		char fullfile[100];
		sprintf(fullfile,"%s%s.%s",arg->dir.c_str(),arg->origName.c_str(), arg->extension.c_str());
		//printf("%s\n",fullfile);
		//string vidname = getFileName(arg);
		int i=0,index=0;
		double fps, total_frames, currentFramePos, startpos=0, endpos;
        	char startBuff[100],endBuff[100],shotBuff[100];
        	char histFilename[200];
        	Mat nextframe;
        	Mat frame2;
		Mat prev_frame;
		sprintf(histFilename,"%s/hist_%s.xml",shot_directory,arg->newName.c_str());	
		VideoCapture capture(fullfile); //try to open string, this will attempt to open it as a video file
		if(!capture.isOpened())
		{
			cerr << "Failed to open a video device or video file!\n" << endl;
			return (void *)0;
		} 
		
		fps = capture.get(CV_CAP_PROP_FPS); //get frame per second
		total_frames = capture.get(CV_CAP_PROP_FRAME_COUNT); //total number of frames 
		FileStorage fs(histFilename, FileStorage::WRITE);  //file storage to write
		fs<<"FileName"<<arg->newName.c_str();	
		for(;;){	
			if(i == 0) {
				startpos = 0; //starting position of video
				capture >> prev_frame;
				i++;
			}
			else {
				capture >> nextframe;
				if(!nextframe.data){ //end of video, save the last frame
					
					Mat hist = calculateHistogram(prev_frame);  //calculate histogram of frame
					//imwrite(filename, prev_frame);// save previous shot
					sprintf(startBuff,"start%d",index);
					sprintf(endBuff,"end%d",index);
					sprintf(shotBuff,"shot%d",index);  //for the sake of differentiating between different shots in same xml file
					endpos = currentFramePos; //frame position just before ending
					fs<<startBuff<<startpos/fps; //save start position
					fs<<endBuff<<endpos/fps; //save end position of shot
					fs<<shotBuff<<hist; 
					

					printf("Total Shots = %d\n", index);
					fs<<"totalshots"<<index+1;
					fs.release(); 
					break; //break; :) 
				}
				Mat hist1 = calculateHistogram(prev_frame); //compute histogram of both frames
				Mat hist2 = calculateHistogram(nextframe);
				double hist_diff = compareHist(hist1, hist2, 0); //compare both histograms
				if(hist_diff <= 0.5) { //if comparison is less than a threshold, consider it as a shot
					cout<<">>>>>>> hist_similarity " <<fullfile<<" = "<<hist_diff << endl;
					Mat hist = calculateHistogram(prev_frame);  //calculate histogram of frame
					//imwrite(filename, prev_frame);// save previous shot
					endpos = capture.get(CV_CAP_PROP_POS_MSEC); //current position
					//printf("currenPos = %f\t startpos = %f\t endpos = %f\n",capture.get(CV_CAP_PROP_POS_MSEC),startpos, endpos);
					//imshow("prev_frame",prev_frame);
					//waitKey(0);
					
					sprintf(startBuff,"start%d",index);
					sprintf(endBuff,"end%d",index);
					sprintf(shotBuff,"shot%d",index);  //for the sake of differentiating between different shots in same xml file

					fs<<startBuff<<startpos/fps; //save start position
					fs<<endBuff<<endpos/fps; //save end position of shot
					fs<<shotBuff<<hist; 
					//fs.release(); 
					startpos = endpos; //swap start and end time for next shot
					index++;
				}
				currentFramePos = capture.get(CV_CAP_PROP_POS_MSEC);
			        std::swap(prev_frame, nextframe);	 //swap both frames
			} //else
		}//for(;;)
		fs.release();
		
}
