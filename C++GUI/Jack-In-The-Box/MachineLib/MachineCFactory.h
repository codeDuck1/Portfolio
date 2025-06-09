/**
 * @file MachineCFactory.h
 *
 * @author Anik Momtaz
 * @author Jaylon Sifuentes
 *
 * Factory for creating Machine #1
 */

#ifndef CANADIANEXPERIENCE_MACHINECFACTORY_H
#define CANADIANEXPERIENCE_MACHINECFACTORY_H

#include <memory>
#include <string>

class Machine;
class Shape;

/**
 * Factory for creating Machine #1
 * this creates the machine that incorporates challenege tasks.
 */
class MachineCFactory {
private:
    /// Path to the images directory
    std::wstring mImagesDir;

public:
    MachineCFactory(std::wstring resourcesDir);
    /**
     * Factory method to create machine #2
     * @return Pointer to created machine
     */
    std::shared_ptr<Machine> Create();
};

#endif //CANADIANEXPERIENCE_MACHINECFACTORY_H
