/**
 * @file Shaft.h
 * @author Jaylon Sifuentes
 *
* Class that represents a shaft component.
 */

#ifndef SHAFT_H
#define SHAFT_H

#include "Component.h"
#include "IRotationSink.h"
#include "Cylinder.h"
#include "RotationSource.h"

/// Default diameter of the shaft cylinder
const int ShaftDiameter = 10;
/// Default length of the shaft cylinder in pixels
const int ShaftLength = 80;
/// Default point for shaft placement
const wxPoint DefaultPoint = wxPoint(90, -185);

/**
 * Class that represents a shaft component.
 * The shaft is a rotation source and sink from the crank.
 */
class Shaft : public Component, public IRotationSink
{
private:
    /// The cylinder image that displays the shaft
    cse335::Cylinder mCylinder;

    /// Rotation source associated with shaft
    RotationSource mRotationSource;

    /// Rotation of the shaft
    double mRotation = 0;

    /// Left center point of shaft. To be used by pulley for placement.
    wxPoint mLeftCenter;
    /// Right center point of shaft. To be used by pulley for placement.
    wxPoint mRightCenter;

public:
    /**
     * Shaft constructor.
     * Sets up the cylinder image for the shaft
     * Also creates owned rotation source
     * @param diameter diameter of shaft image
     * @param length length of shaft image
     * @param location location of the shaft image
     */
    Shaft(int diameter = ShaftDiameter, int length = ShaftLength, wxPoint location = DefaultPoint);

    /// delete the copy constructor
    Shaft(const Shaft&) = delete;

    /// delete the copy assignment operator
    Shaft& operator=(const Shaft&) = delete;

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
     * Updates the rotation of the shaft based on its source.
     * Also sends rotation to shafts owned associated source.
     * Override from IRotationSink
     * @param rotation Amount of rotation to be applied
     */
    void UpdateRotation(double rotation) override;

    /**
     * Reset this component
     */
    void Reset() override;

    /// Get a pointer to the source object
    /// @return Pointer to RotationSource object
    RotationSource* GetSource() { return &mRotationSource; }

    /**
     * Get left center point of shaft image for pulley placement
     * @return Left center point of shaft image
     */
    wxPoint GetLeftCenter() { return mLeftCenter; }
    /**
     * Get right center point of shaft image for pulley placement
     * @return Right center point of shaft image
     */
    wxPoint GetRightCenter() { return mRightCenter; }
};


#endif //SHAFT_H
