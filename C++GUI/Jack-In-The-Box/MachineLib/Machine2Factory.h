/**
 * @file Machine2Factory.h
 * @author Jaylom Sifuentes
 *
 * Factory class for creating machine 2!
 */

#ifndef MACHINE2FACTORY_H
#define MACHINE2FACTORY_H
#include <memory>
#include <string>

class Machine;
class Shap;

/**
 * Factory for creating machine 2!
 * Can create a music playing troll machine
 */
class Machine2Factory
{
private:
    /// Path to the images directory
    std::wstring mImagesDir;
    /// Path to the resources directory
    std::wstring mResourcesDir;

public:
    /**
     * Machine 2 factory constructor
     * @param resourcesDir Path to the resources directory
     */
    Machine2Factory(std::wstring resourcesDir);
    /**
     *
     * @return Pointer to the
     */
    std::shared_ptr<Machine> Create();
};


#endif //MACHINE2FACTORY_H
