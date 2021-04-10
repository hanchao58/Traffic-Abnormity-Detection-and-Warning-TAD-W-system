#include "PyCar.h"



int PyCar::star()
{
	//Py_SetPythonHome(L"E:\\python\\python-3.6.5");		//����Python·��
	Py_Initialize();	// ��ʼ��Python
	import_array();		// ʹ��Numpy����غ���ǰ����������һ�д���

	// ����ʼ���Ƿ�ɹ�
	if (!Py_IsInitialized())
		return -1;

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");	// .py��·����������cpp����ͬһ��Ŀ¼��


	moduleName = PyUnicode_FromString("VD");  //������Ϊ.py���ļ�

	pModule = PyImport_Import(moduleName);
	if (!pModule)		//��������ڸ��ļ��������
	{
		printf("can't find py");
		//getchar();		//����س��رմ���
		Py_Finalize();			//�ر�python������
		return -1;
	}

	pFunc = PyObject_GetAttrString(pModule, "run");  //��ȡ����
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
	Py_Finalize();	// �ر�Python
	// system("pause");
	// getchar();
	Py_XDECREF(pFunc);		//Py_XDECREF�Ǻ��б�Ҫ�ģ�Ϊ�˱����ڴ�й©
	Py_XDECREF(pReturn);

}


int PyCar::findcar(Mat img)
{

	if (img.empty())
	{
		Py_Finalize();
		return -1;
	}

	// CV::Mat ת python numpy
	auto sz = img.size();	// ��ȡͼ��ĳߴ�
	int x = sz.width;
	int y = sz.height;
	int z = img.channels();
	uchar *CArrays = new uchar[x*y*z];//��һ��������ڴ���Ҫ�ͷ�ָ�룬��������ڴ�й©������
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
			CArrays[++id] = p[j];//�����ռ�
		}
	}
	npy_intp Dims[3] = { y, x, z }; //ע�����ά�����ݣ�
	PyObject *ArgList = PyTuple_New(1);  //�����б�:����һ������Ϊ1��Ԫ��
	PyObject *PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);
	PyTuple_SetItem(ArgList, 0, PyArray); //��PyArray������ָ��Ԫ��ArgList�ĵ�0��Ԫ��
	//PyObject *pFunc = PyObject_GetAttrString(pModule, "run");  //��ȡ����
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
	////��ȡ��python�ű����ص�numpyֵ
	////�鿴�Ƿ���Ԫ������
	//if (PyTuple_Check(pReturn))
	//{
	//	//������ֵ��ֹһ����pReturn��һ��Ԫ��
	//	PyArg_UnpackTuple(pReturn, "ref", 2, 2, &Py_array1, &a);  //����Ԫ�������
	//	//��ȡ����ά��
	//	npy_intp *Py_array1_shape = PyArray_DIMS(Py_array1);	//��ȡԪ���һ��Ԫ�أ����󣩵Ĵ�С
	//	npy_intp array1row = Py_array1_shape[0];
	//	npy_intp array1col = Py_array1_shape[1];
	//	npy_intp array1high = Py_array1_shape[2];

	//	Mat mask(array1row, array1col, CV_8UC3, PyArray_DATA(Py_array1)); //ת��ͨ����ɫͼ
	//	// Py_XDECREF(PyArray);
	//	/*����Py_XDECREF(ArgList); �� Py_XDECREF(PyArray);����ͬʱʹ�ã�����������ڴ���ʳ�ͻ
	//	 * �ҵ�����ǣ�PyTuple_SetItem�����������ݣ�ֻ�����õĸ��ơ���˶������������е�����һ��ʹ��
	//	 * Py_XDECREF�����Ի��ն���ʹ�����εĻ������ᵼ�³�ͻ��
	//	 */
	//	Py_XDECREF(ArgList);
	//	delete[] CArrays;		// �ͷ������ڴ棬�����PyArray��ʹ�����Ժ��ͷ�
	//	CArrays = nullptr;
	//	Py_XDECREF(pFunc);		//Py_XDECREF�Ǻ��б�Ҫ�ģ�Ϊ�˱����ڴ�й©
	//	Py_XDECREF(pReturn);
	//	// delete(Py_array1);
	//	//imshow("1", mask);
	//	//waitKey(1000);
	//	mask.release();
	//}

	Py_XDECREF(ArgList);
	delete[] CArrays;		// �ͷ������ڴ棬�����PyArray��ʹ�����Ժ��ͷ�
	CArrays = nullptr;
	//Py_XDECREF(pFunc);		//Py_XDECREF�Ǻ��б�Ҫ�ģ�Ϊ�˱����ڴ�й©
	//Py_XDECREF(pReturn);

}