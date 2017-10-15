# imageCombine
a ways of image steganography by combining two image (trick in QQ/WeChat)


Now there are two ways to achieve this goal, from [Zhihu](https://www.zhihu.com/question/42093145):
+ Hidding small image in larger one by pixel subsisitution, requiring nearest neighbor interpolation when image resized to be smaller;
+ Hidding image by transparency difference, especially visual-pleasant when show image is white hue and hide image is black hue.

General usage can be found in `main.cpp`, you should replace the function pointers to desired methods.