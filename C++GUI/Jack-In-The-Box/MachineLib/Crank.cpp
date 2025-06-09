/**
 * @file Crank.cpp
 * @author Jaylon Sifuentes
 *
 * Crank implementation.
 */

#include "pch.h"
#include "Crank.h"
#include "RotationSource.h"

/// The width of the crank on the screen in pixels
const int CrankWidth = 10;

/// The depth of the crank away from the screen in pixels
const int CrankDepth = 20;

/// The length of the crank in pixels
const int CrankLength = 50;

/// The diameter to draw the crank handle
const int HandleDiameter = 7;

/// How long the handle is in pixels
const int HandleLength = 40;

/// How much to the left of the crank X the handle starts in pixels
const int HandleStartX = -10;

/// Crank color
const wxColour CrankColor = wxColour(220, 220, 220);

/// Line color for the rod
const wxColour CrankHandleLineColor = wxColour(100, 100, 100);

/// X offset for handle placement
const int HandleXOffset = 150;

/// Y offset for handle placement
const int HandleYOffset = -190;

/// Y offset for crank placement
const int CrankYOffset = -170;

/// X offset for crank placement
const int CrankXOffset = 5;

/// Number of lines on crank
const int NumLines = 4;

/**
 * Crank constructor, sets up the handle and crank images
 * and creates its owned rotation source.
 */
Crank::Crank(wxPoint location) : mRotationSource()
{
    SetLocation(location);
    // Create the handle part of the crank
    mHandle.SetSize(HandleDiameter, HandleLength);
    mHandle.SetColour(CrankColor);
    mHandle.SetLines(CrankHandleLineColor, 1, NumLines);

    //Create the rectangle part of crank and set its start location
    mCrank.Rectangle(GetX() + HandleXOffset, 0, CrankWidth, CrankLength / 2);
    mCrank.SetColor(CrankColor);

    mSpeed = 5;
}

void Crank::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
}

void Crank::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
    double handleY = GetY() + cos(mRotation) * CrankLength;
    mHandle.Draw(graphics, HandleXOffset, HandleYOffset + handleY, mRotation);

    // Calculate scaler based on crank position
    double distanceFromHandle = -(handleY);
    double scaler = 1.0 + ((distanceFromHandle) / (CrankLength / 2));
    // Draw the rectangle that always remains
    mCrank.DrawPolygon(graphics, CrankXOffset, CrankYOffset);
    // Apply small offset to scaler so rect goes over the handle
    double rectangleOffset = 0;
    if(scaler < 0)
    {
        rectangleOffset = -0.6;
    } else
    {
        rectangleOffset = 0.3;
    }
    // Draw the connecting rectangle that "follows" the handle.
    // Does this by resizing.
    graphics->PushState();
    graphics->Translate(0, CrankYOffset);
    graphics->Scale(1,  scaler + rectangleOffset);
     mCrank.DrawPolygon(graphics, CrankXOffset, 0);
    graphics->PopState();
}

void Crank::Reset()
{
    mRotation = 0;
}


/**
 * Speed multiplier to be applied to first shaft receiving rotation from crank.
 * This is used to slow down the shaft relative to the crank.
 */
const float speedMult = 0.2f;
void Crank::Advance(double increase)
{
    Component::Advance(increase);
    mRotation += increase * mSpeed;
    mRotationSource.SetRotation(mRotation * speedMult); // Set the rotation of the rotation source
}
