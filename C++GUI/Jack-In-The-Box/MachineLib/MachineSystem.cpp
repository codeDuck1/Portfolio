/**
 * @file MachineSystem.cpp
 * @author Jaylon Sifuentes
 */

#include "pch.h"
#include "MachineSystem.h"
#include "Machine.h"
#include "Machine2Factory.h"
#include "MachineCFactory.h"

MachineSystem::MachineSystem(std::wstring directory) : mResourcesDirectory(directory)
{
    ChooseMachine(1);
}

void MachineSystem::SetLocation(wxPoint location)
{
    mLocation = location;
}

wxPoint MachineSystem::GetLocation()
{
    return mLocation;
}


void MachineSystem::DrawMachine(std::shared_ptr<wxGraphicsContext> graphics)
{
    // This will put the machine where it is supposed to be drawn
    graphics->PushState();
    graphics->Translate(mLocation.x, mLocation.y);
    mMachine->Draw(graphics);
    graphics->PopState();
}

/**
 * Reset time
 */
void MachineSystem::Reset()
{
    mCurrentFrame = 0;
    mFrameRate = 0;
    mTime = 0;
    mMachine->Reset();
}


void MachineSystem::SetMachineFrame(int frame)
{

    while (mCurrentFrame < frame)
    {
        mCurrentFrame++;
        mTime = mCurrentFrame / mFrameRate;

        mMachine->Advance(1.0 / mFrameRate);
        mMachine->SetTime(mTime);
    }

    while (mCurrentFrame > frame)
    {
        mCurrentFrame--;
        mTime = mCurrentFrame / mFrameRate;
        mMachine->Advance(-1.0 / mFrameRate);
        mMachine->SetTime(mTime);
    }
}

void MachineSystem::SetFrameRate(double rate)
{
    mFrameRate = rate;
}


void MachineSystem::ChooseMachine(int machine)
{
    if(mMachine != nullptr)
    {
        Reset();
    }

    if (machine == 1)
    {

        MachineCFactory tempFactory(mResourcesDirectory);
        mMachine = tempFactory.Create();
        mMachine->SetMachineSystem(this);
        mMachineNumber = machine;
    }
    if(machine == 2)
    {

        Machine2Factory tempFactory2(mResourcesDirectory);
        mMachine = tempFactory2.Create();
        mMachine->SetMachineSystem(this);
        mMachineNumber = machine;
    }

}

int MachineSystem::GetMachineNumber()
{
    return mMachineNumber;
}

double MachineSystem::GetMachineTime()
{
    return mTime;
}

void MachineSystem::SetFlag(int flag)
{
    mFlag = flag;
}


