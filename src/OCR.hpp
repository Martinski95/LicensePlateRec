#ifndef OCR_HPP
#define OCR_HPP

#include "LicensePlate.hpp"

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <ml.h>

#include <string.h>
#include <vector>

using namespace std;
using namespace cv;

#define HORIZONTAL 1
#define VERTICAL 0

class OCR {
	public:
		OCR();
		OCR(string trainingFile);

		static const int numChars;
		static const int strChars[];
		int charSize;

		string run(LicensePlate* input);
		Mat preprocessChar(Mat in);

};

#endif
