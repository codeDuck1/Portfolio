/**
 * @file Shaft.cpp
 * @author Jaylon Sifuentes
 *
 */
#include "pch.h"
#include "Shaft.h"

/// The color to draw the shaft
const wxColour ShaftColor = wxColour(220, 220, 220);

/// The color to draw the lines on the shaft
/// First parameter to Cylinder::SetLines
const wxColour ShaftLineColor = wxColour(100, 100, 100);

/// The width to draw the lines on the shaft
/// Second parameter to Cylinder::SetLines
const int ShaftLinesWidth = 1;

/// The number of lines to draw on the shaft
/// Third parameter to Cylinder::SetLines
const int ShaftNumLines = 4;

/// Y offset for shaft placement
const int ShaftYOffset = -185;

/// X offset for shaft placement
const int ShaftXOffset = 90;

/// Left center offset used for pulley placement
const wxPoint ShaftLCOff = wxPoint(5, 2.5f);
/// Right center offset used for pulley placement
const wxPoint ShaftRCOff = wxPoint(20, 2.5f);

Shaft::Shaft(int diameter, int length, wxPoint location) :  mRotationSource()
{
    SetLocation(location);
    mCylinder.SetSize(diameter, length);
    mCylinder.SetColour(ShaftColor);
    mCylinder.SetLines(ShaftLineColor, ShaftLinesWidth, ShaftNumLines);
    mLeftCenter = wxPoint( GetX() + ShaftLCOff.x, GetY() - ShaftLCOff.y );
    mRightCenter = wxPoint( GetX() + (length - ShaftRCOff.x), GetY() - ShaftRCOff.y);
}

void Shaft::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
    mCylinder.Draw(graphics, GetX(), GetY(), mRotation);
}

void Shaft::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
}

void Shaft::UpdateRotation(double rotation)
{
    mRotation = rotation;
    mRotationSource.SetRotation(mRotation); // Set the rotation of the rotation source
}

void Shaft::Reset()
{
    mRotation = 0;
}
