#pragma once
#include "GeneralHeader.h"

class WindowProperty
{
public:
	WindowProperty(string name);
	~WindowProperty();

	void imageshow(Mat image); /*�摜��\��*/								
	void ChangeSize(bool flag); /*�E�B���h�E�̃T�C�Y��ύX*/
private:
	string WindowName; /*�E�B���h�E��*/
	Size fullscreensize; /*�S��ʃT�C�Y*/
	Size defaultsize, windowsize;
};
