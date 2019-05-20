// demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <armadillo>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_wavelet.h>
#include <gsl/gsl_sf.h>

int main()
{
	arma::mat mat1(3, 4, arma::fill::randn);
	mat1.print(std::cout, "mat1");
	arma::mat mat2(3, 4, arma::fill::randn);
	mat2.print(std::cout, "mat2");
	arma::mat mat3(3, 4, arma::fill::randn);
	mat3.print(std::cout, "mat3");

	arma::cube cube(3, 4, 3, arma::fill::zeros);
	cube.slice(0) = mat1;
	cube.slice(1) = mat2;
	cube.slice(2) = mat3;
	cube.print(std::cout, "\ncube");

	arma::vec z1 = cube.tube(0, 0);
	z1.print(std::cout, "\nz1");

	arma::vec z2 = cube.tube(2, 2);
	z2.print(std::cout, "\nz2");

	arma::vec data(128, arma::fill::randn);
	double* arr = new double[data.size()];
	std::copy(data.begin(), data.end(), arr);
	gsl_wavelet *w;
	w = gsl_wavelet_alloc(gsl_wavelet_daubechies, 4);
	/*gsl_wavelet_workspace *work;
	work = gsl_wavelet_workspace_alloc(data.size());*/

	delete[] arr;
	//gsl_wavelet_workspace_free(work);
	gsl_wavelet_free(w);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
