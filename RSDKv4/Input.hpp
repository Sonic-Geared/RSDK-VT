#ifndef INPUT_H
#define INPUT_H

enum InputIDs {
    INPUT_NONE       = 0,
};

enum InputButtons {
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_BUTTONA,
    INPUT_BUTTONB,
    INPUT_BUTTONC,
    INPUT_BUTTONX,
    INPUT_BUTTONY,
    INPUT_BUTTONZ,
    INPUT_BUTTONL,
    INPUT_BUTTONR,
    INPUT_START,
    INPUT_SELECT,
    INPUT_ANY,
    INPUT_BUTTONCOUNT,
};

struct InputData {
    bool up;
    bool down;
    bool left;
    bool right;
    bool A;
    bool B;
    bool C;
    bool X;
    bool Y;
    bool Z;
    bool L;
    bool R;
    bool start;
    bool select;
};

struct InputButton {
    bool press, hold;
    int keyMappings, contMappings;

    inline void setHeld()
    {
        press = !hold;
        hold  = true;
    }
    inline void setReleased()
    {
        press = false;
        hold  = false;
    }

    inline bool down() { return (press || hold); }

    virtual void UpdateInput() {}
    virtual void ProcessInput(int controllerID) {}
    virtual void CloseDevice() {}
    virtual int Unknown1(int unknown1, int unknown2) { return 0; }
    virtual int Unknown2(int unknown1, int unknown2) { return 0; }

    virtual ~InputDevice() = default;

    int gamepadType;
    uint id;
    byte active;
    byte isAssigned;
    byte unused;
    byte disabled;
    byte anyPress;
    int inactiveTimer[2];
};

enum DefaultHapticIDs {
    HAPTIC_NONE = -2,
    HAPTIC_STOP = -1,
};

extern InputData keyPress;
extern InputData keyDown;

extern bool anyPress;

extern int touchDown[8];
extern int touchX[8];
extern int touchY[8];
extern int touchID[8];
extern float touchXF[8];
extern float touchYF[8];
extern int touches;

extern int hapticEffectNum;

extern InputButton inputDevice[INPUT_BUTTONCOUNT];
extern int inputType;

extern int inputSlots[4];

extern float LSTICK_DEADZONE;
extern float RSTICK_DEADZONE;
extern float LTRIGGER_DEADZONE;
extern float RTRIGGER_DEADZONE;

extern int mouseHideTimer;
extern int lastMouseX;
extern int lastMouseY;

#if RETRO_USING_SDL2
// Easier this way
enum ExtraSDLButtons {
    SDL_CONTROLLER_BUTTON_ZL = SDL_CONTROLLER_BUTTON_MAX + 1,
    SDL_CONTROLLER_BUTTON_ZR,
    SDL_CONTROLLER_BUTTON_LSTICK_UP,
    SDL_CONTROLLER_BUTTON_LSTICK_DOWN,
    SDL_CONTROLLER_BUTTON_LSTICK_LEFT,
    SDL_CONTROLLER_BUTTON_LSTICK_RIGHT,
    SDL_CONTROLLER_BUTTON_RSTICK_UP,
    SDL_CONTROLLER_BUTTON_RSTICK_DOWN,
    SDL_CONTROLLER_BUTTON_RSTICK_LEFT,
    SDL_CONTROLLER_BUTTON_RSTICK_RIGHT,
    SDL_CONTROLLER_BUTTON_MAX_EXTRA,
};

void controllerInit(byte controllerID);
void controllerClose(byte controllerID);
#endif

#if RETRO_USING_SDL1
extern byte keyState[SDLK_LAST];

extern SDL_Joystick *controller;
#endif

void InitInputDevices();
void ReleaseInputDevices();

void ProcessInput();

inline uint GetInputDeviceID(byte inputSlot)
{
    byte slotID = inputSlot - 1;
    if (slotID < 4)
        return inputSlots[slotID];

    return INPUT_NONE;
}

inline uint GetFilteredInputDeviceID(bool confirmOnly, bool unassignedOnly, uint maxInactiveTimer)
{
    uint mostRecentTime    = -1;
    int mostRecentValidID = 0;
    int mostRecentID      = 0;
    uint maxTime           = maxInactiveTimer ? maxInactiveTimer : -1;

    if (inputType) {
        for (int i = 0; i < inputType; ++i) {
            if (inputDevice[i] && inputDevice[i].active && !inputDevice[i].disabled
                && (!inputDevice[i].isAssigned || !unassignedOnly)) {
                if (inputDevice[i].inactiveTimer[confirmOnly] < mostRecentTime) {
                    mostRecentTime = inputDevice[i].inactiveTimer[confirmOnly];
                    if (inputDevice[i].inactiveTimer[confirmOnly] <= maxTime)
                        mostRecentValidID = inputDevice[i].id;
                    mostRecentID = inputDevice[i].id;
                }
            }
        }

        if (mostRecentValidID)
            return mostRecentValidID;
    }

    if (mostRecentID)
        return mostRecentID;

    for (int i = 0; i < inputType; ++i) {
        if (inputDevice[i] && inputDevice[i].active && !inputDevice[i].disabled
            && (!inputDevice[i].isAssigned || !unassignedOnly)) {
            return inputDevice[i].id;
        }
    }

    return mostRecentID;
}

void CheckKeyPress(InputData *input);
void CheckKeyDown(InputData *input);

int CheckTouchRect(float x1, float y1, float x2, float y2);
int CheckTouchRectMatrix(void *m, float x1, float y1, float x2, float y2);

#if RETRO_USE_HAPTICS
inline int GetHapticEffectNum()
{
    int num         = hapticEffectNum;
    hapticEffectNum = HAPTIC_NONE;
    return num;
}
void HapticEffect(int *id, int *a2, int *a3, int *a4);
#endif

#endif // !INPUT_H
