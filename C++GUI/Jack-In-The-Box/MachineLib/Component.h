/**
 * @file Component.h
 * @author Jaylon Sifuentes
 *
 * Class that represents components.
 */

#ifndef COMPONENT_H
#define COMPONENT_H
// WHY SHAFT NEED THESE HEADERS BUT OTHERS DID NOT
#include <memory>
#include <wx/graphics.h>


/**
 * Objects of this class represent a component.
 */
class Component
{
private:
    /// Location of component
    wxPoint mLocation = wxPoint(0, 0);
    /// Time
    double mTime = 0;

public:
    /**
   * Draw the component at the currently specified location in the background.
   * @param graphics Graphics object to render to
   */
    virtual void DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics) = 0;

    /**
   * Draw the component at the currently specified location in the foreground.
   * @param graphics Graphics object to render to
   */
    virtual void DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics) = 0;


    /**
     * Reset this components attributes
     */
    virtual void Reset() = 0;

    /**
     * Get X location of component
     * @return x location
     */
    double GetX() { return mLocation.x; }
    /**
     * Get  Y location of component
     * @return y location
     */
    double GetY() { return mLocation.y; }

    /**
     * Location setter for component images
     * @param loc Location to set component to
     */
    void SetLocation(wxPoint loc) { mLocation = loc; }

    /**
     * Set the time
     * @param time time
     */
    void SetTime(double time)
    {
        mTime = time;
    }

    /**
     * Advance the animation time
     * @param increase
     */
    virtual void Advance(double increase)
    {
        mTime += increase;
    }

    /**
     * Get time
     * @return time in seconds
     */
    double GetTime() { return mTime; }
};


#endif //COMPONENT_H
