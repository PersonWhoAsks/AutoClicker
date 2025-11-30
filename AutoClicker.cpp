#include <Windows.h>
#include <iostream>
#include <thread>
using namespace std;

int toggleKey = 0xBE;   // . key
int endKey = 0xC0;      // ` key
int delayTime = 10;     // in ms

bool isEnabled = false;

void click() {
    while (true) {
        INPUT inputs[2] = {};

        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        if (isEnabled) {
            UINT uSent = SendInput(2, inputs, sizeof(INPUT));
            cout << "Sent input" << endl;
        }

        Sleep(delayTime);
    }
}

int main() {
    thread t1(click);
    while (true) {
        // Handle toggle key
		if (GetKeyState(toggleKey) & 0x8000) {
            isEnabled = !isEnabled;
            cout << "Toggled to: " << (isEnabled == true ? "True" : "False") << endl;
            Sleep(300); 
		}

        // Handle end key
        if (GetKeyState(endKey)) {
            cout << "End key pressed, exiting." << endl;
			isEnabled = false;
			t1.detach();
            return 0;
        }
    }
    return 0;
}
