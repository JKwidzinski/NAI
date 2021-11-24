#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const int max_value_hue = 360/2;
const int max_value = 255;

int low_hue = 0, low_saturation = 0, low_value = 0;
int high_hue = max_value_hue;
int high_saturation = max_value, high_value = max_value;

static void on_low_hue_trackbar(int, void *)
{   
    low_hue = min(high_hue-1, low_hue);
    setTrackbarPos("Low Hue", "HSV_sliders", low_hue);
}

static void on_high_hue_trackbar(int, void *)
{
    high_hue = max(high_hue, low_hue+1);
    setTrackbarPos("High Hue", "HSV_sliders", high_hue);
}

static void on_low_saturation_trackbar(int, void *)
{
    low_saturation = min(high_saturation-1, low_saturation);
    setTrackbarPos("Low Saturation", "HSV_sliders", low_saturation);
}

static void on_high_saturation_trackbar(int, void *)
{
    high_saturation = max(high_saturation, low_saturation+1);
    setTrackbarPos("High Saturation", "HSV_sliders", high_saturation);
}

static void on_low_value_trackbar(int, void *)
{
    low_value = min(high_value-1, low_value);
    setTrackbarPos("Low Value", "HSV_sliders", low_value);
}

static void on_high_value_trackbar(int, void *)
{
    high_value = max(high_value, low_value+1);
    setTrackbarPos("High Value", "HSV_sliders", high_value);
}

int main( int argc, char** argv ) {
    int width = 320;
    int height = 200;
    if(argc==3)
    {
        width = stoi(argv[1]);
        height = stoi(argv[2]);
    }
    
    VideoCapture cap("vid.mp4");
    if ( !cap.isOpened() ) {
        cerr << "error opening frames source" << endl;
        return -1;
    }
    cout << "Video size: " << cap.get( CAP_PROP_FRAME_WIDTH ) 
    << "x" << cap.get( CAP_PROP_FRAME_HEIGHT ) << endl;ss

    namedWindow("HSV sliders", WINDOW_AUTOSIZE);
    createTrackbar("Low Hue", "HSV sliders", &low_hue, max_value_hue, on_low_hue_trackbar);
    createTrackbar("High Hue", "HSV sliders", &high_hue, max_value_hue, on_high_hue_trackbar);
    createTrackbar("Low Saturation", "HSV sliders", &low_saturation, max_value, on_low_saturation_trackbar);
    createTrackbar("High Saturation", "HSV sliders", &high_saturation, max_value, on_high_saturation_trackbar);
    createTrackbar("Low Value", "HSV sliders", &low_value, max_value, on_low_value_trackbar);
    createTrackbar("High Value", "HSV sliders", &high_value, max_value, on_high_value_trackbar);

    Mat image_blurred;
    Mat inRange_image;
    while(true){
        Mat frame;
        if ( cap.read( frame ) ) {
            resize(frame, frame, Size(width, height), INTER_LINEAR);
            GaussianBlur(frame, image_blurred, Size(10, 10), 0);
            cvtColor(image_blurred, image_blurred, COLOR_BGR2HSV);
            inRange(image_blurred, Scalar(low_hue, low_saturation, low_value), Scalar(high_hue, high_saturation, high_value), inRange_image);
            imshow("HSV", image_blurred);
            imshow("inRange", inRange_image);
            if ((waitKey( 1000.0/60.0 )&0x0ff) == 120)
            {
	            Rect2d rec_roi = selectROI(inRange_image);
	            Mat img = inRange_image(rec_roi);
                imwrite("../image.jpg", img);
            }
        } else {
            break;
        }
        if( (waitKey( 1000.0/60.0 )&0x0ff) == 113) 
        {
            break;
        }
    }
    return 0;
}
