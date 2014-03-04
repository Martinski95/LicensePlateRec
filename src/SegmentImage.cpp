#include "SegmentImage.hpp"

SegmentImage::SegmentImage(string file) {
	this->filename = file;
}

const string& SegmentImage::getFilename() const {
	return filename;
}

void SegmentImage::setFilename(const string& filename) {
	this->filename = filename;
}

bool SegmentImage::checkSize(RotatedRect rect){
    float aspectRatio = 52.0f / 11.0f;
    float errorRate = 0.4;
    int minArea = 15 * aspectRatio * 15;
    int maxArea = 125 * aspectRatio * 125;

    float ratioMin = aspectRatio - (aspectRatio * errorRate);
    float ratioMax = aspectRatio + (aspectRatio * errorRate);

    int area = rect.size.height * rect.size.width;
    float ratio = (float)rect.size.width / (float)rect.size.height;
    if(ratio < 1) {
        ratio = 1/ratio;
    }

    if( ((area < minArea) || (area > maxArea)) || ((ratio < ratioMin) || (ratio > ratioMax)) ){
        return false;
    }
    return true;
}

Mat SegmentImage::croppingMask(RotatedRect rect, Mat inputImage) {
	float minSize;
	Mat mask;

	int connectivity = 4;
	int newMaskVal = 255;
	int NumSeeds = 10;
	Rect r;

	srand(time(0));

	if(rect.size.width < rect.size.height) {
		minSize = rect.size.width;
	} else {
		minSize = rect.size.height;
	}

	minSize = minSize - (minSize * 0.5);

	mask.create(inputImage.rows + 2, inputImage.cols + 2, CV_8UC1);
	mask = Scalar::all(0);

	int flags = connectivity + (newMaskVal << 8) + CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;
	for(int j = 0; j < NumSeeds; j++){
		Point seed;

		seed.x = rect.center.x + rand() % (int)minSize - (minSize/2);
		seed.y = rect.center.y + rand() % (int)minSize - (minSize/2);

		floodFill(inputImage, mask, seed, Scalar(255,0,0), &r, Scalar(30, 30, 30), Scalar(30, 30, 30), flags);
	}
	return mask;
}

Mat SegmentImage::processRect(Mat inputImage, RotatedRect minRect) {
	Mat imageRotated;
	Mat imageCropped;
	Mat plate;

	float ratio = (float)minRect.size.width / (float)minRect.size.height;
	float angle = minRect.angle;

	if(ratio < 1) {
		angle = 90 + angle;
	}
	Mat rotationMatrix = getRotationMatrix2D(minRect.center, angle, 1);

	warpAffine(inputImage, imageRotated, rotationMatrix, inputImage.size(), CV_INTER_CUBIC);

	Size rectSize = minRect.size;
	if(ratio < 1) {
		swap(rectSize.width, rectSize.height);
	}

	getRectSubPix(imageRotated, rectSize, minRect.center, imageCropped);

	plate.create(33,144, CV_8UC3);
	resize(imageCropped, plate, plate.size(), 0, 0, INTER_CUBIC);
	cvtColor(plate, plate, CV_BGR2GRAY);
	blur(plate, plate, Size(3,3));
	equalizeHist(plate, plate);
	return plate;
}

vector<LicensePlate> SegmentImage::segment(Mat inputImage) {
	vector<LicensePlate> outputImages;
	vector<vector<Point> > contours;
	Mat imageGray;
	Mat imageSobel;
	Mat imageThreshold;

	cvtColor(inputImage, imageGray, CV_BGR2GRAY);
	blur(imageGray, imageGray, Size(5,5));

	Sobel(imageGray, imageSobel, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);

	threshold(imageSobel, imageThreshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);

	Mat element = getStructuringElement(MORPH_RECT, Size(17, 3) );
	morphologyEx(imageThreshold, imageThreshold, CV_MOP_CLOSE, element);

	findContours(imageThreshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<RotatedRect> rects;
	vector<vector<Point> >::iterator it = contours.begin();
	while (it != contours.end()) {
		RotatedRect minBoundingRect = minAreaRect(Mat(*it));
		if(!checkSize(minBoundingRect)) {
			it = contours.erase(it);
		} else {
			++it;
			rects.push_back(minBoundingRect);
		}
	}

	Mat resultImage;
	inputImage.copyTo(resultImage);

	for(unsigned int i = 0; i < rects.size(); i++) {
		Mat mask = croppingMask(rects[i], inputImage);

		vector<Point> interestPoints;

		for(Mat_<uchar>::iterator itMask = mask.begin<uchar>() ; itMask != mask.end<uchar>(); ++itMask) {
			if(*itMask == 255) {
				interestPoints.push_back(itMask.pos());
			}
		}
		RotatedRect minRect = minAreaRect(interestPoints);

		if(checkSize(minRect)) {
			Mat imagePlate = processRect(inputImage, minRect);
			outputImages.push_back(LicensePlate(imagePlate, minRect.boundingRect()));
		}
	}
	return outputImages;
}

vector<LicensePlate> SegmentImage::run(Mat inputImage) {
	return segment(inputImage);
}
