#include "ReadImages.h"
#pragma once

ReadImages::ReadImages(std::string basepath, const std::string imagename, const std::string suffix)
{
	//��·���е��ļ��зָ���ͳһΪ'/'�����Բ���Ҫ���forѭ�������Զ�ͳһ�������ʹ�õ�����ο�C++ Primer��
	for (auto &c : basepath)
	{
		if (c == '\\')
		{
			c = '/';
		}
	}

	_imgSource._basepath = basepath + "/";	//�������'/'��������'\\'
	_imgSource._imagename = imagename;		//ͼƬ����������ţ�
	_imgSource._suffix = suffix;				//ͼ����չ��

}

//���뵥��ͼƬ
//���룺imgId��һ��Ҫ��������ͼƬ��ͼƬ���и����
cv::Mat ReadImages::loadImage(int imgId)
{	
	//��ͼƬ���ת���ַ���
	std::stringstream ss;
	std::string imgNum;
	ss << imgId;
	ss >> imgNum;

	//�õ�ͼƬ����������·��
	std::string path = _imgSource._basepath + _imgSource._imagename + imgNum + _imgSource._suffix;
	
	cv::Mat img = cv::imread(path,0);

	return img;
}