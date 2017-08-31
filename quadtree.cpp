#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;


 
int productSummer(Mat input, Mat error){
int sum=0;
for(int i=0;i<input.rows;i++)
	for(int j=0;j<input.cols;j++)      
		sum=sum+input.at<uchar>(i,j)*error.at<uchar>(i,j);
return sum;
}


Mat EdgeFinder(Mat input){
	int x,y;
	Mat grad = input.clone();
for(int i=0;i<input.rows;i++)
	for(int j=0;j<input.cols;j++)       /*for global thresholding*/
		{
		 	if(((i+1)<input.rows)&&((i-1)>0))    /*boundary conditions*/
	x=abs(input.at<uchar>(i-1,j)-input.at<uchar>(i+1,j));    /*gradient at the pixel in horizontal direction*/
		if(((j+1)<input.cols)&&((j-1)>0))   
	y=abs(input.at<uchar>(i,j-1)-input.at<uchar>(i,j+1));
	grad.at<uchar>(i,j) = x>y ? x:y;
	}
	return grad;
}

float MaxEdge(Mat testImage){
Mat error = EdgeFinder(testImage);
return sum(error)[0];
}


 
void createQuadtree(Mat &temp){
	Mat temp1,temp2,temp3,temp4,ouTrail;
 
 		temp1 = temp(Range(0,temp.rows / 2 ),Range(0,temp.cols / 2 ));

		temp2 = temp(Range(0, temp.rows / 2 ),Range(temp.cols / 2,temp.cols ));

		temp3 = temp(Range(temp.rows / 2,temp.rows ),Range(0,temp.cols / 2 ));

		temp4 = temp(Range(temp.rows/2,temp.rows ),Range(temp.cols/2,temp.cols ));
 		 
	Mat kernH = (Mat_<char>(1,3) << -1, 0, 1);
	Mat kernV = (Mat_<char>(3,1) << 1, 0, -1);
	float thresholdValue;
	Mat horizontal,vertical,error,product;
	filter2D(temp,horizontal,-1,kernH);
 	filter2D(temp,vertical,-1,kernV);
	max(horizontal,vertical,error);
	
	thresholdValue = productSummer(temp,error)/sum(error)[0];
	if(thresholdValue<10)
		thresholdValue = 20;
  	int sensvalue = 1000;

		if(MaxEdge(temp1)>sensvalue){
 			createQuadtree(temp1);
		}
		else{
 			threshold(temp1,temp1,thresholdValue,255,THRESH_BINARY);
 		temp1.copyTo(temp(Range(0,temp.rows / 2 ),Range(0,temp.cols / 2 )));
		}

		if(MaxEdge(temp2)>sensvalue){
			createQuadtree(temp2);
		}
		else{
			threshold(temp2,temp2,thresholdValue,255,THRESH_BINARY);	
		temp2.copyTo(temp(Range(0, temp.rows / 2 ),Range(temp.cols / 2,temp.cols )));
		}

		if(MaxEdge(temp3)>sensvalue){
			createQuadtree(temp3);
		}
		else{
			threshold(temp3,temp3,thresholdValue,255,THRESH_BINARY);	
		temp3.copyTo(temp(Range(temp.rows / 2,temp.rows ),Range(0,temp.cols / 2 )));
		}

		if(MaxEdge(temp4)>sensvalue){
			createQuadtree(temp4);
		}
		else{
			threshold(temp4,temp4,thresholdValue,255,THRESH_BINARY);	
		temp4.copyTo(temp(Range(temp.rows / 2,temp.rows ),Range(temp.cols / 2,temp.cols )));
		}

 
}




int main() {

Mat input,input1,horizontal,vertical,error,product,output1,ouTrail,output,filteredOutput;
int count = 30 ;
Mat testImage;
char path[30];
cout << "Enter the location of image:\n";
cin >> path;
input1 = imread(path);
cvtColor( input1, input, CV_BGR2GRAY);
imshow("input",input);
output = input.clone();
product = input.clone();
error = EdgeFinder(input);
float thresholdValue = productSummer(input,error)/sum(error)[0];

cout << "Global threshold value:" << thresholdValue << "\n";
threshold(input,output1,thresholdValue,255,THRESH_BINARY);
imwrite("GlobalThresholdOutput.jpg", output1);



for (int i = 0; i < count; i++)
{
	for (int j = 0; j < count; j++)
	{
 		testImage = input(Range(i * input.rows /count, (i+1) * input.rows/count),Range(j * input.cols /count, (j+1) * input.cols/count));
   		error = EdgeFinder(testImage);
 		
		thresholdValue = productSummer(testImage,error)/sum(error)[0];
		if(thresholdValue < 15)
			thresholdValue = 15;
		if(sum(error)[0] > 1500){
			threshold(testImage,ouTrail,thresholdValue,255,THRESH_BINARY);
			ouTrail.copyTo(output(Range(i * input.rows /count, (i+1) * input.rows/count),Range(j * input.cols /count, (j+1) * input.cols/count)));
		}
		else{
 			createQuadtree(testImage);
			testImage.copyTo(output(Range(i * input.rows /count, (i+1) * input.rows/count),Range(j * input.cols /count, (j+1) * input.cols/count)));
		}
   	}
}

imwrite("LocalThresholdingOutput.jpg", output);
medianBlur(output,filteredOutput,3);
imwrite("FilteredOutput.jpg",filteredOutput);
waitKey(0);


}