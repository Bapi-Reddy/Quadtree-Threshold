#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(){

Mat input,horizontal,vertical,error,product,output;
//Kernels
Mat kernH = (Mat_<char>(1,3) << -1, 0, 1);
Mat kernV = (Mat_<char>(3,1) << 1, 0, -1);

char path[30];
cout << "Enter the location of image:\n";
cin >> path;
input = imread(path);
 
filter2D(input,horizontal,-1,kernH);
filter2D(input,vertical,-1,kernV);
max(horizontal,vertical,error);
multiply(input,error,product,1,-1);

imshow("horizontal",horizontal);
imshow("vertical",vertical);
imshow("Max",error);
imshow("Product",product);


 
float thresholdValue = sum(product)[0]/sum(error)[0];
cout << sum(product) ;
cout << "Finally threshold value:" << thresholdValue << "\n";

threshold(input,output,thresholdValue,255,THRESH_BINARY);
imshow("Final Output", output);
waitKey(0);

}


 
/*
if(grad = HIgh )
	divide the image
else
	threshold it as per the threshold of 4th piece
*/