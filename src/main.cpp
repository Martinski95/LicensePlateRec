#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	Mat image;
	string file;

	cin >> file;

	image = imread(file, CV_LOAD_IMAGE_COLOR);
	if(!image.data) {
		cout << "No image data..." << endl;
		return -1;
	}

	namedWindow("LicensePlateRec", CV_WINDOW_AUTOSIZE);

	imshow("LicensePlateRec", image);

	waitKey(0);

	return 0;
}
