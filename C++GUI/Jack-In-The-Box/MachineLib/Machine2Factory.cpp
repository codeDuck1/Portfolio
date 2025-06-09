/**
 * @file Machine2Factory.cpp
 * @author Jaylon Sifuentes
 */
#include "pch.h"
#include "Machine2Factory.h"
#include "Machine.h"
#include "Box.h"
#include "MusicBox.h"
#include "Pulley.h"
#include "Crank.h"
#include "Sparty.h"
#include "Shaft.h"
#include "Cam.h"

/// The images directory in resources
const std::wstring ImagesDirectory = L"/images";


Machine2Factory::Machine2Factory(std::wstring resourcesDir)
{
    mResourcesDir = resourcesDir;
    mImagesDir = resourcesDir + ImagesDirectory;
}


std::shared_ptr<Machine> Machine2Factory::Create()
{
    // The machine itself
    auto machine = std::make_shared<Machine>();

    /*
     * The Box class constructor parameters are:
     * @param imagesDir Directory containing the images
     * @param boxSize Size of the box in pixels (just the box, not the lid)
     * @param lidSize Size of the lid in pixels
     */
    auto box = std::make_shared<Box>(mImagesDir, 250, 240);

    // This simulates the cam key drop so the box will immediately open
    //box->KeyDrop();
    machine->AddComponent(box);

    /*
     * The Sparty class constructor parameters are:
     * @param image Image file to load
     * @param size Size to draw Sparty (width and height)
     * @param springLength How long the spring is when fully extended in pixels
     * @param springWidth How wide the spring is in pixels
     * @param numLinks How many links (loops) there are in the spring
     */
    auto troll =
        std::make_shared<Sparty>(mImagesDir + L"/pinkTroll.png", 212, 260, 55, 15, true, 0);
    auto troll2 =
     std::make_shared<Sparty>(mImagesDir + L"/pinkTroll.png", 212, 260, 55, 15, true, 65);
    auto troll3 =
     std::make_shared<Sparty>(mImagesDir + L"/pinkTroll.png", 212, 260, 55, 15, true, -65);

    machine->AddComponent(troll);
    machine->AddComponent(troll2);
    machine->AddComponent(troll3);

    /*
     * Create the crank!
     */
    auto crank = std::make_shared<Crank>();
    machine->AddComponent(crank);

    /*
     * Create the upper right shaft and add it as a sink to the crank.
     */
    auto shaft = std::make_shared<Shaft>();
    machine->AddComponent(shaft);
    crank->GetSource()->AddSink(shaft);

    /*
     * Create the first pulley, and add it as a sink to the upper right shaft.
     */
    auto pulley1 = std::make_shared<Pulley>(40, shaft->GetLeftCenter());
    machine->AddComponent(pulley1);
    shaft->GetSource()->AddSink(pulley1);

    /*
     * Create the second shaft (lower middle)
     * and its connected pulley rot source. Connect this second pulley
     * to the first.
     */
    auto shaft2 = std::make_shared<Shaft>(8, 230, wxPoint(-115, -65));
    machine->AddComponent(shaft2);
    auto pulley2 = std::make_shared<Pulley>(95, shaft2->GetRightCenter());
    machine->AddComponent(pulley2);
    pulley1->BeltTo(pulley2); // Connect the second pulley to the first.
    pulley2->GetSource()->AddSink(shaft2); // Set the 2nd shaft's source to the 2nd pulley

    /*
     * Set the 3rd pulley (left of lower middle shaft) to be driven by the shaft.
     */
    auto pulley3 = std::make_shared<Pulley>(20, shaft2->GetLeftCenter());
    // set 3rd pullys source to the 2nd shaft
    machine->AddComponent(pulley3);
    shaft2->GetSource()->AddSink(pulley3);

    /*
     * Create the upper left mechanisms.
     */
     auto shaft3 = std::make_shared<Shaft>(ShaftDiameter, 50, wxPoint(-115, -185));
     machine->AddComponent(shaft3);
     auto pulley4 = std::make_shared<Pulley>(80, shaft3->GetLeftCenter());
     pulley4->GetSource()->AddSink(shaft3);
     // Connect the 3rd pulley to the 4th
     pulley3->BeltTo(pulley4);
     machine->AddComponent(pulley4);

     /* Create the cam connected to the upper left shaft.
      * Add Box and trolls to responders to Cam.
      */
     auto cam = std::make_shared<Cam>(mImagesDir,shaft3->GetRightCenter());
     machine->AddComponent(cam);
     pulley3->GetSource()->AddSink(cam);
     cam->AddResponder(box);
     cam->AddResponder(troll);
     cam->AddResponder(troll2);
     cam->AddResponder(troll3);

    auto music = std::make_shared<MusicBox>(mResourcesDir,   L"/songs/pop.xml");
    machine->AddComponent(music);
    shaft->GetSource()->AddSink(music);
    return machine;
}
