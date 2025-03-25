/*
**
**---------------------------------------------------------------------------
** Copyright 2025 [Your Name]
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
*/

// HEADER FILES ------------------------------------------------------------


#include "sdl_hid.h"
#include "gameconfigfile.h"
#include "m_argv.h"
#include "cmdlib.h"
#include "keydef.h"


// IMPLEMENTATION ----------------------------------------------------------


// Constructor for the SDL GameController 
FSDLGamepadController::FSDLGamepadController(int index)
{
    Index = index;
    gamepad = SDL_GameControllerOpen(index);
    if (gamepad)
    {
        Axes = new AxisInfo[NUM_AXES];
        SetDefaultConfig();
    }
}

// Destructor for the SDL GameController 
FSDLGamepadController::~FSDLGamepadController()
{
    if (gamepad)
    {
        SDL_GameControllerClose(gamepad);
    }
    delete[] Axes;
}

// Process input for the SDL GameController 
void FSDLGamepadController::ProcessInput()
{
    if (!gamepad || !IsConnected())
        return;

    for (int i = 0; i < NUM_AXES; ++i)
    {
        int16_t value = SDL_GameControllerGetAxis(gamepad, static_cast<SDL_GameControllerAxis>(i));
        Axes[i].Value = value / 32768.0f;
    }
}

// Add axes for the SDL GameController 
void FSDLGamepadController::AddAxes(float axes[NUM_JOYAXIS])
{
    for (int i = 0; i < NUM_AXES; ++i)
    {
        axes[Axes[i].GameAxis] += Axes[i].Value * Multiplier * Axes[i].Multiplier;
    }
}

// Get the name of the SDL GameController 
FString FSDLGamepadController::GetName()
{
    return SDL_GameControllerName(gamepad);
}

// Get the sensitivity of the SDL GameController 
float FSDLGamepadController::GetSensitivity()
{
    return Multiplier;
}

// Set the sensitivity of the SDL GameController 
void FSDLGamepadController::SetSensitivity(float scale)
{
    Multiplier = scale;
}

// Get the number of axes for the SDL GameController 
int FSDLGamepadController::GetNumAxes()
{
    return NUM_AXES;
}

// Get the dead zone for an axis of the SDL GameController 
float FSDLGamepadController::GetAxisDeadZone(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return Axes[axis].DeadZone;
    }
    return 0.0f;
}

// Get the game axis map for an axis of the SDL GameController 
EJoyAxis FSDLGamepadController::GetAxisMap(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return Axes[axis].GameAxis;
    }
    return JOYAXIS_None;
}

// Get the name of an axis of the SDL GameController 
const char* FSDLGamepadController::GetAxisName(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(axis));
    }
    return "Invalid";
}

// Get the scale for an axis of the SDL GameController 
float FSDLGamepadController::GetAxisScale(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return Axes[axis].Multiplier;
    }
    return 0.0f;
}

// Set the dead zone for an axis of the SDL GameController 
void FSDLGamepadController::SetAxisDeadZone(int axis, float deadzone)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        Axes[axis].DeadZone = clamp(deadzone, 0.0f, 1.0f);
    }
}

// Set the game axis map for an axis of the SDL GameController 
void FSDLGamepadController::SetAxisMap(int axis, EJoyAxis gameaxis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        Axes[axis].GameAxis = (unsigned(gameaxis) < NUM_JOYAXIS) ? gameaxis : JOYAXIS_None;
    }
}

// Set the scale for an axis of the SDL GameController 
void FSDLGamepadController::SetAxisScale(int axis, float scale)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        Axes[axis].Multiplier = scale;
    }
}

// Check if the sensitivity of the SDL GameController  is default
bool FSDLGamepadController::IsSensitivityDefault()
{
    return Multiplier == 1.0f;
}

// Check if the dead zone for an axis of the SDL GameController  is default
bool FSDLGamepadController::IsAxisDeadZoneDefault(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return Axes[axis].DeadZone == 0.0f;
    }
    return true;
}

// Check if the game axis map for an axis of the SDL GameController  is default
bool FSDLGamepadController::IsAxisMapDefault(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return Axes[axis].GameAxis == JOYAXIS_None;
    }
    return true;
}

// Check if the scale for an axis of the SDL GameController  is default
bool FSDLGamepadController::IsAxisScaleDefault(int axis)
{
    if (axis >= 0 && axis < NUM_AXES)
    {
        return Axes[axis].Multiplier == 1.0f;
    }
    return true;
}

