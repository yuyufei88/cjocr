//
// Created by chu on 19-12-7.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include "cjocr.h"

using namespace std;
using namespace cv;
//读取指定文件下的所有图片
vector<Mat> readImages(cv::String pattern) {

    //    获取文件夹下所有文件名
    vector<cv::String> fn;
    glob(pattern, fn, false);
    //    存储所有图片
    vector<Mat> images;
    size_t count = fn.size();
    if (count == 0) {
        cout << "路径或者格式有误，未获取到图片！" << endl;
    }
    for (size_t i = 0; i < count; i++) {
        //读取灰度图片并返回Mat类型;
        images.push_back(imread(fn[i], 0));
    }
    return images;
}

//获取标签图片
vector<Mat> getLabelImages(cv::String pattern) {

    vector<Mat> labelImages = readImages(pattern);

    return labelImages;
}

//获取图像水平方向投影
vector<int> getHProjection(int scrGray, cv::Mat scrImg) {

    vector<int> horizontalNum;
    int num = 0;
    //    获取图像宽度
    int imgWidth = scrImg.cols;
    //    获取图像高度
    int imgHeight = scrImg.rows;
    for (int i = 0; i < imgWidth; i++) {
        for (int j = 0; j < imgHeight; j++) {
            // 判断小于输入阈值的个数
            uchar data = scrImg.at<uchar>(j, i);
            if (data <= scrGray) {
                num += 1;
            }
        }
        // 获取每列所有等于阈值像素点的个数
        horizontalNum.push_back(num);
        num = 0;
    }
    return horizontalNum;
}

//获取图像竖直方向投影
vector<int> getVProjection(int scrGray, cv::Mat scrImg) {

    vector<int> verticalNum;
    int num = 0;
    //    获取图像宽度
    int imgWidth = scrImg.cols;
    //    获取图像高度
    int imgHeight = scrImg.rows;
    for (int i = 0; i < imgHeight; i++) {
        unsigned char *data = scrImg.ptr(i);
        for (int j = 0; j < imgWidth; j++) {
            // 判断小于输入阈值的个数
            if (data[j] <= scrGray) {
                num += 1;
            }
        }
        // 获取每列所有等于阈值像素点的个数
        verticalNum.push_back(num);
        num = 0;
    }
    return verticalNum;
}

//获取价格的起止坐标
vector<int> getPoints(vector<int> horizontalNum, vector<int> verticalNum) {
    vector<int> points;
    int hSize = horizontalNum.size();
    int vSize = verticalNum.size();
    //    计算价格的left坐标
    for (int i = 1; i < hSize; i++) {
        if (horizontalNum[i] > horizontalNum[i - 1]) {
            points.push_back(i);
            break;
        }

    }
    //    计算价格的top坐标
    for (int i = 1; i < vSize; i++) {
        if (verticalNum[i] > verticalNum[i - 1]) {
            points.push_back(i);
            break;
        }

    }
    //    计算价格的right坐标
    for (int i = hSize - 1; i > 0; i--) {
        if (horizontalNum[i] < horizontalNum[i - 1]) {
            points.push_back(i);
            break;
        }

    }
    //    计算价格的bottom坐标
    for (int i = vSize - 1; i > 0; i--) {
        if (verticalNum[i] < verticalNum[i - 1]) {
            points.push_back(i);
            break;
        }

    }
    return points;
}

//获取图片的价格
cv::String getPrice(vector<Mat> labelImages, cv::Mat scrImage, int scrGray, int wordSpace, float sorceThread) {
    if (labelImages.empty() or scrImage.empty()) {
        cout << "输入的测试或标签图片为空" << endl;
        return "";
    }
    cv::String imgPirce = "";
    //    获取标签的宽和高
    int wordWidth = labelImages[0].cols;
    int wordHeight = labelImages[0].rows;
    vector<int> horizontalNum = getHProjection(scrGray, scrImage);
    vector<int> verticalNum = getVProjection(scrGray, scrImage);
    vector<int> points = getPoints(horizontalNum, verticalNum);
    cv::Rect rectImage = Rect(points[0], points[1], points[2] - points[0], points[3] - points[1]);
    cv::Mat priceImage = scrImage(rectImage);
    //    第一个数字为1时，需要加宽图像
    int num = 0;
    for (int i = 0; i < priceImage.rows; i++) {
        uchar data = priceImage.at<uchar>(i, 1);
        if (data <= scrGray) {
            num += 1;
        }
    }
    if (num == wordHeight) {
        points[0] = points[0] - 1;
    }
    int left = points[0];
    int top = points[1];
    int right = points[2];
    int wordNum = int((right - left) / (wordWidth + wordSpace)) + 1;
    for (int j = 0; j < wordNum; j++) {
        //  裁切数字
        cv::Rect wordRect = Rect(left, top, wordWidth, wordHeight);
        cv::Mat wordImg = scrImage(wordRect);
        left = left + wordWidth + wordSpace;
        int labelSize = labelImages.size();
        cv::Mat res;
        for (int k = 0; k < labelSize; k++) {
            //  比对标签
            cv::matchTemplate(wordImg, labelImages[k], res, cv::TM_CCOEFF_NORMED);
            if (res.at<float>(0, 0) >= sorceThread) {
                if (k == labelSize - 1) {
                    imgPirce = imgPirce + ".";
                } else {
                    stringstream s;
                    s << k;
                    imgPirce = imgPirce + s.str();
                }
            }

        }
    }
    return imgPirce;
}

