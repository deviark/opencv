#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {

	Mat src = imread("DSC09716.JPG", IMREAD_COLOR);
	Size dsrd_size(800, 600);
	resize(src, src, dsrd_size);
	namedWindow("Original_photo", WINDOW_NORMAL);
	imshow("Original_photo", src);
	waitKey(0);
	Mat black_regions;
	inRange(src, Scalar(80, 80, 70), Scalar(230, 225, 218), black_regions);
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
	/* Mat dilation_dst;
	int dilation_elem = 0;
	int dilation_size = 3;
	int dilation_type = 0;
	//if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	//else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	//else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(black_regions, dilation_dst, element);

	// =====Erosion=====
	int erosion_type;
	///if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
	///else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
	///else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
	element = getStructuringElement( MORPH_RECT,
										Size( 2*dilation_size + 1, 2*dilation_size+1 ),
										Point( dilation_size, dilation_size ) );
	/// Apply the erosion operation
	erode( dilation_dst, dilation_dst, element );
	*/ 
	int operation_size = 3;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * operation_size + 1, 2 * operation_size + 1),
		Point(operation_size, operation_size));
	morphologyEx(black_regions, black_regions, MORPH_GRADIENT, element);
	imshow("Dilation Demo", black_regions);
	waitKey(0);


	//===Hough Line DETECTOR===
	std::vector<Vec4i> lines;
	HoughLinesP(black_regions, lines, 5, CV_PI / 180, 300, 50, 10);
	//cdst = src.clone();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
	}
	
	imshow("Original_photo", src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}