#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class CalibrateCamera {
private:

    std::vector<Mat> inputImages;

    struct CalibrationOuput { //future proofing the code in case i need more parameters. Right now all i need is focal length
        Mat cameraMatrix;
        Mat distCoeffs;
        Mat R;
        Mat T;
    };

public:
    CalibrateCamera();

    void addImage(const Mat& cal_img);

    double runCalibration(); //output = focal length

};