#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(unsigned int argc, char* argv[]) {
	char* pathPlates;
	char* pathNotPlates;
	int numPlates = 71;
	int numNotPlates = 74;

	Mat classes;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;

	pathPlates = argv[1];
	pathNotPlates = argv[2];

	cout << "SVM trainer for LicensePlateRec." << endl;

	cout << "Enter number of plates..." << endl;
	cin >> numPlates;
	cout << "Enter number of not plates..." << endl;
	cin >> numNotPlates;

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

	return 0;
}
