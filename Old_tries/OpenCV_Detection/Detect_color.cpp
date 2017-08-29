#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {

	Mat src = imread("img2.jpg", IMREAD_COLOR);
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
	
	/*
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

	//======Dilation======
	Mat operation_dst;
	int operation_size = 2;
	//if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	//else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	//else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(black_regions, operation_dst, CV_MOP_GRADIENT, element);
	imshow("Dilation Demo", operation_dst);
	waitKey(0);


	//===Hough Line DETECTOR===
	std::vector<Vec4i> lines;
	HoughLinesP(operation_dst, lines, 1, CV_PI / 180, 50, 100, 8);
	//cdst = src.clone();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
	}
	
	imshow("Original_photo", src);
	waitKey(0);
	return 0;
}