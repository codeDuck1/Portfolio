/**
 * @file Box.h
 * @author Jaylon Sifuentes
 *
 * Class that represents a box.
 */

#ifndef BOX_H
#define BOX_H
#include "Component.h"
#include "Cylinder.h"
#include "IKeyResponder.h"
#include "Polygon.h"

/**
 * Objects of this class represent the box component.
 */
class Box : public Component, public IKeyResponder
{
private:
    /// The box background image
    cse335::Polygon mBox;
    /// The box foreground image
    cse335::Polygon mBoxFace;
    /// The lid image
    cse335::Polygon mLid;
    /// The lid position
    wxPoint mLidStartPosition;
    /// The lid angle
    double mLidAngle = 0;
    /// If the lid is open
    bool mOpen = false;

public:
    /**
     * Constructor for Box
     * @param imagesDir image path for box
     * @param boxSize box size
     * @param lidSize lid size
     */
    Box(std::wstring imagesDir, int boxSize, int lidSize);
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
    void Advance(double increase) override;
    /**
     * Reset box attributes
     */
    void Reset() override;
   /**
   * When called by Cam, this function triggers the
   * box to open.
   */
    void OnKeyDrop() override;
};


#endif //BOX_H
