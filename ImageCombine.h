//
// Created by cirq on 17-10-11.
//

#ifndef IMAGECOMBINE_IMAGECOMBINE_H
#define IMAGECOMBINE_IMAGECOMBINE_H

#include <iostream>
#include <functional>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int loadImages(Mat &img1, Mat &img2, const char *name1, const char *name2){
    /**************************************************************************
     * Open two images with given file names, if sucess, returns 0, else 1.
     **************************************************************************/

    int flag = 0;
    img1 = imread(name1, CV_LOAD_IMAGE_UNCHANGED);
    if(!img1.data){
        cerr << "File " << name1 << " does not exit!" << endl;
        flag = -1;
    }
    img2 = imread(name2, CV_LOAD_IMAGE_UNCHANGED);
    if(!img2.data){
        cerr << "File " << name2 << " does not exit!" << endl;
        flag = -1;
    }
    return flag;
}

// in this group test, show = dog.jpg, hide = maid.jpg

int combineViaInterpolation(Mat &show, Mat &hide, Mat &result){
    /**************************************************************************
     * The show is when image displayed in small scale, while the hide is at
     * another case. Due to this requirement, we assume this condition helds:
     *                          show.size < hide.size
     * in this implementation, show.size = hide.size / 2.
     **************************************************************************/

    int flag = 0;
    try{
        hide.copyTo(result);
        Mat resizedShow;
        resize(show, resizedShow, Size_<int>(hide.cols/2, hide.rows/2), 0, 0, INTER_NEAREST);
        if(result.isContinuous()){
            for(int h = 0; h < result.rows; h++){
                if(h%2 != 0) // control which pixel to be overwritten
                    continue;

                auto *rowPtr = result.ptr<Vec3b>(h);
                for(int w = 0; w < result.cols; w++){
                    if(w%2 != 0) // control which pixel to be overwritten
                        continue;

                    rowPtr[w] = resizedShow.at<Vec3b>(h/2, w/2);
                }
            }
        } else{
            cerr << "error during image scanning (seems not your fault)" << endl;
            flag = -1;
        }
    } catch(exception& e){
        cerr << "error during interpolation: " << e.what() << endl;
        flag = -1;
    }
    return flag;
}

void displayWithInterpolation(Mat *image){
    /**************************************************************************
     * Display images when display in different mode, in this case,
     * in small scale or in large scale (double size).
     **************************************************************************/

    Mat &hidden = *image;
    Mat shown;
    resize(hidden, shown, Size(), 0.5, 0.5, INTER_NEAREST);

    imshow("what is display in small scale", shown);
    imshow("what is display in large scale", hidden);
    waitKey(0);
}


// in this group test, show = white.png, hide = black.png

int combineViaAlpha(Mat &show, Mat &hide, Mat &result){
    /**************************************************************************
     * The show is when image displayed in white background, while the hide is
     * what under opposite case (i.e., in black background color).
     **************************************************************************/

    int flag = 0;
    try{
        function<uchar(Vec3b&)> bgr2grey = [](Vec3b& v3b) {
            return uchar( v3b[0]*0.114 + v3b[1]*0.587 + v3b[2]*0.299 ); };

        Mat resizedShow;
        resize(show, resizedShow, hide.size(), 0, 0, INTER_CUBIC);
        result.create(show.size(), CV_8UC4);
        if(show.isContinuous() && hide.isContinuous() && result.isContinuous()){
            for(int h = 0; h < result.rows; h++){
                auto sPtr = show.ptr<Vec3b>(h);
                auto hPtr = hide.ptr<Vec3b>(h);
                auto rPtr = result.ptr<Vec4b>(h);
                for(int w = 0; w < result.cols; w++){
                    Vec4b& rVec = rPtr[w];

                    uchar sGrey = bgr2grey(sPtr[w]), hGrey = bgr2grey(hPtr[w]);
                    Vec4b sResult, hResult;
                    if(sGrey < 128)
                        sResult = Vec4b(sPtr[w][0], sPtr[w][1], sPtr[w][2], 255);
                    else
                        sResult = Vec4b(sPtr[w][0], sPtr[w][1], sPtr[w][2], 0);
                    if(hGrey < 128)
                        hResult = Vec4b(hPtr[w][0], hPtr[w][1], hPtr[w][2], 0);
                    else
                        hResult = Vec4b(hPtr[w][0], hPtr[w][1], hPtr[w][2], 255);

                    if(sResult[3] == 0 && hResult[3] == 0){
                        rVec[0] = sResult[0], rVec[1] = sResult[1], rVec[2] = sResult[2], rVec[3] = 0;
                    } else if(sResult[3] == 0 && hResult[3] == 255){
                        rVec[0] = hResult[0], rVec[1] = hResult[1], rVec[2] = hResult[2], rVec[3] = 255;
                    } else if(sResult[3] == 255 && hResult[3] == 0){
                        rVec[0] = sResult[0], rVec[1] = sResult[1], rVec[2] = sResult[2], rVec[3] = 64;
                    } else{
                        rVec[0] = uchar((sResult[0] + hResult[0]) / 2),
                        rVec[1] = uchar((sResult[1] + hResult[1]) / 2),
                        rVec[2] = uchar((sResult[2] + hResult[2]) / 2),
                        rVec[3] = 255;
                    }
                }
            }
        } else{
            cerr << "error during image scanning (seems not your fault)" << endl;
            flag = -1;
        }
    } catch(exception& e){
        cerr << "error during interpolation: " << e.what() << endl;
        flag = -1;
    }

    return flag;
}

void alphaBlending(Mat &foreground, Mat &back, Mat &result){
    Mat fore = foreground.clone();

    vector<Mat> bgraChannel;
    split(fore, bgraChannel);
    Mat alpha = bgraChannel[3];
    cvtColor(fore, fore, CV_BGRA2BGR);
    fore.convertTo(fore, CV_32FC3);
    back.convertTo(back, CV_32FC3);
    alpha.convertTo(alpha, CV_32FC3, 1.0/255);
    vector<Mat> alpha3{ alpha, alpha, alpha };
    merge(alpha3, alpha);

    result = Mat::zeros(fore.size(), fore.type());
    multiply(alpha, fore, fore);
    multiply(Scalar::all(1.0)-alpha, back, back);
    add(fore, back, result);
    result.convertTo(result, CV_8UC3);
}

void displayWithAlpha(Mat *image){
    /**************************************************************************
     * Display images when display in different mode, in this case,
     * with white background or with black background.
     **************************************************************************/

    Mat whiteBG(image->size(), CV_8UC3, Scalar(255, 255, 255));
    Mat blackBG(image->size(), CV_8UC3, Scalar(0, 0, 0));
    Mat whiteBGshow, blackBGshow;
    alphaBlending(*image, whiteBG, whiteBGshow);
    alphaBlending(*image, blackBG, blackBGshow);

    imshow("image shown on white background", whiteBGshow);
    imshow("image shown on black background", blackBGshow);
    waitKey(0);
}

#endif //IMAGECOMBINE_IMAGECOMBINE_H
