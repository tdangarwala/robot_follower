#include <opencv2/video/tracking.hpp>
#include <opencv2/core.hpp>

class KalmanDistanceFilter {
private:
    float process_noise;
    float measurement_noise;
    bool initialized;
    cv::KalmanFilter kf;  

public:
    KalmanDistanceFilter(float process_noise, float measurement_noise);


    void init(float initial_distance);

    void reset(float distance);

    float predict();

    float update(float measured_distance);

    float process(float measured_distance, bool valid_measurement = true);

    bool isInitialized() const { return initialized;}

};