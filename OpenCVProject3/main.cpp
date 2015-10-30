#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int minH = 0;
int maxH = 179;

int minS = 0;
int maxS = 255;

int minV = 0;
int maxV = 255;

int erossion = 5;
int dilation = 5;

vector<vector<Point> > contours;

vector<Vec4i> hierarchy;

Mat imgMain;


int main(int, char)
{
	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Main", 1);
	namedWindow("Control", CV_WINDOW_NORMAL);

	cvCreateTrackbar("LowH", "Control", &minH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &maxH, 179);

	cvCreateTrackbar("LowS", "Control", &minS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &maxS, 255);

	cvCreateTrackbar("LowV", "Control", &minV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &maxV, 255);

	cvCreateTrackbar("Dilation", "Control", &dilation, 10); //Value (0 - 255)

	cvCreateTrackbar("Erode", "Control", &erossion, 10);
	while (true)
	{

		bool bSuccess = cap.read(imgMain); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}


		Mat imgContour = Mat::zeros(imgMain.rows, imgMain.cols, CV_8UC3);
		Mat imgHull = Mat::zeros(imgMain.rows, imgMain.cols, CV_8UC3);

	Mat imgHSV;

	cvtColor(imgMain, imgHSV, COLOR_BGR2HSV);

	Mat imgThresholded;

	inRange(imgHSV, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), imgThresholded); //Threshold the image

		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(erossion, erossion)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(erossion, erossion)));
		





		//Countour testing

		findContours(imgThresholded, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		vector<vector<Point> >hull(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			convexHull(Mat(contours[i]), hull[i], false);
		}
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			Scalar color( 0, 255, 0);
			drawContours(imgContour, contours, idx, color, CV_FILLED, 8, hierarchy);
			drawContours(imgHull, hull, idx, color, 1, 8, vector<Vec4i>(), 0, Point());
		}
		
		imshow("Hull", imgHull); //show the original image
		imshow("Contour", imgContour); //show the original image
		imshow("Original", imgMain); //show the original image
		imshow("thresholded", imgThresholded);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
		return 0;
}