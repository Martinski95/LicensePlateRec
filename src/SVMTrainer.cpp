#include "SVMTrainer.hpp"

SVMTrainer::SVMTrainer() {
	numNotPlates = 0;
	numPlates = 0;
}

void SVMTrainer::setPaths() {
	cout << "SVM trainer for LicensePlateRec." << endl;
	cout << "Enter number of plates..." << endl;
	cin >> numPlates;
	cout << "Enter number of not plates..." << endl;
	cin >> numNotPlates;
	cout << "Enter path to plates..." << endl;
	cin >> pathPlates;
	cout << "Enter path to not plates..." << endl;
	cin >> pathNotPlates;
}

void SVMTrainer::run() {
	setPaths();

	for(int i = 0; i < numPlates; i++) {
		stringstream ss;
		ss << pathPlates << i << ".jpg";

		Mat img = imread(ss.str(), 0);
		img = img.reshape(1, 1);

		trainingImages.push_back(img);
		trainingLabels.push_back(1);
	}

	for(int i = 0; i < numNotPlates; i++) {
		stringstream ss;
		ss << pathNotPlates << i << ".jpg";

		Mat img = imread(ss.str(), 0);
		img = img.reshape(1, 1);

		trainingImages.push_back(img);
		trainingLabels.push_back(0);
	}

	Mat(trainingImages).copyTo(trainingData);
	trainingData.convertTo(trainingData, CV_32FC1);

	Mat(trainingLabels).copyTo(classes);

	FileStorage fs("SVM.xml", FileStorage::WRITE);
	fs << "TrainingData" << trainingData;
	fs << "classes" << classes;
	fs.release();

}

int SVMTrainer::getNumNotPlates() const {
	return numNotPlates;
}

void SVMTrainer::setNumNotPlates(int numNotPlates) {
	this->numNotPlates = numNotPlates;
}

int SVMTrainer::getNumPlates() const {
	return numPlates;
}

void SVMTrainer::setNumPlates(int numPlates) {
	this->numPlates = numPlates;
}

const string& SVMTrainer::getPathNotPlates() const {
	return pathNotPlates;
}

void SVMTrainer::setPathNotPlates(const string& pathNotPlates) {
	this->pathNotPlates = pathNotPlates;
}

const string& SVMTrainer::getPathPlates() const {
	return pathPlates;
}

void SVMTrainer::setPathPlates(const string& pathPlates) {
	this->pathPlates = pathPlates;
}
