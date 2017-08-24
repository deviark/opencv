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
int operation_type; /// {type is valid 3 - 7} => change to enum
Mat src, src_throught_mask;;
Mat binary_image;
Point start_point, finish_point;
vector<vector<Point>> points(1000);
bool drawNow = false;

void morphology_operate(Mat& img, int oprt_size, int oprtn_type);
void detect_contours(Mat& img);
void detect_lines(Mat& img, int line_gap);
void on_lines_trackbar(int, void*);
void on_morph_size_trackbar(int, void*);
void on_morph_opration_trackbar();
void get_working_area(int event, int x, int y, int flags, void* userdata);


int main(int argc, char** argv) {
	String src_filename = "images/img2";

	src = imread(src_filename + ".jpg", IMREAD_COLOR);
	if (src.empty()) {
		cerr << "failed to load the image";
		return-1;
	}
	Size dsrd_size(800, 600);
	resize(src, src, dsrd_size);
	namedWindow("Original_photo", WINDOW_NORMAL);
	imshow("Original_photo", src);
	waitKey(0);

	createTrackbar("Processing depth", "Original_photo", &line_gap_slider, 10, on_lines_trackbar);
	/// createTrackbar("Morph. size", "Binary_image", &operation_size_slider, 20, on_morph_size_trackbar);

	/// on_lines_trackbar(line_gap_slider, 0);
	/// on_morph_size_trackbar(operation_size_slider, 0);

	namedWindow("Please define area to process", WINDOW_NORMAL);


	while (1){

		line_gap_slider = 1;
		src_throught_mask.release();
		src.copyTo(src_throught_mask);
		imshow("Please define area to process", src_throught_mask);
		setMouseCallback("Please define area to process", get_working_area, NULL);
		waitKey(0);
		binary_image.release();

		inRange(src_throught_mask, Scalar(80, 80, 70), Scalar(230, 225, 225), binary_image);
		/// namedWindow("Binary_image");
		/// imshow("Binary_image", binary_image);
		/// waitKey(0);
		morphology_operate(binary_image, 3, MORPH_CLOSE);
		detect_lines(binary_image, line_gap);
		inpaint(src, binary_image, src, 5, INPAINT_TELEA);
		imshow("Original_photo", src);
		
		/// imshow("Original_photo", src);
		cout << "\nPress enter to finish processing or any other key to proceed\n";
		int key_pressed = waitKey(0);
		if (key_pressed == 13){
			imwrite(src_filename, src);
			break;
		}
		else if (key_pressed == 10){
			break;
		}
	} /// end of WHILE loop
	cout << "Exiting program\n";	
	return 0;
}


void morphology_operate(Mat& img, int oprt_size = 5, int oprtn_type = MORPH_GRADIENT) {
	if (oprt_size) {
		Mat element = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * oprt_size + 1, 2 * oprt_size + 1),
			Point(oprt_size, oprt_size));
		morphologyEx(img, img, oprtn_type, element);
	}
}

void detect_contours(Mat& img) {
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

void on_lines_trackbar(int, void*) {

	line_gap = line_gap_slider;
	/// morphology_operate(binary_image, 3);
	/// detect_lines(binary_image, line_gap);
	/// inpaint(src, binary_image, src, 5, INPAINT_TELEA);
	/// imshow("Original_photo", src);
}

void on_morph_opration_trackbar() {

}

void on_morph_size_trackbar(int, void*) {
	operation_size = operation_size_slider;
	cout << "operation size = " << operation_size << '\n';
	morphology_operate(binary_image, operation_size_slider, 4);
	detect_contours(binary_image);
	///	inpaint(src, binary_image, src, 5, INPAINT_TELEA);
	imshow("Binary_image", binary_image);
	imshow("Original_photo", src);
}

void get_working_area(int event, int x, int y, int flags, void* userdata) {

	if ((!drawNow) && (flags == (EVENT_LBUTTONDOWN)))
	{
		cout << "Starting to draw: (" << x << ", " << y << ")" << endl;
		start_point.x = x;
		start_point.y = y;
		drawNow = true;
	}
	else if ((drawNow) && (flags == (EVENT_LBUTTONDOWN + EVENT_MOUSEMOVE))) {
		if (!drawNow) return;
		cout << "Current point: (" << x << ", " << y << ")" << endl;
		finish_point.x = x;
		finish_point.y = y;
		line(src_throught_mask, start_point, finish_point, Scalar(0, 255, 0), 1, 8, 0);
		points.at(0).push_back(start_point);
		start_point = finish_point;
		imshow("Please define area to process", src_throught_mask);
	}
	else if ((drawNow) && (flags != EVENT_LBUTTONDOWN)) {
		cout << "READY TO DRAW CONTOUR";
		drawNow = false;
		Mat workingAreaMask = Mat::zeros(src.size(), CV_8UC1);
		for (int i = 0; i < points.size(); i++)
		{
			drawContours(workingAreaMask, points, i, 255, CV_FILLED);
		}
		cvtColor(workingAreaMask, workingAreaMask, CV_GRAY2BGR);
		/// imshow("Mask image", workingAreaMask);
		src_throught_mask.release();
		src.copyTo(src_throught_mask, workingAreaMask);
		/// imshow("With_mask", src_throught_mask);
	}
}