/**
 * @file IRotationSink.h
 * @author Jaylon Sifuentes
 *
 * Class that represents rotation sinks.
 */

#ifndef ROTATIONSINK_H
#define ROTATIONSINK_H


/**
 * An object of this class represents a rotation sink.
 * Rotation sinks receive rotation from a source!
 */
class IRotationSink
{
private:

public:
    /**
     * Updates rotation of components respective to the source they are a sink to.
     * Also used to convey rotation to a components owned source.
     * @param rotation Rotation received from some source
     */
    virtual void UpdateRotation(double rotation) = 0;
};


#endif //ROTATIONSINK_H
