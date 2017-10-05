#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int loadImages(Mat& img1, Mat& img2, const char* name1, const char* name2){
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

int main(int argc, char* argv[]) {
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " <filename1> <filename2>" << endl;
        return EXIT_FAILURE;
    }

    Mat showImage, hideImage;
    int loadImageFlag = loadImages(showImage, hideImage, argv[1], argv[2]);
    if(loadImageFlag == -1)
        return EXIT_FAILURE;

    //Vec3b intensity = hideImage.at<Vec3b>(1, 3);
    //cout << intensity << endl;

    //cout << hideImage.rows << hideImage.cols << endl;

    //switch(hideImage.channels()){
    //    case 1: {
    //        break;
    //    }
    //    case 3: {
    //        MatIterator_<Vec3b> it, end;
    //        int i = 0;
    //        for(it = hideImage.begin<Vec3b>(), end = hideImage.end<Vec3b>(); it != end; it++){
    //            cout << i << *it << endl;
    //            i++;
    //        }
    //        break;
    //    }
    //    default:
    //        break;
    //}



    return EXIT_SUCCESS;
}