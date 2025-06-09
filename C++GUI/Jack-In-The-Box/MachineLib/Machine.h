/**
 * @file Machine.h
 * @author Jaylon Sifuentse
 *
 * Class that represents a instantiated machine.
 */

#ifndef MACHINE_H
#define MACHINE_H
#include "Component.h"


class MachineSystem;
class Component;

/**
 * Objects of this class represent a machine.
 */
class Machine
{
private:
    /// The machine system that is
    /// controlling this machine
    MachineSystem* mMachineSystem = nullptr;

    /// The components apart of this machine
    std::vector<std::shared_ptr<Component>> mComponents;

    /// Time
    double mTime = 0;

public:
    /**
    * Draw the machine at the currently specified location
    * @param graphics Graphics object to render to
    */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics);

    /**
     * Add components to machine
     * @param component component to add
     */
    void AddComponent(std::shared_ptr<Component> component)
    {
        mComponents.push_back(component);
    }

    /**
     * Set the system associated with this machine
     * @param system system to associate this machine with
     */
    void SetMachineSystem(MachineSystem* system)
    {
        mMachineSystem = system;
    }

    /**
     * Set time of self and all associated components
     * @param time time
     */
    void SetTime(double time)
    {
        mTime = time;

        for(auto component : mComponents)
        {
            component->SetTime(time);
        }
    }

    /**
     * Advance time of self and all associated components
     * @param increase
     */
    void Advance(double increase)
    {
        mTime += increase;
        for(auto component : mComponents)
        {
            component->Advance(increase);
        }
    }

    /**
     * Reset this machines attributes and its components
     *
     */
    void Reset()
    {
        mTime = 0;
        for(auto component : mComponents)
        {
            component->Reset();
        }
    }

    /**
     * Get the time that has passed
     * @return time that has passed
     */
    double GetTime(){return mTime;}


};


#endif //MACHINE_H
