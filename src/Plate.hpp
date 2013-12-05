#ifndef PLATE_HPP
#define PLATE_HPP

#include <cv.h>

#include <string>
#include <vector>

using namespace std;
using namespace cv;

class Plate {
	public:
		Plate();
		Plate(Mat img, Rect pos);

		string str();

		Rect platePos;
		Mat plateImg;

		vector<char> chars;
		vector<Rect> charsPositions;
};

#endif
