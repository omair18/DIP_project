#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char **av) {
	std::string arg = av[1];
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened())
    {
        cerr << "Failed to open a video device or video file!\n" << endl;
        return 1;
    }

    int fps = capture.get(CV_CAP_PROP_FRAME_COUNT);
    printf("fps  = %d\n",fps);

	return 0;
}