// Get if the SDL GameController  is enabled
bool FSDLGamepadController::GetEnabled()
{
    return gamepad != nullptr;
}

// Set if the SDL GameController  is enabled
void FSDLGamepadController::SetEnabled(bool enabled)
{
    if (enabled && !gamepad)
    {
        gamepad = SDL_GameControllerOpen(Index);
    }
    else if (!enabled && gamepad)
    {
        SDL_GameControllerClose(gamepad);
        gamepad = nullptr;
    }
}

// Set the default configuration for the SDL GameController 
void FSDLGamepadController::SetDefaultConfig()
{
    Multiplier = 1.0f;
    for (int i = 0; i < NUM_AXES; ++i)
    {
        Axes[i].DeadZone = 0.0f;
        Axes[i].GameAxis = JOYAXIS_None;
        Axes[i].Multiplier = 1.0f;
        Axes[i].Value = 0.0f;
        Axes[i].ButtonValue = 0;
    }
}

// Get the identifier for the SDL GameController 
FString FSDLGamepadController::GetIdentifier()
{
    return FStringf("SDL:%d", Index);
}

// Constructor for the SDL GameController manager
FSDLGamepadManager::FSDLGamepadManager()
{
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        // Handle error
    }

    int numGamepads = SDL_NumJoysticks();
    Devices = new FSDLGamepadController * [numGamepads];
    for (int i = 0; i < numGamepads; ++i)
    {
        if (SDL_IsGameController(i))
        {
            Devices[i] = new FSDLGamepadController(i);
        }
        else
        {
            Devices[i] = nullptr;
        }
    }
}

// Destructor for the SDL GameController manager
FSDLGamepadManager::~FSDLGamepadManager()
{
    int numGamepads = SDL_NumJoysticks();
    for (int i = 0; i < numGamepads; ++i)
    {
        delete Devices[i];
    }
    delete[] Devices;

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

// Get if the SDL GameController device is initialized
bool FSDLGamepadManager::GetDevice()
{
    return SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0;
}

// Process input for the SDL GameController manager
void FSDLGamepadManager::ProcessInput()
{
    int numGamepads = SDL_NumJoysticks();
    for (int i = 0; i < numGamepads; ++i)
    {
        if (Devices[i] && Devices[i]->IsConnected())
        {
            Devices[i]->ProcessInput();
        }
    }
}

// Add axes for the SDL GameController manager
void FSDLGamepadManager::AddAxes(float axes[NUM_JOYAXIS])
{
    int numGamepads = SDL_NumJoysticks();
    for (int i = 0; i < numGamepads; ++i)
    {
        if (Devices[i] && Devices[i]->IsConnected())
        {
            Devices[i]->AddAxes(axes);
        }
    }
}

// Get the devices for the SDL GameController manager
void FSDLGamepadManager::GetDevices(TArray<IJoystickConfig*>& sticks)
{
    int numGamepads = SDL_NumJoysticks();
    for (int i = 0; i < numGamepads; ++i)
    {
        if (Devices[i] && Devices[i]->IsConnected())
        {
            sticks.Push(Devices[i]);
        }
    }
}

// Rescan for devices for the SDL GameController manager
IJoystickConfig* FSDLGamepadManager::Rescan()
{
    return NULL;
}

// Initialize SDL GameController
void I_StartupSDLGamepad()
{
    if (!use_joystick || Args->CheckParm("-nojoy"))
    {
        if (JoyDevices[INPUT_SDL] != NULL)
        {
            delete JoyDevices[INPUT_SDL];
            JoyDevices[INPUT_SDL] = NULL;
            UpdateJoystickMenu(NULL);
        }
    }
    else
    {
        if (JoyDevices[INPUT_SDL] == NULL)
        {
            FJoystickCollection* joys = new FSDLGamepadManager;
            if (joys->GetDevice())
            {
                JoyDevices[INPUT_SDL] = joys;
            }
            else
            {
                delete joys;
            }
        }
    }
}

// Shutdown SDL GameController
void I_ShutdownSDLGamepad()
{
    if (JoyDevices[INPUT_SDL] != NULL)
    {
        delete JoyDevices[INPUT_SDL];
        JoyDevices[INPUT_SDL] = NULL;
    }
}