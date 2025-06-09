/**
 * @file RotationSource.h
 * @author Jaylon Sifuentes
 *
 * Class that represents a rotation source.
 */

#ifndef ROTATIONSOURCE_H
#define ROTATIONSOURCE_H
#include "IRotationSink.h"

class IRotationSink;

/**
 * Objects of this class represent a rotation source.
 * A rotation source belongs to a component (who feeds it rotation)
 * and is used to convey rotation to its connected sinks.
 */
class RotationSource
{
private:
    /// Source rotation to be applied to sinks
    double mSourceRotation = 0;

    /// Rotation sinks receiving their rotation from this source
    std::vector<std::shared_ptr<IRotationSink>> mSinks;

public:
    /**
     * Rotation source default constructor
     */
    RotationSource();

    /// delete the copy constructor
    RotationSource(const RotationSource&) = delete;

    /// delete the copy assignment operator
    RotationSource& operator=(const RotationSource&) = delete;

    /**
     * Set the rotation of the source and notify any associated sinks.
     * @param rotation New rotation to update to
     */
    void SetRotation(double rotation)
    {
        mSourceRotation = rotation;
        for (auto sink : mSinks)
        {
            sink->UpdateRotation(mSourceRotation);
        }
    }

    /**
     * Add a sink to the rotation source so it may begin receiving its rotation.
     * @param sink Sink component to receive rotation
     */
    void AddSink(std::shared_ptr<IRotationSink> sink)
    {
        mSinks.push_back(sink);
    }
};


#endif //ROTATIONSOURCE_H
