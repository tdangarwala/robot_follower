#include "PersonDetector.h"
#include <string>

using namespace cv;


PersonDetector::PersonDetector(int width) :  frameWidth(width), scale(1.05),
    winStride(4,4), padding(8,8){
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
}

std::vector<Rect> PersonDetector::detectPeople(Mat& frame){
    Mat resized;
    resize(frame, resized, Size(frameWidth, frame.rows * frameWidth / frame.cols));

    Mat gray;
    cvtColor(resized, gray, COLOR_BGR2GRAY);

    std::vector<Rect> boxes;
    //std::vector<double> weights;
    hog.detectMultiScale(gray, boxes, 0, winStride, padding, scale, 1.5);

    if(!boxes.empty()){
        float scaleFactor = static_cast<float>(frame.cols) / frameWidth;
        for(auto& box : boxes){
            box.x *= scaleFactor;
            box.y *= scaleFactor;
            box.width *= scaleFactor;
            box.height *= scaleFactor;
        }
    }

    return boxes;
}

Position PersonDetector::getPersonOffset(Mat& frame, Rect& personBox){
    Position pos;

    Point frameCenter(frame.cols / 2, frame.rows / 2);
    Point personCenter(personBox.x + personBox.width / 2, 
        personBox.y + personBox.height/2);
    
    pos.dx = personCenter.x - frameCenter.x;
    pos.dy = personCenter.y - frameCenter.y;


    return pos;

}

std::pair<Mat, Position> PersonDetector::processFrame(Mat& frame) {
    auto boxes = detectPeople(frame);

    Position personOffset;

    if(!boxes.empty()){
        auto largestBox = *std::max_element(boxes.begin(), boxes.end(), 
        [](const Rect& a, const Rect& b){
            return a.area() < b.area();
        });

        personOffset = getPersonOffset(frame, largestBox);
        double realDist = getDistance(largestBox);

        rectangle(frame, largestBox, Scalar(0,255,0), 2);
        Point center(largestBox.x + largestBox.width/2,
            largestBox.y + largestBox.height/2);
        circle(frame, center, 4, Scalar(0,0,255), -1);

        putText(frame, std::to_string(realDist), Point(largestBox.x, largestBox.y -10),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0),2);

    }

    return {frame, personOffset};
}

double PersonDetector::getDistance(const Rect& box) const{
    double f = 4.0; //focal length of Logitect C270 camera, will need to change for others based on calibration process

    double pixelWidth = box.width;

    double realWidth = 0.38; //assume average width of person is like 35-40 cm


    //similar triangles math

    double distance = (f/1000 * realWidth) / pixelWidth;

    return distance;

}
