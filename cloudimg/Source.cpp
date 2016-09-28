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


void table(char name[20]) {
	
		IplImage *src_image = 0, *dst_image = 0;
		CvMat *lut_mat;
		lut_mat = cvCreateMatHeader(1, 256, CV_8UC1);
		uchar lut[256];
		for (int i = 0; i < 256; i++) {
			lut[i] = 255 - i;
		}
		src_image = cvLoadImage(name);
		dst_image = cvCloneImage(src_image);
		cvSetData(lut_mat, lut, 0);
		cvLUT(src_image, dst_image, lut_mat);
		cvNamedWindow("src_image", 2);
		cvNamedWindow("dst_image", 2);
		cvShowImage("src_image", src_image);
		cvShowImage("dst_image", dst_image);
		cvWaitKey(0);
		cvReleaseImage(&src_image);
		cvReleaseImage(&dst_image);
		
		
}

void find(char name[20])
{
	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	Mat src = imread(name); // Load source image
	Mat thr(src.rows, src.cols, CV_8UC1);
	Mat dst(src.rows, src.cols, CV_8UC3, Scalar::all(0));
	cvtColor(src, thr, CV_BGR2GRAY); // Convert to gray
	threshold(thr, thr, 25, 255, THRESH_BINARY); // Threshold the gray

	vector<vector<Point>> contours; // Vector for storing contour
	vector<Vec4i> hierarchy;

	findContours(thr, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

	Scalar color(0, 0, 255);

	for (int i = 0; i < contours.size(); i++) // Iterate through each contour
	{
		double a = contourArea(contours[i], false); // Find the area of contour
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = i; // Store the index of largest contour
			bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
		}

	}
	for (int i = 0; i < contours.size(); i++) // Iterate through each contour
	{
		if (i != largest_contour_index) {
			drawContours(dst, contours, i, color, CV_FILLED, 8, hierarchy);
		}
	}

	Scalar color2(255, 0, 0);
	drawContours(dst, contours, largest_contour_index, color2, 5, 8, hierarchy); // Draw the largest contour using previously stored index.

	rectangle(src, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
	
	imshow("Happy Thread", thr);
	imshow("Happy Source", src);
	imshow("Happy Largest Contour", dst);

	// Wait for a keystroke in the window
	waitKey(0);
}

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


	///src erode OR dilate code
	/*
	Mat erodeStruct = getStructuringElement(MORPH_RECT, Size(2, 2)); 
	erode(src_gray, src_gray, erodeStruct);
	Mat dilateStruct = getStructuringElement(MORPH_CROSS, Size(6, 6));
	dilate(src_gray, src_gray, dilateStruct);
	GaussianBlur(src_gray, src_gray, Size(9, 9), 15, 15);
	
	GaussianBlur(src_gray, src_gray, Size(9, 9), 15, 15);
	*/
	
	vector<Vec3f> circles;
	
	/// Apply the Hough Transform to find the circles
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT,1.6, src_gray.rows / 5, 18, 30, 60, 100);

	/// Draw the circles detected
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
	int x, y;
	int day=26, hr=0, min=0;
	char file[20];
	Mat pic;
	Mat dst;
	

	for (i = 0;i<24;i++) {
	
		hr = i;
		min = 0;
		sprintf(file, "201609%02d%02d%02d.jpg", day, hr, min);
		hough(file);

//		pic = imread(file, CV_LOAD_IMAGE_GRAYSCALE);
//		threshold(pic, dst, 180, 255, THRESH_BINARY);
		
		
//		namedWindow("threshold", WINDOW_NORMAL);
		
//		imshow("origin", pic);
//		imshow("threshold", dst);
//		waitKey(0);




		min = 30;
		sprintf(file, "201609%02d%02d%02d.jpg", day, hr, min);
		hough(file);
		
//		pic = imread(file, CV_LOAD_IMAGE_GRAYSCALE);
//		threshold(pic, dst, 180, 255, THRESH_BINARY);
		
		

//		namedWindow("threshold", WINDOW_NORMAL);
//		imshow("origin", pic);
//		imshow("threshold", dst);
//		waitKey(0);


	
		
	}
	
	
	
	return 0;
}