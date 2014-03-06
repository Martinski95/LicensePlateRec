#include "OCR.hpp"
#include "CharSegment.hpp"

const char OCR::chars[] = {'0','1','2','3','4','5','6','7','8','9','A', 'B', 'C', 'E', 'H', 'K', 'M', 'O', 'P', 'T', 'X', 'Y'};
const int OCR::numChars = 22;

OCR::OCR() {
	charSize = 25;
	isTrained = false;
}

OCR::OCR(string trainingFile) {
	charSize = 25;
	isTrained = false;

	FileStorage fs;
	fs.open(trainingFile, FileStorage::READ);

	Mat trainingData;
	Mat classes;

	fs["TrainingDataF25"] >> trainingData;
	fs["classes"] >> classes;

	train(trainingData, classes, 15);
}

bool OCR::train(Mat trainingData, Mat classes, int numLayers) {
	Mat layers(1,3,CV_32SC1);
	layers.at<int>(0)= trainingData.cols;
	layers.at<int>(1)= numLayers;
	layers.at<int>(2)= numChars;

	ann.create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);

	Mat trainClasses;
	trainClasses.create(trainingData.rows, numChars, CV_32FC1);
	for(int i = 0; i < trainClasses.rows; i++) {
		for(int k = 0; k < trainClasses.cols; k++){
			if(k == classes.at<int>(i)) {
				trainClasses.at<float>(i,k) = 1;
			} else {
				trainClasses.at<float>(i,k) = 0;
			}
		}
	}

	Mat weights(1, trainingData.rows, CV_32FC1, Scalar::all(1));

	int res = ann.train(trainingData, trainClasses, weights);
	if(res > 0) {
		isTrained = true;
		return true;
	}
	return false;
}

Mat OCR::calcHistogram(Mat img, int t) {
	int size;
	if(HORIZONTAL) {
		size = img.rows;
	} else {
		size = img.cols;
	}

	Mat mhist = Mat::zeros(1, size, CV_32F);

	for(int i = 0; i < size; i++){
		Mat data;
		if(HORIZONTAL) {
			data = img.row(i);
		} else {
			data = img.col(i);
		}

		mhist.at<float>(i) = countNonZero(data);
	}

	double min, max;
	minMaxLoc(mhist, &min, &max);

	if(max > 0) {
		mhist.convertTo(mhist, -1 , 1.0f/max);
	}

	return mhist;
}

Mat OCR::features(Mat input, int size) {
	Mat verticalHist = calcHistogram(input, VERTICAL);
	Mat horizontalHist = calcHistogram(input, HORIZONTAL);

	Mat lowData;
	resize(input, lowData, Size(size, size));

	int numCols = verticalHist.cols + horizontalHist.cols + lowData.cols*lowData.cols;

	Mat output = Mat::zeros(1, numCols, CV_32F);
	int j = 0;

	for(int i = 0; i < verticalHist.cols; i++) {
		output.at<float>(j) = verticalHist.at<float>(i);
		j++;
	}

	for(int i = 0; i < horizontalHist.cols; i++) {
		output.at<float>(j) = horizontalHist.at<float>(i);
		j++;
	}

	for(int x=0; x<lowData.cols; x++) {
		for(int y = 0; y < lowData.rows; y++){
			output.at<float>(j) = (float)lowData.at<unsigned char>(x,y);
			j++;
		}
	}
	return output;
}

bool OCR::checkSize(Mat rect) {
	float aspectRatio = 45.0f / 75.0f;
	float charAspectRatio = (float)rect.cols / (float)rect.rows;
	float error = 0.35;

	float minHeight = 15;
	float maxHeight = 28;

	float minAspectRatio = 0.2;
	float maxAspectRatio = aspectRatio + aspectRatio*error;

	float charArea = countNonZero(rect);
	float imageArea = rect.cols * rect.rows;
	float percPixels = charArea / imageArea;

	if((percPixels < 0.8) &&
		(charAspectRatio > minAspectRatio) &&
		(charAspectRatio < maxAspectRatio) &&
		(rect.rows >= minHeight) &&
		(rect.rows < maxHeight)) {
    	return true;
    }
	return false;
}


Mat OCR::preprocessChar(Mat input) {
	Mat output;
	int height = input.rows;
	int width = input.cols;
	Mat transformMatrice = Mat::eye(2,3,CV_32F);
	int m = max(width ,height);
	transformMatrice.at<float>(0,2) = (m/2) - (width/2);
	transformMatrice.at<float>(1,2) = (m/2) - (height/2);

	Mat warpImage(m, m, input.type());
	warpAffine(input, warpImage, transformMatrice, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

	resize(warpImage, output, Size(charSize, charSize));

	return output;
}

vector<CharSegment> OCR::segment(LicensePlate plate) {
	Mat input = plate.plateImg;
	vector<CharSegment> output;
	Mat imageThreshold;
	Mat imageContours;

	threshold(input, imageThreshold, 60, 255, CV_THRESH_BINARY_INV);
	imageThreshold.copyTo(imageContours);

	vector<vector<Point> > contours;
	findContours(imageContours, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	Mat result;
	imageThreshold.copyTo(result);
	cvtColor(result, result, CV_GRAY2RGB);

	vector<vector<Point> >::iterator it = contours.begin();
	while(it != contours.end()) {
		Rect minBoundingRect = boundingRect(Mat(*it));
		rectangle(result, minBoundingRect, Scalar(0, 255, 0));

		Mat segment(imageThreshold, minBoundingRect);
		if(checkSize(segment)) {
			segment = preprocessChar(segment);
			output.push_back(CharSegment(segment, minBoundingRect));
		}
		++it;
	}

	return output;
}

int OCR::classify(Mat input) {
	Mat output(1, numChars, CV_32FC1);
	ann.predict(input, output);

	Point maxLoc;
	double maxVal;
	minMaxLoc(output, 0, &maxVal, 0, &maxLoc);

	return maxLoc.x;
}

bool OCR::trained() {
	return isTrained;
}

bool OCR::run(LicensePlate* input) {
	vector<CharSegment> segments = segment(*input);

	for(unsigned int i = 0; i < segments.size(); i++) {
		Mat c = preprocessChar(segments[i].getImg());
		Mat f = features(c, 25);
		int character = classify(f);
		input->chars.push_back(chars[character]);
		input->charsPositions.push_back(segments[i].getPos());
	}

	return true;
}
