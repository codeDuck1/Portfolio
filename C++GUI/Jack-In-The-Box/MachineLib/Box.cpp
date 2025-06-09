/**
 * @file Box.cpp
 * @author Jaylon Sifuentes
 */

#include "pch.h"
#include "Box.h"

/// Image path for box background
std::wstring const BoxBackgroundImage = L"/box-background.png";

/// Image path for box foreground
std::wstring const BoxForegroundImage = L"/box-foreground.png";

/// Image path for box lid
std::wstring const BoxLidImage = L"/box-lid.png";

/// The horizontal scale for the lid when the angle is zero
const double LidZeroAngleScale = 0.02;

/// How long it takes to up the lid in seconds
const double LidOpeningTime = 0.25;

/// Lid offset for open position
const int LidOpenOffset = -300;

/// Lid offset for close position
const int LidCloseOffset = -12500;

Box::Box(std::wstring imagesDir, int boxSize, int lidSize)
{
    mBox.Rectangle(-boxSize / 2, 0, boxSize, boxSize);
    mBox.SetImage(imagesDir + BoxBackgroundImage);
    mLidAngle = 0;
    mLidStartPosition = wxPoint(-lidSize / 2, -boxSize);
    mLid.Rectangle(mLidStartPosition.x, LidCloseOffset, lidSize, lidSize); // -12500 for close
    mLid.SetImage(imagesDir + BoxLidImage);
    mBoxFace.Rectangle(-boxSize / 2, 0, boxSize, boxSize);
    mBoxFace.SetImage(imagesDir + BoxForegroundImage);
}

void Box::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{

    mBox.DrawPolygon(graphics, GetX(), GetY());
    // Amimate box open and close by resizing.
    graphics->PushState();
    double sine = sin(mLidAngle);
    double lidScale = LidZeroAngleScale + (1.0 - LidZeroAngleScale) * sine;

    wxPoint closeTrans = wxPoint(0, 0);
    wxPoint openTrans = wxPoint(0, (-LidCloseOffset) + mLidStartPosition.y);
    wxPoint currPt;
    if(mLidAngle > 0)
    {
        currPt = openTrans;
    } else
    {
        currPt = closeTrans;
    }
    graphics->Translate(currPt.x, currPt.y);
    graphics->Scale(1, lidScale);
    mLid.DrawPolygon(graphics, 0, 0);
    graphics->PopState();
}

void Box::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
    mBoxFace.DrawPolygon(graphics, GetX(), GetY());
}

/**
 * Advance the animation in time
 * @param increase Amount of time to advance in seconds
 */
void Box::Advance(double increase)
{
    Component::Advance(increase);	// Upcall
    double openAngle = M_PI / 2;
    if(mOpen && mLidAngle < openAngle)
    {
        mLidAngle += openAngle * increase / LidOpeningTime;

        if(mLidAngle > openAngle)
        {
            mLidAngle = openAngle;
        }
    }
}

void Box::Reset()
{
    mLidAngle = 0;
}

void Box::OnKeyDrop()
{
    mOpen = true;
}


