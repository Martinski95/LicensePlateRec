#include "CharSegment.hpp"

CharSegment::CharSegment() {

}

CharSegment::CharSegment(Mat image, Rect position){
	img = image;
    pos = position;
}

const Mat& CharSegment::getImg() const {
	return img;
}

void CharSegment::setImg(const Mat& img) {
	this->img = img;
}

const Rect& CharSegment::getPos() const {
	return pos;
}

void CharSegment::setPos(const Rect& pos) {
	this->pos = pos;
}
