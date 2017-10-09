#include <iostream>
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
    img1 = imread(name1);
    if(!img1.data){
        cerr << "File " << name1 << " does not exit!" << endl;
        flag = -1;
    }
    img2 = imread(name2);
    if(!img2.data){
        cerr << "File " << name2 << " does not exit!" << endl;
        flag = -1;
    }
    return flag;
}

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

    cout << image->size() << endl;
    Mat &hidden = *image;
    Mat shown;
    resize(hidden, shown, Size(), 0.5, 0.5, INTER_NEAREST);

    imshow("what is display in small scale", shown);
    imshow("what is display in large scale", hidden);
    waitKey(0);
}

int main(int argc, char *argv[]){
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " <filename1> <filename2>" << endl;
        cerr << "<filename1> stands for image which displayed in small scale, white background," << endl;
        cerr << "<filename2> stands for image which displayed in large scale, black background." << endl;
        return EXIT_FAILURE;
    }

    Mat showImage, hideImage, dstImage;
    int loadImagesFlag = loadImages(showImage, hideImage, argv[1], argv[2]);
    if(loadImagesFlag == -1)
        return EXIT_FAILURE;

    // The original usage of function pointers:
    // int (*methodPtr)(Mat &, Mat &, Mat &) = combineViaInterpolation;
    // void (*displayPtr)(Mat *) = displayWithInterpolation;

    // After C++ 11:
    auto methodPtr = combineViaInterpolation;
    auto displayPtr = displayWithInterpolation;

    int combineImagesFlag = methodPtr(showImage, hideImage, dstImage);
    if(combineImagesFlag == -1)
        return EXIT_FAILURE;
    displayPtr(&dstImage);
    return EXIT_SUCCESS;
}