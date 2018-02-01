#include "stdafx.h"

#include "WindowProperty.h"

#define MainWindowWidth 1600
#define HMDWidth 1600
#define HMDHeight 900


WindowProperty::WindowProperty(string name)
{

	fullscreensize = Size(1600 + 20, 900); /*�E�B���h�E�̃t���X�N���[���T�C�Y�̒�`*/
	defaultsize = Size(400, 225); /*�E�B���h�E�̃f�t�H���g�T�C�Y�̒�`*/

	WindowName = name;
	namedWindow(WindowName, CV_WINDOW_AUTOSIZE); /*�E�B���h�E�̕\��*/
	moveWindow(WindowName, 0, 0); /*�E�B���h�E�����_�Ɉړ�*/
	windowsize = defaultsize;
}

WindowProperty::~WindowProperty()
{
}

void WindowProperty::imageshow(Mat image) {
	resize(image, image, windowsize); /*�E�B���h�E�T�C�Y�̐ݒ�*/
	imshow(WindowName, image); /*�摜���E�B���h�E�ɕ\��*/
}

void WindowProperty::ChangeSize(bool flag) {
	destroyWindow(WindowName); /*�E�B���h�E�̔j��*/
	namedWindow(WindowName, CV_WINDOW_NORMAL); /*�W���T�C�Y�E�B���h�E�̕\��*/

	if (flag) {

		setWindowProperty(WindowName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN); /*�E�B���h�E���t���X�N���[���T�C�Y�ŕ\��*/
		moveWindow(WindowName, MainWindowWidth - 20, 0);

		windowsize = fullscreensize; /*�E�B���h�E���t���X�N���[���T�C�Y�ɂ���*/
		flag = true;
	}
	else {
		setWindowProperty(WindowName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_NORMAL);
		moveWindow(WindowName, 0, 0); /*�E�B���h�E�����_�Ɉړ�����*/

		windowsize = defaultsize; /*�E�B���h�E���f�t�H���g�T�C�Y�ɂ���*/
		flag = false;
	}

	resizeWindow(WindowName, windowsize.width, windowsize.height); /*�E�B���h�E�T�C�Y��flag�ɏ]���ĕύX����*/

}