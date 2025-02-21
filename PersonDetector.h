#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

struct Position {
    int dx;
    int dy;
};

class PersonDetector {
private:
    HOGDescriptor hog;
    int frameWidth;
    double scale;
    Size winStride;
    Size padding;
public:
    PersonDetector(int width);

    std::vector<Rect> detectPeople(Mat& frame);

    Position getPersonOffset(Mat& frame, Rect& personBox);

    std::pair<Mat, Position> processFrame(Mat& frame);

    double getDistance(const Rect& box) const;
};