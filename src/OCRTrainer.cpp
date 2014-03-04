#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include "OCR.hpp"

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

const int charsFiles[22] = {99, 74, 97, 86, 95, 101, 95, 79, 93, 88, 132, 140, 96, 16, 56, 65, 55, 20, 47, 46, 86, 19};

int main() {
	cout << "OCR trainer for LicensePlateRec." << endl;

	string path;
	cout << "Enter path to directory with chars folders..." << endl;
	cin >> path;

	Mat classes;
	Mat trainingDataf5;
	Mat trainingDataf10;
	Mat trainingDataf15;
	Mat trainingDataf20;
	Mat trainingDataf25;

	vector<int> trainingLabels;
	OCR ocr;

	for(int i = 0; i < OCR::numChars; i++) {
		cout << "i = " << i << " numChars == " << OCR::numChars << endl;

		int numFiles = charsFiles[i];
		cout << "numFiles == " << numFiles << endl;
		cin.get();
		for(int j = 0; j < numFiles; j++) {
			cout << "Char: " << OCR::chars[i] << " file: " << j << endl;
			stringstream ss;
			ss << path << "/" << OCR::chars[i] << "_" << j << ".jpg";
			cout << "path: " << ss.str() << endl;
			Mat image = imread(ss.str(), 0);

			Mat f5 = ocr.features(image, 5);
			Mat f10 = ocr.features(image, 10);
			Mat f15 = ocr.features(image, 15);
			Mat f20 = ocr.features(image, 20);
			Mat f25 = ocr.features(image, 25);

			trainingDataf5.push_back(f5);
			trainingDataf10.push_back(f10);
			trainingDataf15.push_back(f15);
			trainingDataf20.push_back(f20);
			trainingDataf25.push_back(f25);

			trainingLabels.push_back(i);
		}
	}

	trainingDataf5.convertTo(trainingDataf5, CV_32FC1);
	trainingDataf10.convertTo(trainingDataf10, CV_32FC1);
	trainingDataf15.convertTo(trainingDataf15, CV_32FC1);
	trainingDataf20.convertTo(trainingDataf20, CV_32FC1);
	trainingDataf25.convertTo(trainingDataf25, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);

	FileStorage fs("OCRData.xml", FileStorage::WRITE);
	fs << "TrainingDataF5" << trainingDataf5;
	fs << "TrainingDataF10" << trainingDataf10;
	fs << "TrainingDataF15" << trainingDataf15;
	fs << "TrainingDataF20" << trainingDataf20;
	fs << "TrainingDataF25" << trainingDataf25;
	fs << "classes" << classes;
	fs.release();

	return 0;
}

