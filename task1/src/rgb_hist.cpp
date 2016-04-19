#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;


vector<cv::Mat> SplitImage(cv::Mat inputImage);
int main() {

	Mat img = imread("../data/frames/shot1_009.jpg");

	Mat hist;
	//calcHist(&img, imgCount, channels, mask, hist, dims, sizes, ranges);
	std::vector<cv::Mat> rgbChannels(3);
    	std::vector<cv::Mat> splittedImage(3);//,cv::Mat(img.size(),CV_64FC1));
	//splittedImage = SplitImage(img);
	split(img, splittedImage);
	
	imshow("window", splittedImage[0]);
	imshow("window1", splittedImage[1]);
	imshow("window2", splittedImage[2]);
	waitKey(0);
	cout<<"Size: = "<<splittedImage[0].cols<<endl;
	return 0;

}
vector<cv::Mat> SplitImage(cv::Mat inputImage)
{
    //copy original in BGR order
    std::vector<cv::Mat> splittedImage(3,cv::Mat(inputImage.size(),CV_64FC1));
    std::vector<cv::Mat>::iterator it;
    it = splittedImage.begin();

    for(int channelNo = 0; channelNo < inputImage.channels(); channelNo++)
    {
        cv::Mat tempImage(inputImage.size(),CV_64FC1);
        for (int row = 0; row < inputImage.size().height; row++)
        {
            for (int col = 0; col < inputImage.size().width; col++)
            {
    tempImage.at<double>(row, col) = inputImage.at<cv::Vec3d>(row, col)[channelNo];
            }
        }

  it = splittedImage.insert ( it , tempImage );     
  it++;
    }
    return splittedImage;
}
