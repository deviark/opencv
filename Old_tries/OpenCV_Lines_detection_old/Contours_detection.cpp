#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <vector>

using namespace cv;
using namespace std;

int FINDCONTUR( )
{
    Mat image;
    image = imread("img2.jpg", 1);  
    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );  
    imshow( "Display window", image );
    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    Canny(gray, gray, 100, 200, 3);
    /// Find contours   
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// RNG rng(12345);
	int color_i = 0;
    findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( color_i, color_i, color_i );
		color_i++;
        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
    }     
 
    imshow( "Result window", drawing );
    waitKey(0);                                         
    return 0;
}