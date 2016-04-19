#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/legacy/legacy.hpp>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>

using namespace cv;
struct dirent **img_dir;
//char dir[] = "/windows/D/MegaDrive/Study/LUMS/Spring16/Vision_DIP/DIP_CS674/project/data/frames";
char dir[] = "../data/frames";

Mat computeDescriptors(Mat img) {
	//imshow("window", img);
	//waitKey(5000);
	vector<KeyPoint> keypoints;
	/*Ptr<FeatureDetector> detector = FeatureDetector::create("SURF");
	detector->detect(img, keypoints); */
	SurfFeatureDetector detector(500);
	detector.detect(img,keypoints);

	Mat descriptor;
	SurfDescriptorExtractor extractor;
	extractor.compute(img,keypoints,descriptor);
	/*Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SURF");
	extractor->compute(img, keypoints, descriptor); */
	return descriptor;
	
}
int main() {
	printf("Reading directory\n");

	int n = scandir(dir, &img_dir, 0, alphasort);
	int i=3;
	char filename[200];
	Mat img;
	Mat des;
	FileStorage fs("../data/descriptors.yaml",FileStorage::WRITE);
	while(i < n) {
		sprintf(filename,"%s/%s",dir,img_dir[i]->d_name);
		printf("Filename: %s\n", filename);
		img = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		if(img.empty()) {
			printf("Can't read image\n");
			return 0;
		}
		des = computeDescriptors(img); //call function here
		fs<<"vid1"<<des;
		i++;
	}
}
