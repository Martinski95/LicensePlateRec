#include "LicensePlate.hpp"

LicensePlate::LicensePlate() {

}

LicensePlate::LicensePlate(Mat img, Rect pos) {
	plateImg = img;
	platePos = pos;
}

string LicensePlate::orderChars() {
	string result = "";
	vector<int> orderIndices;
	vector<int> positions;
	for(unsigned int i = 0; i < charsPositions.size(); i++) {
		orderIndices.push_back(i);
		positions.push_back(charsPositions[i].x);
	}

	int minValue;
	int minId;
	for(unsigned int i = 0; i < positions.size(); i++) {
		minValue = positions[i];
		minId = i;

		for(unsigned int j = i; j < positions.size(); j++) {
			if(positions[j] < minValue) {
				minValue = positions[j];
				minId = j;
			}
		}

		int id = orderIndices[i];
		int idMin = orderIndices[minId];
		orderIndices[i] = idMin;
		orderIndices[minId] = id;

		int posId = positions[i];
		int posIdMin = positions[minId];
		positions[i] = posIdMin;
		positions[minId] = posId;
	}
	for(unsigned int i = 0; i < orderIndices.size(); i++){
		result += chars[orderIndices[i]];
	}
	return result;
}
