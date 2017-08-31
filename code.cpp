#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Mat inc,in,outg,outl,grad,gt;
int t1,t2,i,j,tnum=0,tden=0,tglob,r,c,s,tloc,n,tc,tr,k,p=0,st;
void quad()     
{
s=0;tnum=0;tden=0;
p++;
k=2*p;
st=500*k;                                     /*varying reference grad of the window with its size*/
			tc=(c-1)/k;                         /*location of the window in the quad*/
			tr=(r-1)/k;
		for(j=tc*(in.cols/n);j<=(tc+k)*(in.cols/n);j++)
		for(i=tr*(in.rows/n);i<=(tr+k)*(in.rows/n);i++)
		{
		s=s+grad.at<uchar>(i,j);        /*grad value of local window*/
		tnum=in.at<uchar>(i,j)*grad.at<uchar>(i,j)+tnum;              /*grey-grad value of local window*/
		}
		if(s!=0)
		tloc=tnum/s;
		if(tloc<10)
			tloc=20;
		if(s>st)               
		{
			for(j=c*(in.cols/n);j<(c+1)*(in.cols/n);j++)
			for(i=r*(in.rows/n);i<(r+1)*(in.rows/n);i++)
			{
			if(in.at<uchar>(i,j)>tloc)
			{
			outl.at<uchar>(i,j)=255;      /*converting to  binary image*/
			}
			else
			outl.at<uchar>(i,j)=0;
			}
		}
		else
			quad();     /*recursive calling of quad if the grad value is less than the expected gtad value*/
}
int main()
{
in=imread("m1.jpg",0);    /*reading the input image*/
grad=in.clone();          /*creation of similar Mats as that of input Mat*/
outg=in.clone();
outl=in.clone();

for(i=0;i<in.rows;i++)
for(j=0;j<in.cols;j++)       /*for global thresholding*/
	{
		outg.at<uchar>(i,j)=0;
		outl.at<uchar>(i,j)=0;
		if(((i+1)<in.rows)&&((i-1)>0))    /*boundary conditions*/
	t1=abs(in.at<uchar>(i-1,j)-in.at<uchar>(i+1,j));    /*gradient at the pixel in horizontal direction*/
		if(((j+1)<in.cols)&&((j-1)>0))   
	t2=abs(in.at<uchar>(i,j-1)-in.at<uchar>(i,j+1));
	if(t1>t2){grad.at<uchar>(i,j)=t1;}
	else{grad.at<uchar>(i,j)=t2;}
	tnum=in.at<uchar>(i,j)*grad.at<uchar>(i,j)+tnum;    /*gradient at the pixel in vertical direction*/
	tden=grad.at<uchar>(i,j)+tden;                  /*summation of gradient values of image*/
	}
printf("%d\n",tnum);
printf("%d\n",tden);
printf("%d\n",in.rows);
printf("%d\n",in.cols);
	tglob=tnum/tden;      /*the global threshold value*/
	printf("%d\n",tglob);
imshow("error",grad);
for(i=0;i<in.rows;i++)
for(j=0;j<in.cols;j++)
	{
		if(in.at<uchar>(i,j)>tglob)
		{
		outg.at<uchar>(i,j)=255;                       /*gray level image to binary image*/
		}
		else
		outg.at<uchar>(i,j)=0;
	}
n=30;                                            /*window size for local thresholding*/
for(c=0;c<n;c++)
for(r=0;r<n;r++)
	{
		s=0;tnum=0,tloc=0;
		for(j=c*(in.cols/n);j<(c+1)*(in.cols/n);j++)   /*scanning windows*/
		for(i=r*(in.rows/n);i<(r+1)*(in.rows/n);i++)
		{
		s=s+grad.at<uchar>(i,j);     /*sum of gradients of pixels within the window*/
		tnum=in.at<uchar>(i,j)*grad.at<uchar>(i,j)+tnum;             /*sum of grey grads of pixels within the image*/
		}
		if(s!=0)
		tloc=tnum/s;          /*local threshold of the considered window*/
		if(tloc<15)
			tloc=15;
		if(s>1500)              
		{
			for(j=c*(in.cols/n);j<(c+1)*(in.cols/n);j++)
			for(i=r*(in.rows/n);i<(r+1)*(in.rows/n);i++)
			{
			if(in.at<uchar>(i,j)>tloc)                /*comaparing the pixel intensities with the local threshold*/
			{
			outl.at<uchar>(i,j)=255;
			}
			else
			outl.at<uchar>(i,j)=0;
			}
		}
		else
		{
			p=0;
			quad();     /*if the local grad value is less than expected then call quad functio*/
		
}
}



namedWindow("in",WINDOW_AUTOSIZE);    /*for the display ofimages*/
 imshow("in",in);
 namedWindow("outg",WINDOW_AUTOSIZE);
 imshow("outg",outg);
 namedWindow("outl",WINDOW_AUTOSIZE);
 imshow("outl",outl);
 waitKey(0);
in.release();
inc.release();
outg.release();
outl.release();

}