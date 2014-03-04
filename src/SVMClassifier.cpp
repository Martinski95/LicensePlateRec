#include "SVMClassifier.hpp"

SVMClassifier::SVMClassifier(string trainingFile) {
	this->trainingFile = trainingFile;

	FileStorage fs;
	fs.open(trainingFile, FileStorage::READ);

	fs["TrainingData"] >> trainingData;
	fs["classes"] >> classes;

	setParameters();
}

void SVMClassifier::setParameters() {
	parameters.svm_type = CvSVM::C_SVC;
	parameters.kernel_type = CvSVM::LINEAR;
	parameters.degree = 0;
	parameters.gamma = 1;
	parameters.coef0 = 0;
	parameters.C = 1;
	parameters.nu = 0;
	parameters.p = 0;
	parameters.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
}

int SVMClassifier::predict(Mat image) {
	CvSVM svmClassifier(trainingData, classes, Mat(), Mat(), parameters);
	return (int)svmClassifier.predict(image);
}
