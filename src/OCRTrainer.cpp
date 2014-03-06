#include "OCRTrainer.hpp"

const int OCRTrainer::charsFiles[] = {99, 74, 97, 86, 95, 101, 95, 79, 93, 88, 132, 140, 96, 16, 56, 65, 55, 20, 47, 46, 86, 19};

OCRTrainer::OCRTrainer() {

}

void OCRTrainer::run() {
	cout << "OCR trainer for LicensePlateRec." << endl;
	cout << "Enter path to directory with chars folders..." << endl;
	cin >> path;

	for(int i = 0; i < OCR::numChars; i++) {
		int numFiles = charsFiles[i];
		for(int j = 0; j < numFiles; j++) {
			stringstream ss;
			ss << path << "/" << OCR::chars[i] << "_" << j << ".jpg";

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

	FileStorage fs("OCRData___debug.xml", FileStorage::WRITE);
	fs << "TrainingDataF5" << trainingDataf5;
	fs << "TrainingDataF10" << trainingDataf10;
	fs << "TrainingDataF15" << trainingDataf15;
	fs << "TrainingDataF20" << trainingDataf20;
	fs << "TrainingDataF25" << trainingDataf25;
	fs << "classes" << classes;
	fs.release();
}

const string& OCRTrainer::getPath() const {
	return path;
}

void OCRTrainer::setPath(const string& path) {
	this->path = path;
}
