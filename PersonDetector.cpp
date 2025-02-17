#include "PersonDetector.h"

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
    //bool detectedPerson = false;

    if(!boxes.empty()){
        auto largestBox = *std::max_element(boxes.begin(), boxes.end(), 
        [](const Rect& a, const Rect& b){
            return a.area() < b.area();
        });

        personOffset = getPersonOffset(frame, largestBox);

        rectangle(frame, largestBox, Scalar(0,255,0), 2);
        Point center(largestBox.x + largestBox.width/2,
            largestBox.y + largestBox.height/2);
        circle(frame, center, 4, Scalar(0,0,255), -1);


        //trackingDirection = getTrackingDirection(center.x, frame.cols);

        //detectedPerson = true;
    }

    return {frame, personOffset};
}
