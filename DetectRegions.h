#ifndef DetectRegions_h
#define DetectRegions_h

#include <string.h>
#include <vector>

#include "Plate.h"

#include <imgproc.hpp>
#include <highgui.hpp>
//#include <cvaux.h>

using namespace std;
using namespace cv;

class DetectRegions{
    public:
        DetectRegions();
        string filename;
        void setFilename(string f);
        bool saveRegions;
        bool showSteps;
        vector<Plate> run(Mat input);
    private:
        vector<Plate> segment(Mat input);
        bool verifySizes(RotatedRect mr);
        Mat histeq(Mat in);
};

#endif
