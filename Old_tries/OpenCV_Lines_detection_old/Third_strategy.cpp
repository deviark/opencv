#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int NOTmain(int argc, char** argv) {

	Mat src = imread("10.jpg", IMREAD_COLOR);
	Size dsrd_size(800, 600);
	resize(src, src, dsrd_size);
	namedWindow("Original_photo", WINDOW_NORMAL);
	imshow("Original_photo", src);
	waitKey(0);
	Mat black_regions;
	inRange(src, Scalar(80, 80, 70), Scalar(230, 225, 225), black_regions);
	namedWindow("Black_regions");
	imshow("Balck_regions", black_regions);
	waitKey(0);
	
	//======Morthology======
	int operation_size = 3;
	Mat dst_morthology;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(black_regions, dst_morthology, MORPH_GRADIENT, element);
	namedWindow("Dilation Demo");
	imshow("Dilation Demo", dst_morthology);
	waitKey(0);


	/// Find contours       
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// RNG rng(12345);
	int color_i = 0;
    findContours( dst_morthology, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros( dst_morthology.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( color_i, color_i, color_i );
		color_i++;
        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
    }     
	imshow( "Result window", drawing );
	waitKey(0);
	


	//===Hough Line DETECTOR===
	std::vector<Vec4i> lines;
	Mat detected_lines = Mat::zeros(src.rows, src.cols, CV_8UC1);
	HoughLinesP(dst_morthology, lines, 3, CV_PI / 180, 50, 300, 8);
	//cdst = src.clone();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(detected_lines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 1, LINE_AA);
	}

	//======Morthology======
	operation_size = 3;
	/// Mat dst_morthology;
	element = getStructuringElement(MORPH_RECT,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(detected_lines, dst_morthology, MORPH_CLOSE, element);
	/// namedWindow("Dilation Demo");
	imshow("Dilation over detected", dst_morthology);
	waitKey(0);
	
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// RNG rng(12345);
	int color_i = 127;
    findContours( dst_morthology, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros( dst_morthology.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( color_i, color_i, color_i );
		color_i++;
        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
    }     
	imshow( "Result window", drawing );
	waitKey(0);
	

	/*
	imshow("Original_photo", src);
	waitKey(0);*/
	return 0;
}