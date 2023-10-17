#include <iostream>
#include <Windows.h>

#define game_window_name "Counter-Strike 2"

DWORD dwPid = 0;
HWND hGameWnd = NULL;
bool bAccept = false;

bool isProcessRunning(DWORD processID) //yoink
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, processID);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

int main(int argc, char** argv) {
	SetWindowTextA(GetConsoleWindow(), "KEY: F1");
	printf_s("Auto accept key: F1\n");

GAME_CLOSED:
	printf_s("Searching for game...\n");
	while (!hGameWnd) {
		hGameWnd = FindWindowA(NULL, game_window_name);
		Sleep(10);
	}

	GetWindowThreadProcessId(hGameWnd, &dwPid);
	printf_s("Game found! [PID: %d]\n", dwPid);

	while (true) {
		if (GetAsyncKeyState(VK_F1)) {
			bAccept = !bAccept;

			if (bAccept) {
				SetWindowTextA(GetConsoleWindow(), "AUTOACCEPT: ON");
			}
			else {
				SetWindowTextA(GetConsoleWindow(), "AUTOACCEPT: OFF");
			}

			Beep(500, 50);
		}

		if (!isProcessRunning(dwPid)) {
			printf_s("Game closed...\n");
			hGameWnd = NULL;
			goto GAME_CLOSED;
		}

		if (bAccept) {
			RECT WindowRect;
			GetWindowRect(hGameWnd, &WindowRect);

			INPUT inputs[3] = {};
			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dx = (WindowRect.right / 2) * 65536 / GetSystemMetrics(SM_CXSCREEN);
			inputs[0].mi.dy = (WindowRect.bottom / 1.25) * 65536 / GetSystemMetrics(SM_CXSCREEN);
			inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

			inputs[1].type = INPUT_MOUSE;
			inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

			inputs[2].type = INPUT_MOUSE;
			inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

			SendInput(3, inputs, sizeof(INPUT));
		}

		Sleep(1000);
	}
}