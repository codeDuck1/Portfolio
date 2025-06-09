/**
 * @file MachineSystem.h
 * @author Jaylon Sifuentes
 *
 * Class that represents a machine system.
 */

#ifndef MACHINESYSTEM_H
#define MACHINESYSTEM_H
#include "IMachineSystem.h"


class Machine;

/**
 * Objects of this class represent a machine system, which are derived from the IMachineSystem interface.
 */
class MachineSystem : public IMachineSystem
{
private:
    /// Location of machine
    wxPoint mLocation;
    /// Machine associated with this system
    std::shared_ptr<Machine> mMachine;

    /// Number of currently selected machine
    int mMachineNumber;

    /// Resources directory. Must know since invoking factories
    std::wstring mResourcesDirectory;

    /// Framerate
    double mFrameRate = 0;
    /// Time calculated by frame / framerate
    double mTime = 0;
    /// Current frame
    double mCurrentFrame = 0;
    /// Flag
    int mFlag = 0;

public:
    /**
     * Constructor for a machine system
     * @param resourcseDirect resources directory
     */
    MachineSystem(std::wstring resourcseDirect);
    /**
   * Set the position for the root of the machine
   * @param location The x,y location to place the machine
   */
    void SetLocation(wxPoint location) override;

    /**
    * Get the location of the machine
    * @return Location x,y in pixels as a point
    */
    wxPoint GetLocation() override;

    /**
    * Draw the machine at the currently specified location
    * @param graphics Graphics object to render to
    */
    void DrawMachine(std::shared_ptr<wxGraphicsContext> graphics) override;

    /**
     * Resets self and all components a part of associated machine to time 0
     */
    void Reset();

    /**
    * Set the current machine animation frame
    * @param frame Frame number
    */
    void SetMachineFrame(int frame) override;

    /**
    * Set the expected frame rate in frames per second
    * @param rate Frame rate in frames per second
    */
    void SetFrameRate(double rate) override;


    /**
    * Set the machine number
    * @param machine An integer number. Each number makes a different machine
    */
    void ChooseMachine(int machine) override;

    /**
     * Get the current machine number
     * @return Machine number integer
     */
    int GetMachineNumber() override;

    /**
     * Get the current machine time.
     * @return Machine time in seconds
     */
    double GetMachineTime() override;

    /**
     * Set the flag from the control panel
     * @param flag Flag to set
     */
    void SetFlag(int flag) override;
};


#endif //MACHINESYSTEM_H
