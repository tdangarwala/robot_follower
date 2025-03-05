#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

struct Position {
    int dx;
    int dy;
};

class PersonDetector {
private:
    //Point prevCenter;
    std::string mPath;
    std::string cPath;
    Size blobSize;
    Scalar meanVal;
    dnn::Net net;
    Mat blob;
    Mat output;
    
    double emaDist;
public:
    PersonDetector(std::string modelPath, std::string configPath);

    Rect detectPeople(Mat& frame);

    Position getPersonOffset(Mat& frame, Rect& personBox);

    std::pair<Rect, Position> processFrame(Mat& frame);

    double getDistance(const Rect& box);

    double filterDistance(double newDistance);

    void resetEmaDist();
};