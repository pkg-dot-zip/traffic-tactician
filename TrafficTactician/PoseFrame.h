#pragma once
#include <opencv2/core.hpp>    // Basis OpenCV types zoals cv::Mat

class PoseFrame
{
public:
    // Singleton instance retrieval
    static PoseFrame& getInstance();

    // Set the frame
    void setFrame(const cv::Mat& frame);

    // Get the frame
    cv::Mat getFrame() const;

private:
    PoseFrame() = default; // Private constructor for singleton pattern

    // Delete copy constructor and assignment operator
    PoseFrame(const PoseFrame&) = delete;
    PoseFrame& operator=(const PoseFrame&) = delete;

    cv::Mat frame; // Store the frame
};

