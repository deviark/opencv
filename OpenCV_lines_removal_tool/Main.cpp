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
Mat final_mask;
Point start_point, finish_point;
vector<vector<Point>> points(1);
bool drawNow = false;
bool empty_mask = false;
String src_filename;

void morphology_operate(Mat& img, int oprt_size, int oprtn_type);
void detect_contours(Mat& img);
void detect_lines(Mat& img, int line_gap);
void on_lines_trackbar(int, void*);
void on_morph_size_trackbar(int, void*);
void on_morph_opration_trackbar();
void get_working_area(int event, int x, int y, int flags, void* userdata);


int main(int argc, char** argv) {

	if (argc == 2)
		src_filename = argv[1];
	else
		src_filename = "/15";

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
	/// createTrackbar("Morph. size", "final_mask", &operation_size_slider, 20, on_morph_size_trackbar);

	/// on_lines_trackbar(line_gap_slider, 0);
	/// on_morph_size_trackbar(operation_size_slider, 0);

	namedWindow("Please define area to process", WINDOW_NORMAL);
	setMouseCallback("Please define area to process", get_working_area, NULL);
	

	while (1) {
		cout << "\nNEW ITERATION\n";
		src_throught_mask.release();
		src_throught_mask = src.clone();
		cout << "Please define area to process\n";
		imshow("Please define area to process", src_throught_mask);
		int key_pressed = waitKey(0);
		if (key_pressed == 13) { /// ENTER PRESSED
			imwrite(src_filename + "_.jpg", src);
			break;
		}
		else if (key_pressed == 27) { /// ESC PRESSED
			break;
		}
		if (!empty_mask){			
			morphology_operate(src_throught_mask, 3, MORPH_BLACKHAT);
			inRange(src_throught_mask, Scalar(10, 10, 10), Scalar(100, 100, 100), final_mask); /// better to use OTSU
			morphology_operate(final_mask, 2, MORPH_GRADIENT);
			detect_lines(final_mask, line_gap);

			inpaint(src, final_mask, src, 3, INPAINT_TELEA);

			cout << "\nPress enter to finish processing, esc to EXIT or any other key to proceed\n";
			imshow("Original_photo", src);
			key_pressed = waitKey(0);
			if (key_pressed == 13) { /// ENTER PRESSED
				imwrite(src_filename + "_.jpg", src);
				break;
			}
			else if (key_pressed == 27) { /// ESC PRESSED
				break;
			}
		}
		empty_mask = false;
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

void detect_lines(Mat& img, int line_gap = 5) {
	std::vector<Vec4i> lines(1000); /// reserve memory for 1000 elems to avoid BLOCK_TYPE_IS_VALID error in earlier versions of Visual Studio
	Mat detected_lines = Mat::zeros(img.rows, img.cols, CV_8UC1);
	HoughLinesP(img, lines, 3, CV_PI / 180, 50, 30, line_gap);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(detected_lines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 3, LINE_AA);
	}
}

void on_lines_trackbar(int, void*) {

	line_gap = line_gap_slider;
	morphology_operate(final_mask, 3);
	detect_lines(final_mask, line_gap);
	inpaint(src, final_mask, src, 5, INPAINT_TELEA);
	imshow("Original_photo", src);
}

void on_morph_opration_trackbar() {

}

void on_morph_size_trackbar(int, void*) {
	operation_size = operation_size_slider;
	cout << "operation size = " << operation_size << '\n';
	morphology_operate(final_mask, operation_size_slider, 4);
	detect_contours(final_mask);
	///	inpaint(src, final_mask, src, 5, INPAINT_TELEA);
	imshow("Mask_image", final_mask);
	imshow("Original_photo", src);
}

void get_working_area(int event, int x, int y, int flags, void* userdata) {
	Mat image_copy;
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
		final_mask = Mat::zeros(final_mask.size(), CV_8UC1);
 		if (points.at(0).size() < 1) {
			empty_mask = true;
		}
		for (int i = 0; i < points.size(); i++)
		{
			drawContours(workingAreaMask, points, i, 255, CV_FILLED);
		}
		cvtColor(workingAreaMask, workingAreaMask, CV_GRAY2BGR);
			image_copy.release();
			workingAreaMask.copyTo(image_copy);
		src_throught_mask.release();
		src.copyTo(src_throught_mask, workingAreaMask);
			image_copy.release();
			src_throught_mask.copyTo(image_copy);

		workingAreaMask.release();
		points.erase(points.begin()+1, points.end());
		points.clear();
		points.resize(1);
	}
}