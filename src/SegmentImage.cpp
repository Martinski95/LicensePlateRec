#include "SegmentImage.hpp"

SegmentImage::SegmentImage() {

}

const string& SegmentImage::getFilename() const {
	return filename;
}

void SegmentImage::setFilename(const string& filename) {
	this->filename = filename;
}

bool SegmentImage::checkSize(RotatedRect chunk){
    float plateHeight = 11;
    float plateWidth = 52;
    float aspectRatio = plateWidth / plateHeight;
    float errorRate = 0.3;
    int minArea = 15*15 * aspectRatio;
    int maxArea = 125*125 * aspectRatio;

    float ratioMin= aspectRatio - aspectRatio * errorRate;
    float ratioMax= aspectRatio + aspectRatio * errorRate;

    int area = chunk.size.height * chunk.size.width;
    float ratio = (float)chunk.size.width / (float)chunk.size.height;
    if(ratio < 1)
        ratio = 1/ratio;

    if( ((area < minArea) || (area > maxArea)) || ((ratio < ratioMin) || (ratio > ratioMax)) ){
        return false;
    }else{
        return true;
    }

}

vector<LicensePlate> SegmentImage::segment(Mat inputImage) {
	vector<LicensePlate> outputImages;
	vector<vector<Point> > contours;
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

	Mat element = getStructuringElement(MORPH_RECT, Size(17, 3) );
	morphologyEx(imageThreshold, imageThreshold, CV_MOP_CLOSE, element);
	imshow("Close image.", imageThreshold);

	findContours(imageThreshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	cout << contours.size() << endl;

	vector<RotatedRect> rects;
	for(vector<vector<Point> >::iterator it = contours.begin(); it != contours.end(); ++it) {
		RotatedRect rect = minAreaRect(Mat(*it));
		if(!checkSize(rect)) {
			it = contours.erase(it);
		} else {
			++it;
			rects.push_back(rect);
		}
	}

	return outputImages;
}

vector<LicensePlate> SegmentImage::run(Mat inputImage) {
	return segment(inputImage);
}
