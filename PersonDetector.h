#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

#include "KalmanDistanceFilter.h"

using namespace cv;

struct Position {
    int dx;
    int dy;
};

struct DetectionOutput {
    Position pan_pos;
    double distance;
    Rect boundingBox;
};

class PersonDetector {
private:
    //Point prevCenter;
    std::string mPath;
    std::string cPath;
    KalmanDistanceFilter distanceFilter;
    Size blobSize;
    Scalar meanVal;
    dnn::Net net;
    Mat blob;
    Mat output;

    

public:
    PersonDetector(std::string modelPath, std::string configPath, KalmanDistanceFilter dkf);

    Rect detectPeople(Mat& frame);

    Position getPersonOffset(Mat& frame, Rect& personBox);

    DetectionOutput processFrame(Mat& frame);

    double getDistance(const Rect& box);
};