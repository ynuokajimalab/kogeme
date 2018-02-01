#include "stdafx.h"

#include "WindowProperty.h"

#define MainWindowWidth 1600
#define HMDWidth 1600
#define HMDHeight 900


WindowProperty::WindowProperty(string name)
{

	fullscreensize = Size(1600 + 20, 900); /*ウィンドウのフルスクリーンサイズの定義*/
	defaultsize = Size(400, 225); /*ウィンドウのデフォルトサイズの定義*/

	WindowName = name;
	namedWindow(WindowName, CV_WINDOW_AUTOSIZE); /*ウィンドウの表示*/
	moveWindow(WindowName, 0, 0); /*ウィンドウを原点に移動*/
	windowsize = defaultsize;
}

WindowProperty::~WindowProperty()
{
}

void WindowProperty::imageshow(Mat image) {
	resize(image, image, windowsize); /*ウィンドウサイズの設定*/
	imshow(WindowName, image); /*画像をウィンドウに表示*/
}

void WindowProperty::ChangeSize(bool flag) {
	destroyWindow(WindowName); /*ウィンドウの破棄*/
	namedWindow(WindowName, CV_WINDOW_NORMAL); /*標準サイズウィンドウの表示*/

	if (flag) {

		setWindowProperty(WindowName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN); /*ウィンドウをフルスクリーンサイズで表示*/
		moveWindow(WindowName, MainWindowWidth - 20, 0);

		windowsize = fullscreensize; /*ウィンドウをフルスクリーンサイズにする*/
		flag = true;
	}
	else {
		setWindowProperty(WindowName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_NORMAL);
		moveWindow(WindowName, 0, 0); /*ウィンドウを原点に移動する*/

		windowsize = defaultsize; /*ウィンドウをデフォルトサイズにする*/
		flag = false;
	}

	resizeWindow(WindowName, windowsize.width, windowsize.height); /*ウィンドウサイズをflagに従って変更する*/

}