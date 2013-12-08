#include "SegmentImage.hpp"

SegmentImage::SegmentImage() {

}

const string& SegmentImage::getFilename() const {
	return filename;
}

void SegmentImage::setFilename(const string& filename) {
	this->filename = filename;
}

vector<LicensePlate> SegmentImage::segment(Mat inputImage) {
	vector<LicensePlate> outputImages;

	Mat imageGray;
	Mat imageSobel;
	Mat imageThreshold;

	cvtColor(inputImage, imageGray, CV_BGR2GRAY);
	blur(imageGray, imageGray, Size(5,5));
	imshow("Gray image.", imageGray);

	Sobel(imageGray, imageSobel, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	imshow("Sobel image.", imageSobel);

	threshold(imageSobel, imageThreshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
	imshow("Threshold image.", imageThreshold);

	return outputImages;
}

vector<LicensePlate> SegmentImage::run(Mat inputImage) {
	return segment(inputImage);
}
