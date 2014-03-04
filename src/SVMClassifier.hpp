#ifndef SVM_CLASSIFIER_HPP
#define SVM_CLASSIFIER_HPP

#include <iostream>
#include <cv.h>
#include <cvaux.h>

using namespace std;
using namespace cv;

class SVMClassifier {
	public:
		SVMClassifier();
		SVMClassifier(string trainingFile);

		int predict(Mat image);

	private:
		string trainingFile;
		Mat trainingData;
		Mat classes;
		CvSVMParams parameters;
		CvSVM classifier;

		void setParameters();
};

#endif
