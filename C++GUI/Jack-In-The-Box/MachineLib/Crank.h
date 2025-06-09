/**
 * @file Crank.h
 * @author Jaylon Sifuentes
 *
 * Class that represents a crank.
 */

#ifndef CRANK_H
#define CRANK_H
#include "Component.h"
#include "Cylinder.h"
#include "Polygon.h"
#include "RotationSource.h"

/// Default location for crank
const wxPoint ZeroPoint = wxPoint(0, 0);
/**
 * Class that represents a crank component.
 */
class Crank : public Component
{
private:
    /// Handle image of the crank
    cse335::Cylinder mHandle;
    /// Rectangle image connected to handle
    cse335::Polygon mCrank;
    /// Rotation source associated with crank
    RotationSource mRotationSource;

    /// Rotation of the crank in turns
    double mRotation = 0;
    /// Speed the crank rotates at
    double mSpeed;

public:
    /**
     * Constructor for crank
     * Sets up crank image and location
     * @param location x,y location to place crank at. Default 0,0
     */
    Crank(wxPoint location = ZeroPoint);

    /// delete the copy constructor
    Crank(const Crank&) = delete;

    /// delete the copy assignment operator
    Crank& operator=(const Crank&) = delete;

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

    /**
    * Advance this components animation time
    * @param increase Amount of time to advance in seconds
    */
    void Advance(double increase) override;

    /** Get a pointer to the source object
    * @return Pointer to RotationSource object
    */
    RotationSource *GetSource() { return &mRotationSource; }

};


#endif //CRANK_H
