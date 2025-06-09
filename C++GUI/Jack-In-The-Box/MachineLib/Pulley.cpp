/**
 * @file Pulley.cpp
 * @author Jaylon Sifuentes
 */

#include "pch.h"
#include "Pulley.h"

/// How wide the hub is on each side of the pulley
const double PulleyHubWidth = 3;
/// How deep the belt is into the pulley
const double PulleyBeltDepth = 3;
/// The color to use for pulleys
const wxColour PulleyColor = wxColour(205, 250, 5);
/// The line color to use for the hub
/// First parameter to Cylinder::SetLines
const wxColour PulleyHubLineColor = wxColour(139, 168, 7);
/// The width to draw the lines on the hub
/// Second parameter to Cylinder::SetLines
const int PulleyHubLineWidth = 4;
/// The number of lines to draw on a pulley is the int(diameter / 6.0)
/// Third parameter to Cylinder::SetLines
const double PulleyHubLineCountDiviser = 6.0;
/// The distance on the X axis from the first hub to second
const int PulleyHubDistance = 12;
/// Width of belt
const int BeltWidth = 10;
/// X offset for belt
const int BeltXOffset = 4;

Pulley::Pulley(double diameter, wxPoint pos) : Component(), mRotationSource()
{
    // Set pulleys draw position
    //mPulleyCenterPt = pos;

    SetLocation(pos);

    // Set pulley length
    mPulleyDiameter = diameter;
    // Set the size of each component
    mPulleyHub1.SetSize(diameter, PulleyHubWidth);
    mPulleyHub2.SetSize(diameter, PulleyHubWidth);

    // Set the colors
    mPulleyHub1.SetColour(PulleyColor);
    mPulleyHub2.SetColour(PulleyColor);

    // Setup the the hub lines
    mPulleyHub1.SetLines(PulleyHubLineColor, PulleyHubLineWidth, (diameter / PulleyHubLineCountDiviser));
    mPulleyHub2.SetLines(PulleyHubLineColor, PulleyHubLineWidth, (diameter / PulleyHubLineCountDiviser));
}

void Pulley::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
}

void Pulley::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
    mPulleyHub1.Draw(graphics, GetX(), GetY(), mRotation);
    mPulleyHub2.Draw(graphics, GetX() + PulleyHubDistance, GetY(), mRotation);


    if (mBeltConnectedPulley != nullptr)
    {
        /*
         * If this pulley's Y is greater than the connected pulley, then its below it.
         * Otherwise its above. Determine Y location accordingly.
         */
        if (GetY() > mBeltConnectedPulley->GetY())
        {
            mBelt.DrawPolygon(graphics, GetX() + (PulleyHubDistance / BeltXOffset), GetY() + (mPulleyDiameter / 2));
        }
        else
        {
            mBelt.DrawPolygon(graphics, GetX() + (PulleyHubDistance / BeltXOffset),
                              mBeltConnectedPulley->GetY() + mBeltConnectedPulley->GetDiameter() / 2);
        }
    }
}

void Pulley::Reset()
{
    mRotation = 0;
}

void Pulley::UpdateRotation(double rotation)
{

    mRotation = rotation;
    mRotationSource.SetRotation(mRotation); // Set the rotation of the rotation source

    // If there is a pulley connected to this one, update its rotation.
    if (mBeltConnectedPulley != nullptr)
    {
        /*
         *If a pulley with radius N drives a pulley with radius M using the belt,
         *the speed of the second pulley is N/M times the speed of the first pulley.
         */
        double driverPullyRadius = mPulleyDiameter / 2;
        double connectedPulleyRadius = mBeltConnectedPulley->GetDiameter() / 2;
        double speedCal = driverPullyRadius / connectedPulleyRadius;
        mBeltConnectedPulley->UpdateRotation(mRotation * speedCal);
    }
}



void Pulley::BeltTo(std::shared_ptr<Pulley> pulley)
{
    mBeltConnectedPulley = pulley;
    /*
     * Determine belt height based on the difference between pulley center points
     * and overhang past these points.
     */
    int height = std::abs(GetY() - mBeltConnectedPulley->GetY()) + (mPulleyDiameter / 2) + (mBeltConnectedPulley->GetDiameter() / 2) - 1;

    mBelt.Rectangle(0, 0, BeltWidth, height);
    mBelt.SetColor(*wxBLACK);
}
