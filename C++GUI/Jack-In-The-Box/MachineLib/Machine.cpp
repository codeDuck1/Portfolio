/**
 * @file Machine.cpp
 * @author Jaylon Sifuentes
 */
#include "pch.h"
#include "Machine.h"
#include "Component.h"

void Machine::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    for (auto component : mComponents)
    {
        component->DrawComponentBackground(graphics);

    }

    for(auto component : mComponents)
    {
        component->DrawComponentForeground(graphics);
    }
}
