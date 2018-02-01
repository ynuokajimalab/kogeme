#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*Mat—p*/
#define MatR(IMG,x,y) (IMG).data[ (y) *(IMG).step + (x) *(IMG).elemSize()]
#define MatG(IMG,x,y) (IMG).data[ (y) *(IMG).step + (x) *(IMG).elemSize()+1]
#define MatB(IMG,x,y) (IMG).data[ (y) *(IMG).step + (x) *(IMG).elemSize()+2]
#define MatI(IMG,x,y) (IMG).data[ (y) *(IMG).step + (x) *(IMG).elemSize()+3]
#define MatGray(IMG,x,y) (IMG).data[ (y) *(IMG).step + (x) *(IMG).elemSize()]
#define NOW 1
#define PRE 0