#include "stdafx.h"

#include "targetver.h"
#include "GeneralHeader.h"
#include "WindowProperty.h"
#include "ColorSpaceChange2.h"
#include "Linker.h"
#include "Binarization.h"

#define InputName "Input"
#define OutputName "Output"
#define PropertyWindowName "FireProperty"

#define X_MAX 100
#define Y_MAX 50
#define Z_MAX 100

#define Alpha 1.0

using namespace ColorSpaceChange;

//WindowProperty InputWindow(InputName), MainWindow(OutputName);

//void init_camera(); /*カメラを起動*/
//void binarization(); /*HSV色空間における二値化処理*/
//void RegionJudge(); /*二値化処理されているかどうかをピクセルごとに判定*/

VideoCapture cap(1);
Mat frame, src;
Mat xyz_x, xyz_y, xyz_z, xyz_xy, result;
bool flag[640][480]; /*二値化処理後の白黒の判定結果*/
int x, y;

int main(int argc, char* argv[])
{
	Mat image, image_Lab, image_gray;
	Mat region;
	Mat final;
	image = imread("silent.JPG");

	double lum, lum_mean;
	int count, sum, dst;
	
	region = binarization_lab(image);

	cvtColor(image, image_gray, CV_BGR2GRAY);

	vector<Mat> Lab_channels;

	sum = 0;
	count = 0;

	for (int y = 0; y < region.rows; y++)
	{
		for (int x = 0; x < region.cols; x++)
		{
			dst = region.at<uchar>(y, x);

			if (dst > 0)
			{
				sum += image_gray.at<uchar>(y, x);
				count++;
			}

		}
	}

	lum_mean = (double)sum / (double)count;
	image_Lab = BGR2Lab(image, calibdata, 1);

	split(image_Lab, Lab_channels);

	for (y = 0; y < image.rows; y++)
	{
		for (x = 0; x < image.cols; x++)
		{
			dst = region.at<uchar>(y, x);

			if (dst > 0)
			{
				Lab_channels[0].at<float>(y, x) += Alpha * 0.3749 * (62.1864 - Lab_channels[0].at<float>(y, x)) * (image_gray.at<uchar>(y, x) / lum_mean);
				Lab_channels[1].at<float>(y, x) += Alpha * 1.3305 * (17.2450 - Lab_channels[1].at<float>(y, x)) * (image_gray.at<uchar>(y, x) / lum_mean);
				Lab_channels[2].at<float>(y, x) += Alpha * 1.0588 * (29.9626 - Lab_channels[2].at<float>(y, x)) * (image_gray.at<uchar>(y, x) / lum_mean);
			}
		}
	}

	merge(Lab_channels, result);
	final = Lab2BGR(result, calibdata, 1);

	while (1)
	{
		imshow("result", final);
		if (waitKey(1) == 27)
			break;
	}
	

	return 0;
}

//void init_camera()
//{
//	namedWindow("capture", CV_WINDOW_AUTOSIZE);
//
//	while (1)
//	{
//		cap >> frame;
//		src = frame;
//		imshow("capture", src);
//		imwrite("org_capture3.jpg", src);
//		binarization();
//		/*RegionJudge();*/
//		if (waitKey(1) == 27)
//			break;
//	}
//}

//void binarization()
//{
//	vector<Mat> channels_xyz;
//
//	double X, Y, Z;
//	Cal calibdata("20170615.csv");
//	Mat xyz = BGR2XYZ(src, calibdata);
//	imshow("Lab", src);
//	imwrite("Lab3.jpg", src);
//	xyz.convertTo(xyz, CV_32F);
//	medianBlur(src, src, 3); /*メディアンフィルタ*/
//	morphologyEx(src, src, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
//	morphologyEx(src, src, MORPH_ERODE, Mat(), Point(-1, -1), 1);
//	xyz.convertTo(xyz, CV_8UC1);
//	split(src, channels_xyz); /*HSVチャンネル分割*/
//	
//	X = threshold(channels_xyz[0], xyz_x, X_MAX, 255, THRESH_BINARY);
//	Y = threshold(channels_xyz[1], xyz_y, Y_MAX, 255, THRESH_BINARY);
//	Z = threshold(channels_xyz[2], xyz_z, Z_MAX, 255, THRESH_BINARY);
//
//	/*cout << "L=" << X << endl;
//	cout << "a=" << Y << endl;
//	cout << "b=" << Z << endl;
//	cout << endl;*/
//
//	bitwise_and(xyz_x, xyz_y, xyz_xy);
//	bitwise_and(xyz_xy, xyz_z, result);
//
//	morphologyEx(result, result, MORPH_DILATE, Mat(), Point(-1, -1), 1); /*モルフォロジーフィルタ(close)*/
//	morphologyEx(result, result, MORPH_ERODE, Mat(), Point(-1, -1), 1);
//
//
//	imshow("binarization", result);
//	imwrite("result3.jpg", result);
//}

//void RegionJudge()
//{
//	int xsize, ysize; /*カメラの解像度*/
//	int x, y; /*作業用変数*/
//	xsize = result.size().width;
//	ysize = result.size().height;
//	for (x = 0; x < xsize; x++)
//	{
//		for (y = 0; y < ysize; y++)
//		{
//			if ((MatH(result, x, y) == 0) && (MatS(result, x, y) == 0) && (MatV(result, x, y) == 0))
//				flag[x][y] = 1;
//			else
//				flag[x][y] = 0;
//		}
//	}
//
//}