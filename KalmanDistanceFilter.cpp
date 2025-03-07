#include "KalmanDistanceFilter.h"


KalmanDistanceFilter::KalmanDistanceFilter(float process_noise, float measurement_noise):
    process_noise(process_noise) , measurement_noise(measurement_noise), initialized(false) {
        kf = cv::KalmanFilter(2,1,0);

        kf.transitionMatrix = (cv::Mat_<float>(2,2) << 1,1,0,1);

        kf.measurementMatrix = (cv::Mat_<float>(1,2) << 1,0);

        kf.processNoiseCov = (cv::Mat_<float>(2,2) << process_noise,0,0,process_noise);

        kf.measurementNoiseCov = (cv::Mat_<float>(1,1) << measurement_noise);

        kf.statePost = (cv::Mat_<float>(2,1) << 2.0,0);


    }

void KalmanDistanceFilter::init(float initialDistance) {
    kf.statePost = (cv::Mat_<float>(2, 1) << initialDistance, 0);
    initialized = true;
}

void KalmanDistanceFilter::reset(float distance) {
    // Reset with higher confidence
    kf.errorCovPost = (cv::Mat_<float>(2, 2) << 0.1, 0, 0, 0.1);
    kf.statePost = (cv::Mat_<float>(2, 1) << distance, 0);
    initialized = true;
}

float KalmanDistanceFilter::predict() {
    cv::Mat prediction = kf.predict();
    return prediction.at<float>(0);
}

float KalmanDistanceFilter::update(float measuredDistance) {
    cv::Mat_<float> measurement(1, 1);
    measurement(0) = measuredDistance;
    kf.correct(measurement);
    return kf.statePost.at<float>(0);
}

float KalmanDistanceFilter::process(float measuredDistance, bool validMeasurement) {
    // First predict
    float predictedDistance = predict();
    
    // If not initialized and have valid measurement, initialize
    if (!initialized && validMeasurement) {
        init(measuredDistance);
    }
    
    // If we have a valid measurement, update the filter
    if (validMeasurement) {
        return update(measuredDistance);
    }
    
    // Otherwise return just the prediction
    return predictedDistance;
}