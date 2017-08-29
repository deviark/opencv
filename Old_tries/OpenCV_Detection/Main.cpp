#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;

int NOT__main() {

	Mat obj;
	obj = imread("parking.jpg", 1);
	/*
	Mat m_rgb(2, 2, CV_8UC3, Scalar(0, 0, 255));
	Mat M_gray(2, 2, CV_8UC1, Scalar(0));

	//=========rand============
	Mat M = Mat(3, 2, CV_8UC1);
	randu(M, Scalar::all(0), Scalar(255));


	//========ones, zeros=========
	Mat O = Mat::ones(2, 2, CV_32F);
	std::cout << "O=" << std::endl << "" << O << std::endl << std::endl;

	//======contrast, brightness====
	double alpha = 1;
	int beta = 0;



	//=======mask==========

	Mat mask = Mat::zeros(obj.size(), CV_8UC1);
	circle(mask, cvPoint(250, 250), 150, Scalar(255), -1);
	Mat masked = Mat(obj.size(), obj.type());
	obj.copyTo(masked, mask);

	/*
	//======drawing==========
	rectangle(obj, Rect(50, 50, 400, 400), Scalar(0, 0, 255), 5);
	RotatedRect rr = RotatedRect(Point2f(250, 250), Size2f(250, 50), 45);
	ellipse(obj, rr, Scalar(0, 255, 0), -1);

	//=======putText=======
	putText(obj, "Parking", Point(40, 40), FONT_ITALIC, 1, Scalar(150, 250, 0), 3, (4));


	namedWindow("drawing");
	imshow("drawing", obj);
	waitKey(0);*/
	/*
	//=======dostupDoPixelya, medianFilter========
	std::vector<uchar> temp;
	Mat med(obj.size(), obj.type());
//	double t = getTickCount();
	//Vec3b pixel = obj.at<Vec3b>(i, j);
	Mat medFunc;
	medianBlur(obj, medFunc, 3);
	*/
	//t = getTickCount() - t / getTickFrequency();
	//std::cout << t << std::endl;

	obj.forEach<Vec3d>([](Vec3d& val, const int pos[]) {
		val[0] = 0; // set blue to zero
		//val[1] = 1;
	});

	return 0;
}