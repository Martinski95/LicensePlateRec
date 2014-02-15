#ifndef SEGMENTIMAGE_HPP_
#define SEGMENTIMAGE_HPP_

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
		SegmentImage();

		string filename;

		vector<LicensePlate> run(Mat inputImage);
		const string& getFilename() const;
		void setFilename(const string& filename);
		bool checkSize(RotatedRect chunk);

	private:
		vector<LicensePlate> segment(Mat inputImage);

};

#endif
