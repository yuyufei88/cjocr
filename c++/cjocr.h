//
// Created by chu on 19-12-7.
//
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#ifndef CJ_OCR_CJOCR_H
#define CJ_OCR_CJOCR_H

//读取指定文件下的所有图片
vector<Mat> readImages(cv::String pattern);

//获取标签图片
vector<Mat> getLabelImages(cv::String pattern);

//获取图像水平方向投影
vector<int> getHProjection(int scrGray, cv::Mat scrImg);

//获取图像竖直方向投影
vector<int> getVProjection(int scrGray, cv::Mat scrImg);

//获取价格的起止坐标
vector<int> getPoints(vector<int> horizontalNum, vector<int> verticalNum);

//获取图片的价格
cv::String getPrice(vector<Mat> labelImages, cv::Mat scrImage, int scrGray, int wordSpace, float sorceThread);
#endif //CJ_OCR_CJOCR_H
