#include <string>
#include <stdio.h>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv.h>
#include <highgui.h>
using namespace cv;
using namespace std;




void hough(char name[20])
{
	Mat pic,src, src_gray;

	/// Read the image

	src = imread(name, 0);
	pic = imread(name,1);

	///binary src

	threshold(src, src_gray, 180, 255, THRESH_BINARY_INV);
	
	///loop for clean other side
	
	for (int i = 0;i<src_gray.rows;i++) {
		for (int j = 0;j<src_gray.cols; j++) {
			if (j<490||j>900) {
				src_gray.at<uchar>(i, j) = 255;
			}
			else if (i < 460||i>800) {
				src_gray.at<uchar>(i, j) = 255;
			}		
		}
	}


	///loop for dilate
	/*
	for (int j = 0;j<4;j++) {
		Mat dilateStruct = getStructuringElement(MORPH_CROSS, Size(2, 2));
		dilate(src_gray, src_gray, dilateStruct);
	}
	for (int k=0;k<4;k++) {
		Mat erodeStruct = getStructuringElement(MORPH_RECT, Size(2, 2));
		erode(src_gray, src_gray, erodeStruct);
	}
	*/


	vector<Vec3f> circles;
	/// Hough find the circles
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT,1.6, src_gray.rows / 5, 18, 30, 60, 100);

	/// Draw the circles 
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(pic, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(pic, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	/// Show results
	namedWindow("Hough Circle Transform Demo", WINDOW_NORMAL);
	imshow("Hough Circle Transform Demo", pic);
	namedWindow("Circle", WINDOW_NORMAL);
	imshow("Circle", src_gray);
	waitKey(0);
	
}









int main()
{
	int i=0, j, k;
	
	int day=26, hr=0, min=0;
	char file[20];
	
	///read pictures
	
	for (i = 0;i<24;i++) {
	
		hr = i;
		min = 0;
		sprintf(file, "201609%02d%02d%02d.jpg", day, hr, min);
		hough(file);


		min = 30;
		sprintf(file, "201609%02d%02d%02d.jpg", day, hr, min);
		hough(file);
			
	}
	return 0;
}
