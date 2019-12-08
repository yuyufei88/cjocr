#include <iostream>
#include <opencv2/opencv.hpp>
#include "cjocr.h"

using namespace std;
using namespace cv;


int main() {
    //测试图片的路径
    cv::String valPattern = "../val/*.PNG";
    //标签图片路径
    cv::String labelPattern = "../labels/*.jpg";
    //获取测试图片
    vector<Mat> valImages = readImages(valPattern);
    //获取标签图片
    vector<Mat> labelImages = getLabelImages(labelPattern);
    cv::String result;
    int valGray = 2;
    int wordSpace = 1;
    float sorceThread = 0.9;
    int64 start = cv::getTickCount();
    for (int i = 0; i < valImages.size(); i++) {
        result = getPrice(labelImages, valImages[i], valGray, wordSpace, sorceThread);
//        std::cout << "图片价格是：" << result << std::endl;
//        cv::Mat image = valImages[i];
//        cv::putText(image, result, cv::Point(15,15), cv::FONT_HERSHEY_TRIPLEX, 0.4, cv::Scalar(0, 0, 255), 1, CV_AA);
//        cv::namedWindow("img",0);
//        cv::imshow("img",image);
//        cv::waitKey();
    }
    cout<<"测试总耗时为："<<(cv::getTickCount()-start)/cv::getTickFrequency()<<endl;
    return 0;
}