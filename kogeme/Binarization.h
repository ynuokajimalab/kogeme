#pragma once

#include "stdafx.h"

#include "targetver.h"
#include "GeneralHeader.h"
#include "ColorSpaceChange2.h"
#include "Linker.h"

using namespace ColorSpaceChange;

Cal calibdata("calibdata_experiment.csv");

Mat binarization_lab(Mat image)
{
	double L, a, b;
	Mat Lab_L, Lab_a, Lab_b, Lab_La, result, Lab;
	vector<Mat> channels_Lab;

	//Lab = BGR2Lab(image, calibdata, 1);
	cvtColor(image, Lab, CV_BGR2Lab);
	//imshow("lab", Lab);
	//imwrite("lab.jpg", Lab);
	Lab.convertTo(Lab, CV_32F);
	medianBlur(Lab, Lab, 3); /*メディアンフィルタ*/
	morphologyEx(Lab, Lab, MORPH_ERODE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(Lab, Lab, MORPH_DILATE, Mat(), Point(-1, -1), 1);
	Lab.convertTo(Lab, CV_8UC1);
	split(Lab, channels_Lab); /*Labチャンネル分割*/

	L = threshold(channels_Lab[0], Lab_L, 0, 255, THRESH_BINARY | THRESH_OTSU);
	a = threshold(channels_Lab[1], Lab_a, 0, 255, THRESH_BINARY | THRESH_OTSU);
	b = threshold(channels_Lab[2], Lab_b, 0, 255, THRESH_BINARY | THRESH_OTSU);

	/*cout << "L=" << L << endl;
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
	cout << endl;*/

	bitwise_and(Lab_L, Lab_a, Lab_La);
	bitwise_and(Lab_La, Lab_b, result);

	morphologyEx(result, result, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(result, result, MORPH_ERODE, Mat(), Point(-1, -1), 1);

	//imshow("binarization", result);

	/*while (1)
	{
	imshow("binarization", result);
	if (waitKey(1) == 27)
	break;
	}*/
	//imwrite("binarization_Lab3.jpg", result);

	return result;
}

Mat binarization_rgb(Mat image)
{
	double r, g, b;
	Mat rgb_r, rgb_g, rgb_b, rgb_rg, result, Lab;
	vector<Mat> channels_rgb;

	image.convertTo(image, CV_32F);
	medianBlur(image, image, 3); /*メディアンフィルタ*/
	morphologyEx(image, image, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(image, image, MORPH_ERODE, Mat(), Point(-1, -1), 1);
	image.convertTo(image, CV_8UC1);
	split(image, channels_rgb); /*Labチャンネル分割*/

	r = threshold(channels_rgb[0], rgb_r, 0, 255, THRESH_BINARY | THRESH_OTSU);
	g = threshold(channels_rgb[1], rgb_g, 0, 255, THRESH_BINARY | THRESH_OTSU);
	b = threshold(channels_rgb[2], rgb_b, 0, 255, THRESH_BINARY | THRESH_OTSU);

	cout << "R=" << r << endl;
	cout << "G=" << g << endl;
	cout << "B=" << b << endl;
	cout << endl;

	bitwise_and(rgb_r, rgb_g, rgb_rg);
	bitwise_and(rgb_rg, rgb_b, result);

	morphologyEx(result, result, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(result, result, MORPH_ERODE, Mat(), Point(-1, -1), 1);

	imshow("binarization", result);
	imwrite("binarization_rgb3.jpg", result);

	return result;
}

//Mat binarization_hsv(Mat image)
//{
//	double h, s, v;
//	Mat hsv, hsv_h, hsv_s, hsv_v, hsv_hs, result;
//	vector<Mat> channels_hsv;
//	//cvtColor(image, hsv, CV_BGR2HSV);
//
//	hsv = BGR2HSV(image, calibdata);
//	//imshow("hsv", image);
//	//imwrite("hsv.jpg", image);
//	hsv.convertTo(hsv, CV_32F);
//	medianBlur(hsv, hsv, 3); /*メディアンフィルタ*/
//	morphologyEx(hsv, hsv, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
//	morphologyEx(hsv, hsv, MORPH_ERODE, Mat(), Point(-1, -1), 1);
//	hsv.convertTo(hsv, CV_8UC1);
//	split(hsv, channels_hsv); /*Labチャンネル分割*/
//
//	h = threshold(channels_hsv[0], hsv_h, 0, 255, THRESH_BINARY | THRESH_OTSU);
//	s = threshold(channels_hsv[1], hsv_s, 0, 255, THRESH_BINARY | THRESH_OTSU);
//	v = threshold(channels_hsv[2], hsv_v, 0, 255, THRESH_BINARY | THRESH_OTSU);
//
//	/*cout << "H=" << h << endl;
//	cout << "S=" << s << endl;
//	cout << "V=" << v << endl;
//	cout << endl;*/
//
//	bitwise_and(hsv_h, hsv_s, hsv_hs);
//	bitwise_and(hsv_hs, hsv_v, result);
//
//	morphologyEx(result, result, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
//	morphologyEx(result, result, MORPH_ERODE, Mat(), Point(-1, -1), 1);
//	imshow("binarization", result);
//	/*while (1)
//	{
//	imshow("binarization", result);
//	if (waitKey(1) == 27)
//	break;
//	}*/
//	//imwrite("binarization_hsv.jpg", result);
//
//	return result;
//}

Mat binarization_ycrcb(Mat image)
{
	double y;
	Mat ycrcb, result;
	vector<Mat> channels_ycrcb;

	ycrcb = BGR2Y(image, calibdata);
	/*imshow("lab", ycrcb);
	imwrite("YCrCb.jpg", ycrcb);*/
	ycrcb.convertTo(ycrcb, CV_32F);
	medianBlur(ycrcb, ycrcb, 3); /*メディアンフィルタ*/
	morphologyEx(ycrcb, ycrcb, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(ycrcb, ycrcb, MORPH_ERODE, Mat(), Point(-1, -1), 1);
	ycrcb.convertTo(ycrcb, CV_8UC3);
	//split(ycrcb, channels_ycrcb);

	//imshow("Y", channels_ycrcb[0]);
	//imshow("Cr", channels_ycrcb[1]);
	//imshow("Cb", channels_ycrcb[2]);

	y = threshold(ycrcb, ycrcb, 0, 255, THRESH_BINARY | THRESH_OTSU);

	cout << "Y=" << y << endl;

	morphologyEx(ycrcb, ycrcb, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(ycrcb, result, MORPH_ERODE, Mat(), Point(-1, -1), 1);

	//while (1)
	//{
	//	imshow("binarization", result);
	//	if (waitKey(1) == 27)
	//		break;
	//}
	/*imwrite("binarization_ycrcb3.jpg", result);*/

	return result;
}

Mat binarization_xyz(Mat image)
{
	double x, y, z;
	Mat xyz, xyz_x, xyz_y, xyz_z, xyz_xy, result;
	vector<Mat> channels_xyz;

	xyz = BGR2XYZ(image, calibdata);
	xyz.convertTo(xyz, CV_32F);
	medianBlur(xyz, xyz, 3); /*メディアンフィルタ*/
	morphologyEx(xyz, xyz, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(xyz, xyz, MORPH_ERODE, Mat(), Point(-1, -1), 1);
	xyz.convertTo(xyz, CV_8UC1);
	split(xyz, channels_xyz); /*Labチャンネル分割*/

	x = threshold(channels_xyz[0], xyz_x, 0, 255, THRESH_BINARY | THRESH_OTSU);
	y = threshold(channels_xyz[1], xyz_y, 0, 255, THRESH_BINARY | THRESH_OTSU);
	z = threshold(channels_xyz[2], xyz_z, 0, 255, THRESH_BINARY | THRESH_OTSU);

	/*cout << "X=" << x << endl;
	cout << "Y=" << y << endl;
	cout << "Z=" << z << endl;
	cout << endl;*/

	bitwise_and(xyz_x, xyz_y, xyz_xy);
	bitwise_and(xyz_xy, xyz_z, result);

	morphologyEx(result, result, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
	morphologyEx(result, result, MORPH_ERODE, Mat(), Point(-1, -1), 1);

	/*imshow("binarization", result);*/
	/*imwrite("binarization_xyz3.jpg", result);*/

	cvtColor(result, result, COLOR_GRAY2BGR);
	result = XYZ2BGR(result, calibdata);

	return result;
}
