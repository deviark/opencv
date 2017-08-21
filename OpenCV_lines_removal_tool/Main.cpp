#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int operation_size_slider = 1, operation_type_slider = 3, line_gap_slider = 1;
int line_gap;
int operation_size;
int operation_type; /// {type is valid 3 - 7}
Mat src;
Mat binary_image;

void morphology_operate(Mat& img, int oprt_size = 5, int oprtn_type = 4){
	if (oprt_size) {
		Mat element = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * oprt_size + 1, 2 * oprt_size + 1),
			Point(oprt_size, oprt_size));
		morphologyEx(img, img, oprtn_type, element);
	}
}

void detect_contours(Mat& img){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);
		drawContours(img, contours, i, color, 1, 8, hierarchy, 0, Point());
	}
}

void detect_lines(Mat& img, int line_gap = 15) {
	std::vector<Vec4i> lines;
	Mat detected_lines = Mat::zeros(src.rows, src.cols, CV_8UC1);
	HoughLinesP(img, lines, 3, CV_PI / 180, 50, 300, line_gap);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(detected_lines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 5, LINE_AA);
	}
}

void on_lines_trackbar(int, void*){
	line_gap = line_gap_slider;
	morphology_operate(binary_image, 3);
	detect_lines(binary_image, line_gap);
	inpaint(src, binary_image, src, 5, INPAINT_TELEA);
	imshow("Original_photo", src);
}

void on_morph_size_trackbar(int, void*){
	operation_size = operation_size_slider;
	cout << "operation size = " << operation_size << '\n';
	morphology_operate(binary_image, operation_size_slider, 4);
	detect_contours(binary_image);
	///	inpaint(src, binary_image, src, 5, INPAINT_TELEA);
	imshow("Binary_image", binary_image);
	imshow("Original_photo", src);
}

void on_morph_opration_trackbar(){

}

void get_working_area() {
	if ("MOUSE PRESSED") {
		/// GET MOUSE POSITION (startPoint(x, y))
	}
	if ("MOUSE RELEASED") {
		/// GET MOUSE POSITION (endPoint(x, y))
		/// DRAW RECTANGLE startPoint, endPoint
	}

	/*
	much more precisian contour
	findContour while mouse is pressed
	// create temporary image that will hold the mask
	Mat mask_image( your_image.size(), CV_8U, Scalar(0));
	// draw your contour in mask
	drawContours(mask_image, contours, ind, Scalar(255), CV_FILLED);
	// copy only non-zero pixels from your image to original image
	your_image.copyTo(original_image, mask_image);
	*/
}

int main(int argc, char** argv) {

	src = imread("img2.JPG", IMREAD_COLOR);
	Size dsrd_size(800, 600);
	resize(src, src, dsrd_size);
	namedWindow("Original_photo", WINDOW_NORMAL);
	imshow("Original_photo", src);
	waitKey(0);

	inRange(src, Scalar(80, 80, 70), Scalar(230, 225, 225), binary_image);
	namedWindow("Binary_image");
	imshow("Binary_image", binary_image);
	waitKey(0);

	createTrackbar("Line gap", "Original_photo", &line_gap_slider, 50, on_lines_trackbar);
	createTrackbar("Morph. size", "Binary_image", &operation_size_slider, 20, on_morph_size_trackbar);

	/// on_lines_trackbar(line_gap_slider, 0);
	/// on_morph_size_trackbar(operation_size_slider, 0);

	/// imshow("Original_photo", src);
	waitKey(0);
	return 0;
}
