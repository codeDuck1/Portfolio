/**
 * @file Pulley.h
 * @author Jaylon Sifuentes
 *
 * Class that represents a pulley component.
 */

#ifndef PULLEY_H
#define PULLEY_H
#include "Component.h"
#include "Cylinder.h"
#include "IRotationSink.h"
#include "Polygon.h"
#include "RotationSource.h"


/**
 * Objects of this class represent a pulley component.
 *
 * The pulley is the primary means of changing the speed of shafts and moving
 * rotation to different shafts. Some sources cause a pulley to rotate making it
 * both a source and sink.
 */
class Pulley : public Component, public IRotationSink
{
private:
    /// Rotation of the pulley
    double mRotation = 0;
    /// Rotation source associated with pulley
    RotationSource mRotationSource;
    /// Left side of pulley hub image
    cse335::Cylinder mPulleyHub1;
    /// Right side of pulley hub image
    cse335::Cylinder mPulleyHub2;
    /// Pulley belt image.
    cse335::Polygon mBelt;
    /// Length of pulley
    int mPulleyDiameter;

    /// Center point at which to draw pulley. Based on attached shaft
    wxPoint mPulleyCenterPt;

    /// Pulley this pulley is connected to by a belt. Shared ptr okay????
    std::shared_ptr<Pulley> mBeltConnectedPulley = nullptr;

public:
    /**
     * Pulley constructor.
     * Sets up pulley image.
     * Also creates owned rotation source.
     * @param diameter Length to create pulley image with
     * @param pos Point of connected shaft to draw pulley at
     */
    Pulley(double diameter, wxPoint pos);
    /// delete the copy constructor
    Pulley(const Pulley&) = delete;

    /// delete the copy assignment operator
    Pulley& operator=(const Pulley&) = delete;

    /**
    * Draw the component at the currently specified location in the background.
    * @param graphics Graphics object to render to
    */
    void DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics) override;
    /**
    * Draw the component at the currently specified location in the foreground.
    * @param graphics Graphics object to render to
    */
    void DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics) override;
    /**
     * Reset this component
     */
    void Reset() override;

    /// Get a pointer to the source object
    /// @return Pointer to RotationSource object
    RotationSource* GetSource() { return &mRotationSource; }

    /**
    * Updates the rotation of the pulley based on its source.
    * Also sends rotation to pulley owned associated source.
    * Override from IRotationSink
    * @param rotation Amount of rotation to be applied
    */
    void UpdateRotation(double rotation) override;

    /**
     * Connect a pulley to another pulley
     * and setup the belt image.
     * @param pulley Pulley to connect to
     */
    void BeltTo(std::shared_ptr<Pulley> pulley);

    /**
     * Get the length of the pulley
     * Used for setting up belt image
     * @return Length of pulley
     */
    int GetDiameter() { return mPulleyDiameter; }
};


#endif //PULLEY_H
