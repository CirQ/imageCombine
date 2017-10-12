//
// Created by cirq on 17-10-11.
//

#ifndef IMAGECOMBINE_IMAGECOMBINE_H
#define IMAGECOMBINE_IMAGECOMBINE_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

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


// in this group test, show = white.jpg, hide = black.jpg

int combineViaAlpha(Mat &show, Mat &hide, Mat &result){
    /**************************************************************************
     * The show is when image displayed in white background, while the hide is
     * what under opposite case (i.e., in black background color).
     **************************************************************************/

    int flag = 0;

}

#endif //IMAGECOMBINE_IMAGECOMBINE_H
