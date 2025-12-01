#include <Windows.h>
#include <iostream>
#include <thread>
#include <semaphore>
using namespace std;

int toggleKey = 0xBE;   // . key
int endKey = 0xC0;      // ` key
int delayTime = 10;     // in ms

bool isEnabled = false;
binary_semaphore sem(1);

bool read_isEnabled() {
    if (sem.try_acquire_for(chrono::seconds(1))) {
        bool val = isEnabled;
        sem.release();
        return val;
    }
    else {
		cout << "Failed to acquire semaphore within 1 second." << endl;
		return false;
    }
}

void set_isEnabled(bool val) {
    if (sem.try_acquire_for(chrono::seconds(1))) {
        isEnabled = val;
        sem.release();
    }
    else {
        cout << "Failed to acquire semaphore within 1 second." << endl;
    }
}

void click() {
    while (true) {
        INPUT inputs[2] = {};

        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        if (read_isEnabled()) {
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
			set_isEnabled(!read_isEnabled());
            cout << "Toggled to: " << (read_isEnabled() == true ? "True" : "False") << endl;
            Sleep(300); 
		}

        // Handle end key
        if (GetKeyState(endKey)) {
            cout << "End key pressed, exiting." << endl;
			set_isEnabled(false);
			t1.detach();
            return 0;
        }
    }
    return 0;
}
