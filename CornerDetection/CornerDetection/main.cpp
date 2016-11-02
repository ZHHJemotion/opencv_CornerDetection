//
//  main.cpp
//  CornerDetection --- 角点检测
//
//  Created by ZHHJemotion on 2016/11/1.
//  Copyright © 2016年 Lukas_Zhang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#define PATH string("/Users/zhangxingjian/Desktop/Programming/C++/OpenCV/opencv_CornerDetection/CornerDetection/CornerDetection/")

using namespace cv;
using namespace std;


// ------------------------------ 宏定义部分 --------------------------------
#define WINDOW_NAME1 "The Window 1"
#define WINDOW_NAME2 "The Window 2"


// ------------------------------ 全局变量声明部分 --------------------------------
//  全局变量声明
// -----------------------------------------------------------------------------
Mat srcImage, srcImage1, grayImage;//srcImage1为原始图的备份
int thresh = 10; //当前阈值
int maxThresh = 100; //最大阈值


// ----------------------------- 全局函数申明部分 ---------------------------------
//  全局函数申明
// -----------------------------------------------------------------------------
static void onCornerHarris(int, void *); //回调函数
static void showHelpText();

// ----------------------------- main() function -------------------------------

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    //============================== small exercise =============================
    
    // 以灰度图的形式载入图像并显示
    Mat src = imread(PATH+string("1.jpg"),0);// 以灰度图的形式载入图像并显示
    namedWindow("the original image");
    imshow("the original image", src);
    
    // 进行 Harris 角点检测
    Mat cornerStrength;
    cornerHarris(src, cornerStrength, 2, 3, 0.01);// blockSize=2,ksize=3,k=0.01
    
    // 对灰度图进行阈值操作，得到二值图并显示
    Mat harrisCorner;
    threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);// thresh=0.00001,maxval=255,type=THRESH_BINATY
    namedWindow("the destination image");
    imshow("the destination image", harrisCorner);
    
    //waitKey(0);
    
    // ===========================================================================
    
    
    // =========================== 综合范例部分 =================================
    
    showHelpText(); //显示帮助文字
    
    //载入原始图并进行克隆保护
    srcImage = imread(PATH+string("1.jpg"),1);
    //异常处理
    if (!srcImage.data)
    {
        std::cout<<"error: we cannot read the source image!!! \n";
        return false;
    }
    namedWindow("the source image");
    imshow("the source image", srcImage);
    srcImage1 = srcImage.clone();//克隆保护
    
    //保留一张灰度图以备用
    cvtColor(srcImage1, grayImage, CV_BGR2GRAY);
    
    //创建滚动条
    namedWindow(WINDOW_NAME1,CV_WINDOW_AUTOSIZE);
    createTrackbar("Threshold", WINDOW_NAME1, &thresh, maxThresh,onCornerHarris);
    
    //调用回调函数，进行初始化
    onCornerHarris(0, 0);//初始化两个参数均为0
    
    waitKey(0);
    
    return 0;
}


//-------------------------- onCornerHarris() function ------------------------------
// 回调函数
//-----------------------------------------------------------------------------------
static void onCornerHarris(int, void *)
{
    //定义局部变量
    Mat dstImage;//目标图
    Mat normImage;//归一化后的图
    Mat scaledImage;//线性变换后的8位无符号整型(8-bit unsigned int)的图
    
    //初始化
    //置零当前需要显示的两幅图，即清除上一次调用此函数是它们的值
    dstImage = Mat::zeros(srcImage.size(), CV_32FC1);
    srcImage1 = srcImage.clone();
    
    //进行角点检测
    //Harris角点检测为单通道即灰度图检测
    cornerHarris(grayImage, dstImage, 2, 3, 0.04);//blockSize=2,ksize=3,k=0.04
    
    //归一化
    normalize(dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    //C++: void normalize(InputArray src, OutputArray dst, double alpha=1, double beta=0, int norm_type=NORM_L2, int dtype=-1, InputArray mask=noArray() )
    //src – input array.
    //dst – output array of the same size as src .
    //alpha – norm value to normalize to or the lower range boundary in case of the range normalization.
    //beta – upper range boundary in case of the range normalization; it is not used for the norm normalization.
    //normType – normalization type (see the details below).
    //dtype – when negative, the output array has the same type as src; otherwise, it has the same number of channels as src and the depth =CV_MAT_DEPTH(dtype).
    //mask – optional operation mask.
    
    //将归一化后的图像线性变换成8位无符号整型
    convertScaleAbs(normImage, scaledImage);
    
    //进行绘制
    //将检测得到的，且符合阈值条件的角点绘制出来
    for (int j = 0; j < normImage.rows; j++)
    {
        for (int i = 0; i < normImage.cols; i++)
        {
            if ((int)normImage.at<float>(j,i) > thresh+80)
            {
                circle(srcImage1, Point(i,j), 5, Scalar(10,10,255), 2, 8, 0);
                circle(scaledImage, Point(i,j), 5, Scalar(0,10,255),2, 8, 0);
                //radius=5,thickness=2
                //C++: void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
                //img – Image where the circle is drawn.
                //center – Center of the circle.
                //radius – Radius of the circle.
                //color – Circle color.
                //thickness – Thickness of the circle outline, if positive. Negative thickness means that a filled circle is to be drawn.
                //lineType – Type of the circle boundary. See the line() description.
                //shift – Number of fractional bits in the coordinates of the center and in the radius value.

            }
        }
    }
    
    //显示目标图
    imshow(WINDOW_NAME1, srcImage1);
    imshow(WINDOW_NAME2, scaledImage);
    
}


//-------------------------- showHelpText() function -------------------------
// 输出帮助信息
//----------------------------------------------------------------------------
static void showHelpText()
{
    //输出一些帮助信息
    printf("\n\n\n\t\t\t【欢迎来到Harris角点检测示例程序~】\n\n");
    printf("\n\n\n\t请调整滚动条观察图像效果~\n\n");
    printf("\n\n\t\t\t\t\t\t\t\t by ZHHJemotion");
}
