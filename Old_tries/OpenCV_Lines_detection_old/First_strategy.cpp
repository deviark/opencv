#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2/photo.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	Mat src = imread("DSC09716.JPG", IMREAD_COLOR);
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
	int operation_size = 5;
	Mat dst_morthology;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(black_regions, dst_morthology, MORPH_BLACKHAT, element);
	namedWindow("Morthology");
	imshow("Morthology", dst_morthology);
	waitKey(0);


	/// Find contours   
    
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// RNG rng(12345);
	int color_i = 128;
    findContours( dst_morthology, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros(src.rows, src.cols, CV_8UC1);
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( color_i, color_i, color_i );
		color_i++;
        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
    }     
	operation_size = 3;
	element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(drawing, drawing, MORPH_DILATE, element);
	imshow( "Result window", drawing );
	waitKey(0);
	


	//===Hough Line DETECTOR===
	/*
	std::vector<Vec4i> lines;
	Mat detected_lines = Mat::zeros(src.rows, src.cols, CV_8UC1);
	HoughLinesP(dst_morthology, lines, 3, CV_PI / 180, 50, 300, 15);
	//cdst = src.clone();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(detected_lines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 5, LINE_AA);
	}
	*/

	inpaint(src, drawing, src, 5, INPAINT_TELEA);

	//======Morthology======
	/*
	operation_size = 3;
	/// Mat dst_morthology;
	element = getStructuringElement(MORPH_RECT,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(detected_lines, dst_morthology, MORPH_CLOSE, element);
	/// namedWindow("Dilation Demo");
	imshow("Dilation over detected", dst_morthology);
	waitKey(0);
	*/
	
	
	imshow("Original_photo", src);
	waitKey(0);
	return 0;
}

	/* 25 LINE
	//===Canny edge DETECTOR===
	Mat dst, detected_edges;
	int edgeThresh = 1;
	int lowThreshold = 30;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	char* window_name = "Edge Map";
	blur(black_regions, detected_edges, Size(3, 3));
	namedWindow("Detected edges");
	imshow("Detected_edges", detected_edges);
	waitKey(0);
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	//namedWindow("Detected edges");
	imshow("Detected_edges", dst);
	waitKey(0);
	*/


	//===Hough Circle DETECTOR===
	/*
	vector<Vec3f> circles;
	HoughCircles(black_regions, circles, HOUGH_GRADIENT, 1,
                black_regions.rows/16, // change this value to detect circles with different distances to each other
                100, 30, 450, 700 // change the last two parameters
                            // (min_radius & max_radius) to detect larger circles
                );
	for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( src, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( src, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }
	imshow("Detected circles", src);
	waitKey(0);
	*/


	/// Find contours   
	/* 95 LINE
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
	*/