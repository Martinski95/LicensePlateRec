#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "SegmentImage.hpp"
#include "OCR.hpp"
#include "SVMClassifier.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	Mat image;
	string file;

	cout << "LicensePlateRec" << endl;

	cin >> file;
	image = imread(file, CV_LOAD_IMAGE_COLOR);

	if(!image.data) {
		cout << "No image data..." << endl;
		return -1;
	}

	SegmentImage segmentedImage(file);
	vector<LicensePlate> regions = segmentedImage.run(image);

	SVMClassifier svm("SVM.xml");
	vector<LicensePlate> plates;
	for(unsigned int i=0; i< regions.size(); i++)
	{
		Mat img = regions[i].plateImg;
		Mat p = img.reshape(1, 1);
		p.convertTo(p, CV_32FC1);

		if(svm.predict(p)) {
			plates.push_back(regions[i]);
		}
	}

	if(plates.size() < 1) {
		cout << "No plates detected." << endl;
		return 0;
	}

	OCR ocr("OCRData.xml");
	if(plates.size()) {
		for(unsigned int j = 0; j < plates.size(); j++) {
			LicensePlate plate = plates[j];
			ocr.run(&plate);
			for(unsigned int i = 0; i < plate.chars.size(); i++) {
				cout << "char " << i << " : " << plate.chars[i] << endl;
			}
			string licensePlate = plate.orderChars();
			cout << licensePlate << endl;
		}
	}

	return 0;
}
