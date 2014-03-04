#ifndef LICENSE_PLATE_HPP
#define LICENSE_PLATE_HPP

#include <cv.h>
#include <vector>

using namespace std;
using namespace cv;

class LicensePlate {
	public:
		LicensePlate();
		LicensePlate(Mat img, Rect pos);

		string orderChars();

		Rect platePos;
		Mat plateImg;

		vector<char> chars;
		vector<Rect> charsPositions;
};

#endif
