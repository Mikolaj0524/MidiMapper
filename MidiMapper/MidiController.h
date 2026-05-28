#pragma once

#include <windows.h>

namespace MidiController {
	extern int lastChanged;

	extern void Init();
	void SendCC(int channel, int cc);
	void CALLBACK MidiProc(HMIDIIN, UINT wMsg, DWORD_PTR, DWORD_PTR dwParam1, DWORD_PTR);
	void Close();
}