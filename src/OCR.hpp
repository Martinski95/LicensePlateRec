#ifndef OCR_HPP
#define OCR_HPP

#include "LicensePlate.hpp"
#include "CharSegment.hpp"

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <ml.h>

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
        static const char chars[];
		int charSize;

		bool run(LicensePlate* input);
		Mat preprocessChar(Mat input);
		Mat features(Mat input, int size);
		bool train(Mat trainingData, Mat classes, int numLayers);
		int classify(Mat input);
		bool trained();

	private:
		bool isTrained;
		CvANN_MLP  ann;
		Mat calcHistogram(Mat img, int t);
		vector<CharSegment> segment(LicensePlate input);
		bool checkSize(Mat rect);

};

#endif
