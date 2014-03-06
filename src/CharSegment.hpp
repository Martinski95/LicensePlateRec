#ifndef CHAR_SEGMENT_HPP
#define CHAR_SEGMENT_HPP

#include <cv.h>

using namespace cv;

class CharSegment {
	public:
		CharSegment();
		CharSegment(Mat image, Rect position);

		const Mat& getImg() const;
		void setImg(const Mat& img);
		const Rect& getPos() const;
		void setPos(const Rect& pos);

	private:
		Mat img;
		Rect pos;
};

#endif
