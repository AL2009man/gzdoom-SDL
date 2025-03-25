/*
**
**---------------------------------------------------------------------------
** Copyright 2025 Al. Lopez (AL2009man)
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

#ifndef SDL_HID_H
#define SDL_HID_H

#include <SDL2/SDL.h>

// Define the number of axes here if not defined elsewhere
#ifndef NUM_AXES
#define NUM_AXES 6
#endif

// Axis information structure
struct AxisInfo
{
    float DeadZone;
    EJoyAxis GameAxis;
    float Multiplier;
    float Value;
    int ButtonValue;
};

// Gamepad controller class for SDL
class FSDLGamepadController
{
public:
    FSDLGamepadController(int index);
    ~FSDLGamepadController();

    void ProcessInput();
    void AddAxes(float axes[NUM_JOYAXIS]);

    FString GetName();
    float GetSensitivity();
    void SetSensitivity(float scale);

    int GetNumAxes();
    float GetAxisDeadZone(int axis);
    EJoyAxis GetAxisMap(int axis);
    const char* GetAxisName(int axis);
    float GetAxisScale(int axis);

    void SetAxisDeadZone(int axis, float deadzone);
    void SetAxisMap(int axis, EJoyAxis gameaxis);
    void SetAxisScale(int axis, float scale);

    bool IsSensitivityDefault();
    bool IsAxisDeadZoneDefault(int axis);
    bool IsAxisMapDefault(int axis);
    bool IsAxisScaleDefault(int axis);

    bool GetEnabled();
    void SetEnabled(bool enabled);

    void SetDefaultConfig();

    FString GetIdentifier();

    bool IsConnected() { return gamepad != nullptr; }

private:
    int Index;
    SDL_GameController* gamepad;
    AxisInfo* Axes;
    float Multiplier;
};

// Gamepad manager class for SDL
class FSDLGamepadManager : public FJoystickCollection
{
public:
    FSDLGamepadManager();
    ~FSDLGamepadManager();

    bool GetDevice();
    void ProcessInput();
    void AddAxes(float axes[NUM_JOYAXIS]);
    void GetDevices(TArray<IJoystickConfig*>& sticks);
    IJoystickConfig* Rescan();

private:
    FSDLGamepadController** Devices;
};

// Initialize SDL gamepad
void I_StartupSDLGamepad();

// Shutdown SDL gamepad
void I_ShutdownSDLGamepad();

#endif // SDL_HID_H