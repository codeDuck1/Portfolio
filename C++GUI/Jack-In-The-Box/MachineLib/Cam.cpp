/**
 * @file Cam.cpp
 * @author Jaylon Sifuentes
 */
#include "pch.h"
#include "Cam.h"
#include "IKeyResponder.h"

/// Width of the cam on the screen in pixels
const double CamWidth = 17;
/// Cam diameter
const double CamDiameter = 55;
/// How big is the hole in the cam?
const double HoleSize = 8;
/// The key image
const std::wstring KeyImage = L"/key.png";
/// The key image size
const int KeyImageSize = 20;
/// The amount the key drops into the hole
const int KeyDrop = 10;
/// Affects how large the hole becomes when scaling
const double ScaleMult = 4.5;
/// X offset to center hole placement on cam
const int HoleXOffset = 4;
/// Y offset for center hole placement on cam
const int HoleYOffset = 5;
/// Hole rotation speed div
const int HoleRotDiv = 3;

Cam::Cam(std::wstring resourcesDir, wxPoint location)
{
    // Setup the cam image
    SetLocation(location);
    mCamCylinder.SetSize(CamDiameter, CamWidth);
    mCamCylinder.SetColour(*wxWHITE);
    // Setup the key image
    mKey.SetImage(resourcesDir + KeyImage);
    mKey.Rectangle(-KeyImageSize/2, 0, KeyImageSize, KeyImageSize);
}

void Cam::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
}

void Cam::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw the cam rectangle
    mCamCylinder.Draw(graphics, GetX(), GetY(), 0);
    graphics->SetBrush(*wxBLACK_BRUSH);
    // Calculate current position
    double startX = GetX() + (CamWidth / HoleXOffset);
    double startY = GetY() + (CamDiameter / 2) - HoleYOffset;
    double endY = GetY() - (CamDiameter / 2) - 2;
    /* Adjust rotation for the hole
     * so it appears to move at same speed as the
     * cam's connected shaft.
     */
    double holeRotation = mRotation / HoleRotDiv;
    // Current y position
    double currentY = startY + (endY - startY) * holeRotation;
    // Resize Y using sine wave
    double verticalScaler = 1.0 + sin(holeRotation * M_PI) * 1;
    // Calculate new scaled height and y position
    double scaledHeight = ScaleMult * verticalScaler;
    double yOffset = (ScaleMult * (verticalScaler - 1.0)) / 2.0;
    graphics->SetBrush(*wxBLACK_BRUSH);
    /*
     * Move the hole along the rectangle until it has reached the end point.
     * Once at the end, trigger key drop event.
     */
    if(currentY > endY)
    {
        graphics->DrawEllipse(startX, currentY - yOffset, HoleSize, scaledHeight);
        mKey.DrawPolygon(graphics, GetX() + HoleXOffset * 2, GetY() - (CamDiameter / 2));
    } else
    {
        mKey.DrawPolygon(graphics, GetX() + HoleXOffset * 2, GetY() - KeyImageSize);
        mCamCylinder.Draw(graphics, GetX(), GetY(), 0);
        HoleUnderKey();
    }
}

void Cam::Reset()
{
    mRotation = 0;
}

void Cam::UpdateRotation(double rotation)
{
    mRotation = rotation;
}

void Cam::HoleUnderKey()
{
    for(auto responder : mKeyResponders)
    {
        responder->OnKeyDrop();
    }
}
