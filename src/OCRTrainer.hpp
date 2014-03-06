#ifndef OCR_TRAINER_HPP
#define OCR_TRAINER_HPP

#include "OCR.hpp"

#include <vector>

using namespace std;
using namespace cv;

class OCRTrainer {
	public:
		OCRTrainer();

		void run();
		const string& getPath() const;
		void setPath(const string& path);

	private:
		static const int charsFiles[];

		string path;
		Mat classes;
		Mat trainingDataf5;
		Mat trainingDataf10;
		Mat trainingDataf15;
		Mat trainingDataf20;
		Mat trainingDataf25;
		vector<int> trainingLabels;
		OCR ocr;
};

#endif
