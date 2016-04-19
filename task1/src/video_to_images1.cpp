#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/videoio/videoio.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>

using namespace cv;
using namespace std;

#define TOTALFRAMES 12
//hide the local functions in an anon namespace
namespace
{ 

    int process(VideoCapture& capture, std::string arg)
    {
	int n=0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
	int waste_time=0;
        Mat frame;
	double frames_per_sec = capture.get(CAP_PROP_FPS);
	int frame_diff = frames_per_sec*15; //get frames after 15secs 
	int next_frame = 0;
	printf("total frames = %f\n",capture.get(CAP_PROP_FRAME_COUNT));
	printf("frames per sec = %f\n", capture.get(CAP_PROP_FPS));
	/*capture >> frame;
	sprintf(filename, "%s_%.3d.jpg", arg.c_str(), n++); */
	//imwrite(filename, frame);
        for (int i=0; i< TOTALFRAMES; i++)
        {
		if(i != 0) {
			next_frame = next_frame + frame_diff;
			printf("next_frame = %d\n",next_frame);
			capture.set(CAP_PROP_POS_FRAMES, next_frame);
		}
		capture >> frame;
		sprintf(filename, "%s_%.3d.jpg", arg.c_str(), n++);
		printf("filename = %s\n",filename);
		imshow(window_name, frame);
		cv::waitKey(5); 
		

		imwrite(filename, frame);	
		/*waste_time++;
		if(waste_time > 500) { //nearly 2sec wait 
			waste_time = 0;
			if (frame.empty())
				break;	
			else {
				imshow(window_name, frame);
				cv::waitKey(5);
				printf(">>>>>> waste_time = %d\n",waste_time);
				//imwrite(filename, frame);
			}
		} */
       	}
	return 0;
    }
}

int main(int ac, char** av)
{

    if (ac != 2)
    {
	    printf("usage = %s <device> \n",av[1]);
	    return 1;
    }
    std::string arg = av[1];
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened())
    {
        cerr << "Failed to open a video device or video file!\n" << endl;
        return 1;
    }
    return process(capture, arg);
}
