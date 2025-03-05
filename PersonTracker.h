#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>



using namespace cv;

class PersonTracker {
private:
    Mat prev_gray;
    std::vector<Point2f> features;
    Rect bounding_box;
    TermCriteria criteria;

public:
    PersonTracker(Mat& frame, Rect bbox);

    void detectFeatures();

    void updateBbox(Rect new_bounding_box);

    std::vector<Point2f> calculateLK(Mat& frame);
};