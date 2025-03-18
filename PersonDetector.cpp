#include "PersonDetector.h"
#include <string>

using namespace cv;


PersonDetector::PersonDetector(std::string modelPath, std::string configPath, KalmanDistanceFilter dkf) : 
    mPath(modelPath), cPath(configPath), distanceFilter(dkf){
    blobSize = Size(300, 300);
    meanVal = Scalar(127.5, 127.5, 127.5);
    
    net = dnn::readNetFromTensorflow(mPath, cPath);
    net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(dnn::DNN_TARGET_CPU); 
}

Rect PersonDetector::detectPeople(Mat& frame){
    dnn::blobFromImage(frame, blob, 1.0, blobSize, meanVal, true, false);
    net.setInput(blob);
    net.forward(output);
    Mat detections(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    std::vector<Rect> detected;

    for (int i = 0; i < detections.rows; i++) {
        float confidence = detections.at<float>(i, 2);
        if (confidence > 0.5) {  // Set higher threshold to filter noise
            int class_id = static_cast<int>(detections.at<float>(i, 1));
            if (class_id == 1) {  // Class 1 = Person
                int left = static_cast<int>(detections.at<float>(i, 3) * frame.cols);
                int top = static_cast<int>(detections.at<float>(i, 4) * frame.rows);
                int right = static_cast<int>(detections.at<float>(i, 5) * frame.cols);
                int bottom = static_cast<int>(detections.at<float>(i, 6) * frame.rows);
                rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 2);
                detected.emplace_back(Rect(left, top, right - left, bottom - top));
            }
        }
    }

    if(detected.size() == 1){
        return detected[0];
    }

    if(detected.empty()){
        return Rect();
    }

    // Rect bestMatch;
    // double minDist = std::numeric_limits<double>::max();

    // for(auto& b : detected){
    //     Point tempC
    // }

    return Rect();

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

DetectionOutput PersonDetector::processFrame(Mat& frame) {
    auto box = detectPeople(frame);

    Position personOffset;
    double realDist;

    if(box.width > 0){
        personOffset = getPersonOffset(frame, box);
        realDist = getDistance(box);

        rectangle(frame, box, Scalar(0,255,0), 2);
        
        putText(frame, std::to_string(realDist), Point(box.x, box.y -10),
                FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,255,0),2);
    }

    DetectionOutput res = {personOffset, realDist, box};

    return res;
}

double PersonDetector::getDistance(const Rect& box) {

    if(box.width <= 0){
        if(distanceFilter.isInitialized()){
            return distanceFilter.process(0,false);
        }
        else{
            return -1;
        }
    }
    double f = 4.0*1280 / 3.58; //focal length of Logitect C270 camera, will need to change for others based on calibration process

    double pixelHeight = box.height;

    double realHeight= 1.88; //assume average width of person is like 21 in 


    //similar triangles math

    double theoretical_distance = (f * realHeight) / pixelHeight;

    return distanceFilter.process(theoretical_distance);

    //interpolate to find closest calibrated values

    auto upper = calibratedDistances.lower_bound(theoretical_distance);
    auto lower = std::prev(upper);020

}


