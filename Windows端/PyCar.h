#pragma once
#include "Python.h"
#include <numpy/arrayobject.h>	
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


class PyCar
{
public:
	int star();
	~PyCar();
	int findcar(Mat img);

public:
	PyObject *moduleName ;  
	PyObject *pModule ;
	int *a;
	PyObject *pFunc;
	PyObject *pReturn;

};

