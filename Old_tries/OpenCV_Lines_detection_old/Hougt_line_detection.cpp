#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demonstrates line finding with the Hough transform.\n"
        "Usage:\n"
        "./houghlines <image_name>, Default is pic1.png\n" << endl;
}

int HoughLinesP_function(int argc, char** argv) /// rename to main BEFORE USING THIS
{
    const char* filename = argc >= 2 ? argv[1] : "DSC09716.JPG";

    Mat src = imread(filename, 0);
    if (src.empty())
    {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }

    Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);
    vector<Vec4i> lines;
	lines.reserve(1000);
    HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 110, 10);

    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);

        Point p1, p2;
        p1 = Point(l[0], l[1]);
        p2 = Point(l[2], l[3]);
        //calculate angle in radian,  if you need it in degrees just do angle * 180 / PI
        double angle = atan2(p1.y - p2.y, p1.x - p2.x);
        double angles = angle * 180 / 3.14159265358979323846;
        cout << "line coordinates are " << l << endl;
        cout << "Angles are " << angles << endl;
    }

    imshow("source", src);
    imshow("detected lines", cdst);


    waitKey();

    return 0;
}