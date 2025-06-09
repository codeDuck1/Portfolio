/**
 * @file Sparty.cpp
 * @author Jaylon Sifuentes
 */

#include "pch.h"
#include "Sparty.h"

/// The spring pen size to use in pixels
const double SpringWireSize = 2;
/// Minimum spacing between spring wire
const double SpringWireMinSpacing = 1;
/// Color to draw the spring
const wxColour SpringColor = wxColour(220, 220, 220);
/// Sparty popup time in seconds
const double SpartyPopupTime = 0.25;
/// Spring length full sizer
const double MaxSpringLengthMult = 3.5;
/// Spring increase speed
const int SpringSpeed = 140;
/// Link seperator sizer
const int LinkSeperationDiv = 800;
/// Max bounce height
const int BounceHeight = 8;
/// Max bounce width
const int BounceWidth = 3;
/// Bounce speed
const int BounceSpeed = 3;
/// Toy bottom Y pos
const int ToyBottomY = -90;
/// Spring length compress div
const int SpringCompressDiv = 4;

Sparty::Sparty(std::wstring imagesDir, int size, int springLength, int springWidth, int numLinks, bool bouncy, int springX)
{
    mSpringX = springX;
    mBouncyToy = bouncy;
    mSpringStartLength = springLength / SpringCompressDiv;
    mSpringWidth = springWidth;
    mSpringLinks = numLinks;
    mSparty.Rectangle(-size / 2, ToyBottomY, size, size);
    mSparty.SetImage(imagesDir);

}

void Sparty::Reset()
{
    mSpringIncrease = 0;
    mIsSprung = false;
}

void Sparty::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw consistently
    DrawSpring(graphics, mSpringX, 0, mSpringStartLength + mSpringIncrease, mSpringWidth, mSpringLinks - mSpringIncrease / LinkSeperationDiv);
    // If this is not a bouncy toy or a toy that hasnt been sprung up, position accordingly.
    if(!mIsSprung || !mBouncyToy)
    {
        graphics->PushState();
        graphics->Translate( mSpringX, (mSpringStartLength - mSpringIncrease));
        mSparty.DrawPolygon(graphics, 0, 0);
        graphics->PopState();
    }

    // If this is a bouncy toy, make it bounce once sprung is triggered.
    if(mIsSprung && mBouncyToy)
    {
        double yBounceOff = BounceHeight * sin(mBounceTime * BounceSpeed);
        double xBounceOff = BounceWidth * sin(mBounceTime * BounceSpeed / 2);  // Adding a phase shift
        graphics->PushState();
        graphics->Translate(mSpringX + xBounceOff, (mSpringStartLength - mSpringIncrease) + yBounceOff);  // Apply yOffset based on mSpringIncrease
        mSparty.DrawPolygon(graphics, 0, 0);
        graphics->PopState();
    }
}

void Sparty::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
}

/**
 * Draw a spring.
 * @param graphics Graphics context to render to
 * @param x X location of the bottom center of the spring in pixels
 * @param y Y location of the bottom center of the spring in pixels
 * @param length Length to draw the spring (bottom to top) in pixels
 * @param width Spring width in pixels
 * @param numLinks Number of links (loops) in the spring
 */
void Sparty::DrawSpring(std::shared_ptr<wxGraphicsContext> graphics,
                        int x, int y, double length, double width, int numLinks)
{
    auto path = graphics->CreatePath();
    // We keep track of three locations, the bottom of which
    // is y1. First half-loop will be y1 to y3, second half-loop
    // will be y3 to y2.
    double y1 = y;
    double linkLength = length / numLinks;
    // Left and right X values
    double xR = x + width / 2;
    double xL = x - width / 2;
    path.MoveToPoint(x, y1);
    for(int i=0; i<numLinks; i++)
    {
        auto y2 = y1 - linkLength;
        auto y3 = y2 - linkLength / 2;
        path.AddCurveToPoint(xR, y1, xR, y3, x, y3);
        path.AddCurveToPoint(xL, y3, xL, y2, x, y2);
        y1 = y2;
    }
    graphics->StrokePath(path);
}

void Sparty::OnKeyDrop()
{
    mIsSprung = true;
}

/**
 * Advance the animation in time
 * @param increase Amount of time to advance in seconds
 */
void Sparty::Advance(double increase)
{
    // Begin animation once Sparty receives call that key has dropped.
    if(mIsSprung)
    {
        Component::Advance(increase);	// Upcall
        mSpringIncrease += SpringSpeed * increase / SpartyPopupTime;

        // Stop Sparty from going beyond his max spring length
        if(mSpringIncrease > mSpringStartLength * MaxSpringLengthMult)
        {
            mSpringIncrease = mSpringStartLength * MaxSpringLengthMult;
        }

        /*
         * Only adjust the spring size while the increased size is larger than the starting size.
         * This is to prevent rewinding below the starting length.
         * Otherwise draw Sparty and Spring in starting position.
         */
        if(mSpringIncrease + mSpringStartLength < mSpringStartLength)
        {
            mSpringIncrease = 0;
        }
    }
    mBounceTime += increase;
}






