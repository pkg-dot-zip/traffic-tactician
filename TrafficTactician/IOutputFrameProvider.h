#pragma once
#include <memory>
#include "Texture.h"
class IOutputFrameProvider
{
public:
    virtual ~IOutputFrameProvider() = default;
    // Methode om het frame in te stellen
    virtual void setOutputFrame(const std::shared_ptr<Texture>& frame) = 0;
    // Methode om het frame op te halen
    virtual std::shared_ptr<Texture> getOutputFrame() const = 0;
};