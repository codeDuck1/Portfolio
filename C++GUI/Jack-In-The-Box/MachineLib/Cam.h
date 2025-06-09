/**
 * @file Cam.h
 * @author Jaylon Sifuentes
 *
 * Class that represents a cam component.
 */

#ifndef CAM_H
#define CAM_H
#include "Component.h"
#include "Cylinder.h"
#include "IRotationSink.h"
#include "Polygon.h"


class IKeyResponder;
/**
 * Objects of this class represent a cam component.
 * The cam is a rotation sink that triggers the box and Sparty
 * when its key drops into the hole.
 */
class Cam : public Component, public IRotationSink
{
private:
    /// Rectangle image for the cam
    cse335::Cylinder mCamCylinder;
    /// Components that respond to the cam key drop
    std::vector<std::shared_ptr<IKeyResponder>> mKeyResponders;
    /// Rotation of the cam
    double mRotation = 0;

    /// Key image
    cse335::Polygon mKey;

public:
    /**
     * Cam default constructor
     * Sets up cam image
     * @param resourcesDir image directory
     * @param location location of the cam image
     */
    Cam(std::wstring resourcesDir, wxPoint location);
    /// delete the copy constructor
    Cam(const Cam&) = delete;

    /// delete the copy assignment operator
    Cam& operator=(const Cam&) = delete;
    /**
    * Draw the cam at the currently specified location in the background.
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
   * Updates the rotation of the cam based on its source.
   * Override from IRotationSink
   * @param rotation Amount of rotation to be applied from source
   */
    void UpdateRotation(double rotation) override;

    /**
     * Add responder to list that responds to key drop event
     * @param keyResponder Key responder to add to responder list
     */
    void AddResponder(std::shared_ptr<IKeyResponder> keyResponder)
    {
        mKeyResponders.push_back(keyResponder);
    }

    /**
     * Called when the hole is underneath the key.
     * Alerts all key responders of the event!
     */
    void HoleUnderKey();
};


#endif //CAM_H
