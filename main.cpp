/*
* �ļ����ƣ�main.cpp
* �����⣺opencv3.1.0�������汾
* ���Ի�����VS2015 x64
* ժ Ҫ��	calcOpticalFlowFeaturesPyrLKSAD�����Ĳ��Ժ������������ݼ�ΪETH���ݼ����ܹ�����107֡ͼ��
* �����ļ���calcOpticalFlowFeaturesPyrLKSAD.cpp/.h����Ҫ�����ļ���ReadImages.cpp/.h������Ƭ��
* ���ֲ��Խ����pyrLKSize��������Ƚ�Ӱ����ʱ��ȡpyrLKSize=5��107֡��ʱԼ3.4�룻ȡ15��ʱԼ6.5�롢
*				���������ĿnumOFҲ�Ƚ�Ӱ��ʱ�䣬һ��ȡnumOF=18�����Զ�һЩ���Թ�ȡ15����ʱ���˽�0.4�룩
* ʹ��ʵ������ʾ������main.cpp
* 2016.05.24
*/

#pragma once
#include "calcOpticalFlowFeaturesPyrLKSAD.h"
#include "ReadImages.h"
#include <time.h>

int main()
{
	const int numOF = 18;				//���������Ŀ
	const int pyrLKSize = 5;			//������LK��������
	const int sadBlockSize = 4;			//SADƥ����С����ʵ��СΪ2*sadBlockSize+1��
	const int blockSize = 5;			//�Ǽ���ֵ���ƴ�����ʵ��С
	const int maxPixShift = 30;			//�趨���������λ�ƣ�����֡�䣩

	std::vector<cv::Point2f> prevXY;	//�洢�������������һ֡�е�����
	std::vector<cv::Point2f> currXY;	//�洢�������������һ֡�е�����

	float ofXY[numOF][2];				//�洢��������Ĺ���

	//ͼƬ·����ETH���ݼ�ͼƬ���ܹ�107֡
	ReadImages imageReader("G:\\ETH ASL\\ETH-IMAGE-MODIFIED\\Img1", "", ".png");

	cv::Mat prev;
	cv::Mat curr;

	//�ȼ����һ��֡
	prev = imageReader.loadImage(1);
	curr = imageReader.loadImage(2);

	calcOpticalFlowFeaturesPyrLKSAD(
		prev,
		curr,
		prevXY,
		currXY,
		ofXY,
		1,
		numOF,
		pyrLKSize,
		sadBlockSize,
		blockSize,
		maxPixShift);

	//Ϊ����ȾԭͼƬ������ԭͼƬ���ڸ���ͼƬ�ϻ�㣬��ʾ���
	cv::Mat prev_copy, curr_copy;
	prev.copyTo(prev_copy);
	curr.copyTo(curr_copy);

	//��ʾ��һ��֡�Ľ��
	for (int iter2 = 0;iter2 < prevXY.size();iter2++)
	{
		std::cout << prevXY[iter2].x << ", " << prevXY[iter2].y << std::endl;
		std::cout << currXY[iter2].x << ", " << currXY[iter2].y << std::endl;
		std::cout << ofXY[iter2][0] << ", " << ofXY[iter2][1] << std::endl << std::endl;
		cv::circle(prev_copy, prevXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
		cv::circle(curr_copy, currXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
	}
	cv::imshow("prevImg", prev_copy);
	cv::imshow("currImg", curr_copy);
	cv::waitKey(1000);

	curr.copyTo(prev);
	int imgID = 3;

	//��ʱ
	double timeConsume;
	clock_t start, finish;
	start = clock();

	//����ѭ������
	while (1)
	{
		curr = imageReader.loadImage(imgID);	//���뵱ǰ֡

		if (curr.empty())	break;				//ѭ����ֹ����

		prev.copyTo(prev_copy);
		curr.copyTo(curr_copy);
		prevXY.swap(currXY);					//��������ǰ֡��Ϊ��һ֡������
		currXY.clear();							//*****�ǵ�����*****

		calcOpticalFlowFeaturesPyrLKSAD(
			prev,
			curr,
			prevXY,
			currXY,
			ofXY,
			0,
			numOF,
			pyrLKSize,
			sadBlockSize,
			blockSize,
			maxPixShift);

		//��ʾ���
		for (int iter2 = 0;iter2 < prevXY.size();iter2++)
		{
			//std::cout << prevXY[iter2].x << ", " << prevXY[iter2].y << std::endl;
			//std::cout << currXY[iter2].x << ", " << currXY[iter2].y << std::endl;
			std::cout << ofXY[iter2][0] << ", " << ofXY[iter2][1] << std::endl << std::endl;
			cv::circle(prev_copy, prevXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
			cv::circle(curr_copy, currXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
		}
		cv::imshow("prevImg", prev_copy);
		cv::imshow("currImg", curr_copy);
		cv::waitKey(500);

		curr.copyTo(prev);						//��ǰ֡��Ϊ��һ֡
		std::cout << std::endl << "****************************����ɵ�" << imgID << "֡****************************" << std::endl << std::endl;
		imgID++;
	}

	finish = clock();

	//���㲢��ʾ��ʱ
	timeConsume = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << std::endl << "timeConsume= " << timeConsume << std::endl << std::endl;

	system("pause");
}