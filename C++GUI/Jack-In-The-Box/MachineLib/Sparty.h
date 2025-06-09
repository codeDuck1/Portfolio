/**
 * @file Sparty.h
 * @author Jaylon Sifuentes
 *
 * Class that represents Sparty
 */

#ifndef SPARTY_H
#define SPARTY_H
#include "Component.h"
#include "IKeyResponder.h"
#include "Polygon.h"

/**
 * Objects of this class represent Sparty and the spring attached to him.
 *
 */
class Sparty : public Component, public IKeyResponder
{
private:
    /// The sparty image
    cse335::Polygon mSparty;
    /// If Sparty should be springing out
    bool mIsSprung = false;
    /// Spring increaser
    double mSpringIncrease = 0;
    /// If the toy attached to the spring is bouncy (FOR CHALLENGE TASK)
    bool mBouncyToy = false;
    /// Sparty Bounce increaser (FOR CHALLENGE TASK)
    double mBounceTime = 0;
    /// Spring/connected toy X offset
    int mSpringX = 0;
    /// Spring starting height
    int mSpringStartLength = 0;
    /// Number of spring links
    int mSpringLinks = 0;
    /// Spring width
    int mSpringWidth = 0;


public:
    /**
     * Sparty constructor
     * @param imagesDir Image file to load
     * @param size to draw Sparty (width and height)
     * @param springLength How long the spring is when fully extended in pixels
     * @param springWidth How wide the spring is in pixels
     * @param numLinks How many links (loops) there are in the spring
     * @param bouncy if the toy should bounce around when out of box
     * @param springX x location for spring placement
     */
    Sparty(std::wstring imagesDir, int size, int springLength, int springWidth, int numLinks, bool bouncy, int springX);
    /**
     * Reset Spartys attributes
     */
    void Reset() override;

    void Advance(double increase) override;
    /**
   * Draw the component at the currently specified location in the background.
   * @param graphics Graphics object to render to
   */
    void DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics) override;
    /**
    * Draw the component at the currently specified location in the background.
    * @param graphics Graphics object to render to
    */
    void DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics) override;

    void DrawSpring(std::shared_ptr<wxGraphicsContext> graphics, int x, int y, double length, double width,
                    int numLinks);
    /**
     * When called by Cam, this function triggers Sparty
     * to spring out of the box.
     */
    void OnKeyDrop() override;
};


#endif //SPARTY_H
