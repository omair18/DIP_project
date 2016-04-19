#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv/cv.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

struct dirent **img_dir;
//char dir[] = "/windows/D/MegaDrive/Study/LUMS/Spring16/Vision_DIP/DIP_CS674/project/data/frames";
char dir[] = "../data/frames";
#define BIN_SIZE 256

Mat calculateHistogram(Mat img);
void drawHistrogram(vector<Mat> hists);
int main() {
	printf("Reading directory\n");

	int n = scandir(dir, &img_dir, 0, alphasort);
	int i=2;
	char filename[200]; 
	Mat img;	
	Mat hists;
	while(i < n) {
		sprintf(filename,"%s/%s",dir,img_dir[i]->d_name);
		printf("Filename: %s\n", filename);
		img = imread(filename);
		if(img.empty()) {
			printf("Can't read image\n");
			return 0;
		}
		Mat hist_img = calculateHistogram(img);	 // returns histogram
		if(i == 2) //first image
		{
			hconcat(hist_img,hists);
		}
		else {
			hconcat(hists,hist_img,hists);
		//cout<<hists.rows<<'\t'<<hists.cols<<endl;
		}
		printf("Final hists cols = %d \n",hists.cols);
		i++;
	}
	FileStorage fs("../data/descriptors.yml",FileStorage::APPEND);
	cout<<img_dir[i]->d_name<<endl;
	string sc = string(img_dir[i-1]->d_name).substr(0,4);
	cout<<sc<<endl;
	fs<<sc<<hists;
	printf("histogram size: rows = %d\t cols=%d\n",hists.rows, hists.cols);
	double res = compareHist(hists,hists,0);
	printf(" res = %f\n",res);
 
}
Mat calculateHistogram(Mat img) {
	vector<Mat> bgr_planes;
	vector<Mat> histograms;
	split(img, bgr_planes);
	
	int h = img.rows;
	int w = img.cols;
	int cx = w*0.5;
	int cy = h*0.5;
	int axesX = (w*0.75)/2;
	int axesY = (h*0.75)/2;

	int segments[4][4] = {{0,cx,0,cy}, {cx,w,0,cy}, {cx,w,cy,h}, {0,cx,cy,h}}; //4 segments, 
	Mat seg;

	seg = Mat(4,4, CV_32SC1, segments); //segment
	Mat ellipseMask;
	Mat overLapMask = Mat::zeros(h,w,CV_8UC1);	

	Mat cornerMask1 = Mat::zeros(h,w,CV_8UC1);	 // zeros of size rows x cols
	Mat cornerMask2 = Mat::zeros(h,w,CV_8UC1);	 // zeros of size rows x cols
	Mat cornerMask3 = Mat::zeros(h,w,CV_8UC1);	 // zeros of size rows x cols
	Mat cornerMask4 = Mat::zeros(h,w,CV_8UC1);	 // zeros of size rows x cols

	rectangle(cornerMask1,Point(0,cx),Point(0,cy),255,-1); //rect1
	rectangle(cornerMask2,Point(cx,w),Point(0,cy),255,-1); //rect2
	rectangle(cornerMask3,Point(cx,w),Point(cy,h),255,-1); //rect3
	rectangle(cornerMask4,Point(0,cx),Point(cy,h),255,-1); //rect4

	rectangle(overLapMask,Point(cx/2,cy/2),Point(cx+(cx/2), (cy+(cy/2))), 255,-1);
	
	/*cout<<"overLapMask = " << overLapMask.rows <<" " <<overLapMask.cols<<endl;
	cout<<"image = " << img.rows << " " << img.cols<<endl;
	return Mat();	 */
	Mat hist_of_three_channels;
	for(int channels =0; channels < 3; channels++) {
		int histSize = BIN_SIZE;
		float range[] = {0, 256};
		const float* histRange = {range};
		bool uniform = true; bool accumulate = false;
		Mat hist_of_one_channel;

		Mat ch_hist1;
		calcHist(&bgr_planes[channels], 1, 0, cornerMask1, ch_hist1, 1, &histSize, &histRange, uniform, accumulate); //histogram of rect1
		transpose(ch_hist1, ch_hist1);
		printf("hist cols = %d\n",ch_hist1.cols);

		Mat ch_hist2;
		calcHist(&bgr_planes[channels], 1, 0, cornerMask2, ch_hist2, 1, &histSize, &histRange, uniform, accumulate); //histogram of rect2
		transpose(ch_hist2, ch_hist2);
		hconcat(ch_hist1,ch_hist2,hist_of_one_channel);
		printf("hist cols = %d\n",hist_of_one_channel.cols);


		Mat ch_hist3;
		calcHist(&bgr_planes[channels], 1, 0, cornerMask3, ch_hist3, 1, &histSize, &histRange, uniform, accumulate); //histogram of rect3
		transpose(ch_hist3, ch_hist3);
		hconcat(hist_of_one_channel,ch_hist3,hist_of_one_channel);
		
		printf("hist cols = %d\n",hist_of_one_channel.cols);

		Mat ch_hist4;
		calcHist(&bgr_planes[channels], 1, 0, cornerMask1, ch_hist4, 1, &histSize, &histRange, uniform, accumulate); //histogram of rect4
		transpose(ch_hist4, ch_hist4);
		hconcat(hist_of_one_channel,ch_hist4,hist_of_one_channel);

		printf("hist cols = %d\n",hist_of_one_channel.cols);

		Mat ch_hist_overlap; //to save histogram of the ellipse region we have already made in this function
		calcHist(&bgr_planes[channels], 1, 0, overLapMask, ch_hist_overlap, 1, &histSize, &histRange, uniform, accumulate); //histogram of rect4
		transpose(ch_hist_overlap, ch_hist_overlap); //transpose
		hconcat(hist_of_one_channel,ch_hist_overlap,hist_of_one_channel);

		printf("hist cols = %d\n",hist_of_one_channel.cols);

		if(channels ==0 ) {
			hconcat(hist_of_one_channel, hist_of_three_channels);
			printf("hist 3channel cols = %d\n",hist_of_three_channels.cols);
		}
		else  {
			hconcat(hist_of_three_channels, hist_of_one_channel, hist_of_three_channels); //concatinate 4*NUM_BINS of one channel into Matrix
			printf("hist 3channel cols = %d\n",hist_of_three_channels.cols);
		}


	}	
	return hist_of_three_channels; 
}
void drawHistrogram(vector<Mat> hists) {
	
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/256 );

  	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
	for (int i=0; i<256; i++) {
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hists[0].at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hists[0].at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hists[1].at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hists[1].at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hists[2].at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hists[2].at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
	}

	 namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	 imshow("calcHist Demo", histImage );
	 waitKey(0);
}

