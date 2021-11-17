#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat src=imread("image1.jpg");
	namedWindow("input", WINDOW_AUTOSIZE);
	while (true)
	{
		imshow("input", src);
        	Mat dst;
        	flip(src, dst, 1);
		imshow("output", dst);
		if (waitKey(1) == 81)
			break;
	}
	return 0;
}
