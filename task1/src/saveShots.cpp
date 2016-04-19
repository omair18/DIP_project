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

using namespace cv;
using namespace std;

#define TOTALFRAMES 12
#define BIN_SIZE 256

int saveShots(int argc, char ** arg);

char shot_directory[100] = "../data/shots";
struct dirent **shots;

int main(int ac, char** av)
{
    if (ac < 2)
    {
	    printf("usage = %s <video1> <video2> ... <videoN> \n",av[0]);
	    return 1;
    }
     return saveShots(ac, av); // call 
}
int saveShots(int argc, char ** arg) {
	//int m = scandir(shot_directory, &shots, 0, alphasort);  //scan directory
	//printf(" m = %d\n", m);
	system("rm -rf ../data/shots/*"); //remove previous shots 
	for (int j=1; j< argc; j++) {

		printf("\nSaving shots of video = %s\n",arg[j]);
		//string last_file = shots[m]->d_name;  //get the last file of directory 
		/*string last_file = arg[j];
		std::size_t pos = last_file.find("."); //vid1.mp4
		std::string sbstr = last_file.substr(pos);  //.mp4
		last_file.erase(pos, sbstr.size()); //shot1
		pos = last_file.find("vid");	
		sbstr = "vid";
		last_file = last_file.substr(pos);
		last_file.erase(pos, sbstr.size()); //vid1 - vid = 1
	
		shot_index = std::stoi(last_file);
		printf("shot_index = %d\n",shot_index);
		return 0; */
		VideoCapture capture(arg[j]); //try to open string, this will attempt to open it as a video file
		if (!capture.isOpened())
		{
			cerr << "Failed to open a video device or video file!\n" << endl;
			return 1;
		} 
		int n=0;
        	char filename[200];
        	char histFilename[200];
        	Mat nextframe;
        	Mat frame2;
	
		Mat prev_frame;
		int i=0;
		for(;;){	
			if(i == 0) {
				capture >> prev_frame;
				i++;
			}
			else {
				capture >> nextframe;
				if(!nextframe.data){ //end of video, save the last frame
					
					sprintf(filename, "%s/vid%d_%.3d.jpg",shot_directory,j, n);
					sprintf(histFilename, "%s/hist%d_%.3d.xml",shot_directory,j, n++);

					FileStorage fs(histFilename, FileStorage::WRITE);  //file storage to write
					Mat hist = calculateHistogram(prev_frame);  //calculate histogram of frame
					imwrite(filename, prev_frame);// save previous shot
					fs<<"shot"<<hist; 
					fs.release(); 

					printf("Total Shots = %d\n", n);
					break; //break; :) 
				}
				Mat hist1 = calculateHistogram(prev_frame); //compute histogram of both frames
				Mat hist2 = calculateHistogram(nextframe);
				double hist_diff = compareHist(hist1, hist2, 0); //compare both histograms
				if(hist_diff <= 0.5) { //if comparison is less than a threshold, consider it as a shot
					cout<<">>>>>>> hist_similarity =  " << hist_diff << endl;
					sprintf(filename, "%s/vid%d_%.3d.jpg",shot_directory,j, n);
					sprintf(histFilename, "%s/hist%d_%.3d.xml",shot_directory,j, n++);

					FileStorage fs(histFilename, FileStorage::WRITE);  //file storage to write
					Mat hist = calculateHistogram(prev_frame);  //calculate histogram of frame
					imwrite(filename, prev_frame);// save previous shot
					fs<<"shot"<<hist; 
					fs.release(); 
				}
			        std::swap(prev_frame, nextframe);	 //swap both frames
			}
		}	
    	}
		return 0;
}
