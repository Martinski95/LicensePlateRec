#include "LicensePlate.hpp"

LicensePlate::LicensePlate() {
//...
}

LicensePlate::LicensePlate(Mat img, Rect pos) {
	plateImg = img;
	platePos = pos;
}

string LicensePlate::str() {

	return "";
}
