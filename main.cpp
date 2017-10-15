#include "ImageCombine.h"

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
    auto methodPtr = combineViaAlpha;
    auto displayPtr = displayWithAlpha;

    int combineImagesFlag = methodPtr(showImage, hideImage, dstImage);
    if(combineImagesFlag == -1)
        return EXIT_FAILURE;
    displayPtr(&dstImage);
    return EXIT_SUCCESS;
}
