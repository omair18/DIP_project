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
struct dirent **ptr_dir;
char videoDir[100] = "../data/videos/";
char xmlFile[100] = "../data/videosMap.xml";
char vidName[100];
char origName[100];
char newName[100];
char newTag[100];
char origTag[100];
char extensionTag[100];
char extension[100];

std::string getFileName(char *arg); //remove extension and gives filename
std::string getExtension(char *arg); //returns extension of file, eg. file.mp4 will be returned as mp4
int main (int argc, char **argv) {
	if(argc < 2) {
		printf("Usage = %s <directory path containing videos\n", argv[0]);
		return 0;
	}
	int numvids = scandir(argv[1], &ptr_dir, 0, alphasort);
	printf("Number of videos = %d\n", numvids-2);
	FileStorage fs (xmlFile, FileStorage::WRITE);
	fs<<"total"<<numvids-2;
	fs<<"dir"<<argv[1];
	int j=2;	
	for(int i=0; i<numvids-2; i++) {
		sprintf(origTag,"orig%d",i); //xml tags
		sprintf(newTag,"new%d",i); //xml tags
		sprintf(extensionTag,"extension%d",i);

		string name= getFileName(ptr_dir[j]->d_name);
		sprintf(origName,"%s",name.c_str()); //original title of video
		printf("origName = %s\n",origName);
		sprintf(newName,"vid%d",i); //vid0, vid1, vid2
		string ext = getExtension(ptr_dir[j++]->d_name);
		sprintf(extension,"%s",ext.c_str()); //mp4

		fs<<origTag<<origName; //write both
		fs<<newTag<<newName;
		fs<<extensionTag<<ext;
	}
	fs.release();
	printf("\n\n, Video Mapping File Saved at %s\n",xmlFile);
}

std::string getFileName(char *arg) {
	std::string file = arg;
	//file = file.substr(15);
	std::size_t pos = file.find("."); //video.mp4
	std::string sbstr = file.substr(pos); //sbstr = .mp4
	file.erase(pos,sbstr.size());
	return file;
}

std::string getExtension(char *arg) {
	std::string file = arg;
	std::size_t pos = file.find("."); //video.mp4
	std::string sbstr = file.substr(pos+1); //sbstr = .mp4
	//file.erase(pos,sbstr.size());
	return sbstr;
}
