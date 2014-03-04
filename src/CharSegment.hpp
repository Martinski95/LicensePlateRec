#ifndef CHAR_SEGMENT_HPP
#define CHAR_SEGMENT_HPP

#include <cv.h>

using namespace cv;

class CharSegment {
	public:
		CharSegment();
		CharSegment(Mat image, Rect position);

		Mat img;
		Rect pos;
};

#endif
