#include "MidiController.h"
#include <windows.h>
#include <mmeapi.h>
#include "ConfigManager.h"
#include "Keyboard.h"

#pragma comment(lib, "winmm.lib")

namespace MidiController{
    typedef unsigned char uchar;

    const wchar_t* controllerName = L"LC3 1 MIDI";
    const int middle = 63;

    UINT inputDevice = 0;
    UINT outputDevice = 0;

    HMIDIIN midiIn = NULL;
    HMIDIOUT midiOut = NULL;

    int lastChanged = -1;

    void Init(){
        for(UINT i = 0; i < midiInGetNumDevs(); i++){
            MIDIINCAPSW caps;
            midiInGetDevCapsW(i, &caps, sizeof(caps));

            if (wcscmp(caps.szPname, controllerName) == 0)
                inputDevice = i;
        }

        for (UINT i = 0; i < midiOutGetNumDevs(); i++) {
            MIDIOUTCAPSW caps;
            midiOutGetDevCapsW(i, &caps, sizeof(caps));

            if (wcscmp(caps.szPname, controllerName) == 0)
                outputDevice = i;
        }
        
        midiInOpen(&midiIn, inputDevice, (DWORD_PTR)MidiProc, 0, CALLBACK_FUNCTION);
        midiInStart(midiIn);
        midiOutOpen(&midiOut, outputDevice, 0, 0, 0);

        for (int i = 0; i <= 15; i++) {
            SendCC(0, i);
        }
    }

    void PressKey(Shortcut& shortcut, int amount) {
        float repeats = amount == 1 ? 1 : (amount + amount * 0.5f);

        for (int i = 0; i < repeats; i++) {
            Keyboard::Key(shortcut);
        }
    }

    void CALLBACK MidiProc(HMIDIIN, UINT wMsg, DWORD_PTR, DWORD_PTR dwParam1, DWORD_PTR) {
        if (wMsg != MIM_DATA)
            return;

        uchar status = dwParam1 & 0xFF;
        uchar cc = (dwParam1 >> 8) & 0xFF;
        uchar value = (dwParam1 >> 16) & 0xFF;
        uchar type = status & 0xF0;
        uchar channel = status & 0x0F;

        if (type != 0xB0 || value == middle)
            return;

        auto &config = ConfigManager::config;

        if(value == 0){
            PressKey(config[cc].first, 1);
        }
        else if(value < middle){
            PressKey(config[cc].first, middle - value);
        }
        else {
            PressKey(config[cc].second, value - middle);
        }

        lastChanged = cc;
        SendCC(channel, cc);
    }

    void SendCC(int channel, int cc) {
        if (!midiOut)
            return;

        DWORD msg = (0xB0 | (channel & 0x0F)) | (cc << 8) | (middle << 16);
        midiOutShortMsg(midiOut, msg);
    }

    void Close(){
        if(midiIn){
            midiInStop(midiIn);
            midiInClose(midiIn);
        }

        if (midiOut)
            midiOutClose(midiOut);
    }
}