#include "CharSegment.hpp"

CharSegment::CharSegment() {

}

CharSegment::CharSegment(Mat image, Rect position){
	img = image;
    pos = position;
}
