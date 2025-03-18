#pragma once
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <time.h>
#include <VersionHelpers.h>
#include <memory>
#include "i18n.h"
#include "resource.h"
#include "globaldata.h"
#include "otp.h"
#include "DataHandler.h"
#include "encode.h"

#pragma comment(lib, "comctl32.lib")



// Macro Definitions for Button IDs
#define IDC_BUTTON_IMPORT      1001
#define IDC_BUTTON_EXPORT      1002
#define IDC_BUTTON_ADD         1003
#define IDC_BUTTON_EDIT        1004
#define IDC_BUTTON_DELETE      1005
#define IDC_BUTTON_NETWORK_TIME 1006

// Function Declarations
int APIENTRY mainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void AddControls(HWND hWnd);

