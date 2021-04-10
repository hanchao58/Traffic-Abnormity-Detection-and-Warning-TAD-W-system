#include "PyCar.h"



int PyCar::star()
{
	//Py_SetPythonHome(L"E:\\python\\python-3.6.5");		//设置Python路径
	Py_Initialize();	// 初始化Python
	import_array();		// 使用Numpy的相关函数前必须声明这一行代码

	// 检查初始化是否成功
	if (!Py_IsInitialized())
		return -1;

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");	// .py的路径，这里与cpp放在同一个目录下


	moduleName = PyUnicode_FromString("VD");  //载入名为.py的文件

	pModule = PyImport_Import(moduleName);
	if (!pModule)		//如果不存在改文件，则结束
	{
		printf("can't find py");
		//getchar();		//点击回车关闭窗口
		Py_Finalize();			//关闭python解释器
		return -1;
	}

	pFunc = PyObject_GetAttrString(pModule, "run");  //获取函数
	if (!pFunc)
	{
		printf("can't find function [run]");
		//getchar();
		Py_Finalize();
		return -1;
	}
	pReturn = NULL;
}


PyCar::~PyCar()
{
	// Py_CLEAR(moduleName);
    // Py_CLEAR(pModule);
	Py_XDECREF(moduleName);
	Py_XDECREF(pModule);
	Py_Finalize();	// 关闭Python
	// system("pause");
	// getchar();
	Py_XDECREF(pFunc);		//Py_XDECREF是很有必要的，为了避免内存泄漏
	Py_XDECREF(pReturn);

}


int PyCar::findcar(Mat img)
{

	if (img.empty())
	{
		Py_Finalize();
		return -1;
	}

	// CV::Mat 转 python numpy
	auto sz = img.size();	// 获取图像的尺寸
	int x = sz.width;
	int y = sz.height;
	int z = img.channels();
	uchar *CArrays = new uchar[x*y*z];//这一行申请的内存需要释放指针，否则存在内存泄漏的问题
	int iChannels = img.channels();
	int iRows = img.rows;
	int iCols = img.cols * iChannels;
	if (img.isContinuous())
	{
		iCols *= iRows;
		iRows = 1;
	}
	uchar* p;
	int id = -1;
	for (int i = 0; i < iRows; i++)
	{
		// get the pointer to the ith row
		p = img.ptr<uchar>(i);
		// operates on each pixel
		for (int j = 0; j < iCols; j++)
		{
			CArrays[++id] = p[j];//连续空间
		}
	}
	npy_intp Dims[3] = { y, x, z }; //注意这个维度数据！
	PyObject *ArgList = PyTuple_New(1);  //参数列表:创建一个长度为1的元组
	PyObject *PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);
	PyTuple_SetItem(ArgList, 0, PyArray); //将PyArray的引用指向元组ArgList的第0个元素
	//PyObject *pFunc = PyObject_GetAttrString(pModule, "run");  //获取函数
	//if (!pFunc)
	//{
	//	printf("can't find function [run]");
	//	//getchar();
	//	Py_Finalize();
	//	return -1;
	//}
	pReturn = PyObject_CallObject(pFunc, ArgList);
	if (pReturn == NULL)
	{
		Py_Finalize();
		return -1;
	}

	//PyArrayObject *Py_array1;
	////读取从python脚本返回的numpy值
	////查看是否是元组数据
	//if (PyTuple_Check(pReturn))
	//{
	//	//当返回值不止一个，pReturn是一个元组
	//	PyArg_UnpackTuple(pReturn, "ref", 2, 2, &Py_array1, &a);  //解析元组的内容
	//	//获取矩阵维度
	//	npy_intp *Py_array1_shape = PyArray_DIMS(Py_array1);	//获取元组第一个元素（矩阵）的大小
	//	npy_intp array1row = Py_array1_shape[0];
	//	npy_intp array1col = Py_array1_shape[1];
	//	npy_intp array1high = Py_array1_shape[2];

	//	Mat mask(array1row, array1col, CV_8UC3, PyArray_DATA(Py_array1)); //转三通道彩色图
	//	// Py_XDECREF(PyArray);
	//	/*这里Py_XDECREF(ArgList); 和 Py_XDECREF(PyArray);不能同时使用，否则会引起内存访问冲突
	//	 * 我的理解是：PyTuple_SetItem并不复制数据，只是引用的复制。因此对这两个对象中的任意一个使用
	//	 * Py_XDECREF都可以回收对象。使用两次的话反而会导致冲突。
	//	 */
	//	Py_XDECREF(ArgList);
	//	delete[] CArrays;		// 释放数组内存，最好在PyArray被使用完以后释放
	//	CArrays = nullptr;
	//	Py_XDECREF(pFunc);		//Py_XDECREF是很有必要的，为了避免内存泄漏
	//	Py_XDECREF(pReturn);
	//	// delete(Py_array1);
	//	//imshow("1", mask);
	//	//waitKey(1000);
	//	mask.release();
	//}

	Py_XDECREF(ArgList);
	delete[] CArrays;		// 释放数组内存，最好在PyArray被使用完以后释放
	CArrays = nullptr;
	//Py_XDECREF(pFunc);		//Py_XDECREF是很有必要的，为了避免内存泄漏
	//Py_XDECREF(pReturn);

}