#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	VideoCapture cap("video.mp4");
    if(!cap.isOpened()){
        return -1;  
    }   
	namedWindow("output", WINDOW_AUTOSIZE);
	while (true)
	{
        Mat frame;
        cap >> frame;
        if(frame.empty()){
            break;
        }
        flip(frame, frame, 1);
		imshow("output", frame);
		if (waitKey(1) == 81)
			break;
	}
    cap.release();
    destroyAllWindows();
	return 0;
}