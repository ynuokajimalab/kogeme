#pragma once
#include "GeneralHeader.h"

class WindowProperty
{
public:
	WindowProperty(string name);
	~WindowProperty();

	void imageshow(Mat image); /*画像を表示*/								
	void ChangeSize(bool flag); /*ウィンドウのサイズを変更*/
private:
	string WindowName; /*ウィンドウ名*/
	Size fullscreensize; /*全画面サイズ*/
	Size defaultsize, windowsize;
};
