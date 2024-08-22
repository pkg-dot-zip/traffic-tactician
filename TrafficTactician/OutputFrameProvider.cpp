#include "IOutputFrameProvider.h"

class OutputFrameProvider : public IOutputFrameProvider
{
private:
    std::shared_ptr<Texture> outputFrame;

public:
    void setOutputFrame(const std::shared_ptr<Texture>& frame) override
    {
        outputFrame = frame;
    }

    std::shared_ptr<Texture> getOutputFrame() const override
    {
        return outputFrame;
    }
};
