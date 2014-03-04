#ifndef SEGMENT_IMAGE_HPP
#define SEGMENT_IMAGE_HPP

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include "LicensePlate.hpp"

#include <vector>
#include <cstring>
#include <string>

using namespace std;
using namespace cv;

class SegmentImage {
	public:
		SegmentImage(string file);

		vector<LicensePlate> run(Mat inputImage);
		const string& getFilename() const;
		void setFilename(const string& filename);
		bool checkSize(RotatedRect rect);

	private:
		string filename;
		vector<LicensePlate> segment(Mat inputImage);
		Mat croppingMask(RotatedRect rect, Mat inputImage);
		Mat processRect(Mat inputImage, RotatedRect minRect);
};

#endif
