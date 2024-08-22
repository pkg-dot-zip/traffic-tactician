#include "PoseFrame.h"

// Singleton instance retrieval
PoseFrame& PoseFrame::getInstance()
{
    static PoseFrame instance;
    return instance;
}

// Set the frame
void PoseFrame::setFrame(const cv::Mat& newFrame)
{
    newFrame.copyTo(frame);
}

// Get the frame
cv::Mat PoseFrame::getFrame() const
{
    return frame;
}
