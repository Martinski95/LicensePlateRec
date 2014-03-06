#ifndef SVM_TRAINER_HPP
#define SVM_TRAINER_HPP

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class SVMTrainer {
	public:
		SVMTrainer();

		int getNumNotPlates() const;
		void setNumNotPlates(int numNotPlates);

		int getNumPlates() const;
		void setNumPlates(int numPlates);

		const string& getPathNotPlates() const;
		void setPathNotPlates(const string& pathNotPlates);

		const string& getPathPlates() const;
		void setPathPlates(const string& pathPlates);

		void run();

	private:
		string pathPlates;
		string pathNotPlates;
		int numPlates;
		int numNotPlates;

		Mat classes;
		Mat trainingData;
		Mat trainingImages;
		vector<int> trainingLabels;

		void setPaths();
};

#endif
