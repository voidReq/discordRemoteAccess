#include <windows.h>
#include <windowsx.h> 
#include <commctrl.h> // For WC_EDIT, WC_STATIC, though often covered by windows.h
#include <stdio.h>    // For sprintf_s or sprintf
#include <stdlib.h>   // For rand(), srand(), atoi()
#include <time.h>     // For time()


#define IDC_EDIT_PLAYER_CHOICE 101
#define IDC_BUTTON_PLAY        102
#define IDC_STATIC_PROMPT      103
#define IDC_STATIC_PLAYER_DISPLAY 104
#define IDC_STATIC_COMPUTER_DISPLAY 105
#define IDC_STATIC_RESULT_DISPLAY 106


#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 300 


static HWND hEditInput;
static HWND hButtonPlay;
static HWND hStaticPlayerDisplay;
static HWND hStaticComputerDisplay;
static HWND hStaticResultDisplay;


enum {
    CHOICE_NONE = 0,
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3
};


const char* ChoiceToString(int choice) {
    switch (choice) {
        case ROCK: return "Rock";
        case PAPER: return "Paper";
        case SCISSORS: return "Scissors";
        default: return "Invalid";
    }
}


void HandlePlayAction(HWND hwnd) {
    char buffer[10];
    GetWindowText(hEditInput, buffer, sizeof(buffer));

    int playerChoice = atoi(buffer); 

    if (playerChoice < ROCK || playerChoice > SCISSORS) {
        SetWindowText(hStaticPlayerDisplay, "Your choice: -");
        SetWindowText(hStaticComputerDisplay, "Computer's choice: -");
        SetWindowText(hStaticResultDisplay, "Result: Please enter 1, 2, or 3.");
        SetFocus(hEditInput); 
        SendMessage(hEditInput, EM_SETSEL, 0, -1); 
        return;
    }

    
    int computerChoice = (rand() % 3) + 1;

    char displayBuffer[100];

    
    snprintf(displayBuffer, sizeof(displayBuffer), "Your choice: %s", ChoiceToString(playerChoice));
    SetWindowText(hStaticPlayerDisplay, displayBuffer);

    
    snprintf(displayBuffer, sizeof(displayBuffer), "Computer's choice: %s", ChoiceToString(computerChoice));
    SetWindowText(hStaticComputerDisplay, displayBuffer);

    
    const char* resultText;
    if (playerChoice == computerChoice) {
        resultText = "Result: It's a Draw!";
    } else if ((playerChoice == ROCK && computerChoice == SCISSORS) ||
               (playerChoice == PAPER && computerChoice == ROCK) ||
               (playerChoice == SCISSORS && computerChoice == PAPER)) {
        resultText = "Result: You Win!";
    } else {
        resultText = "Result: Computer Wins!";
    }
    SetWindowText(hStaticResultDisplay, resultText);
    SetFocus(hEditInput); 
    SendMessage(hEditInput, EM_SETSEL, 0, -1); 
}


LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HINSTANCE hInstance = GetModuleHandle(NULL);

            
            srand((unsigned int)time(NULL));

            
            CreateWindowEx(0, WC_STATIC, "Enter 1 (Rock), 2 (Paper), or 3 (Scissors):",
                           WS_CHILD | WS_VISIBLE,
                           10, 10, 460, 20,
                           hwnd, (HMENU)IDC_STATIC_PROMPT, hInstance, NULL);

            
            hEditInput = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, "",
                                      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_AUTOHSCROLL,
                                      10, 40, 60, 25, // x, y, width, height
                                      hwnd, (HMENU)IDC_EDIT_PLAYER_CHOICE, hInstance, NULL);

            
            hButtonPlay = CreateWindowEx(0, WC_BUTTON, "Play",
                                       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                       80, 40, 80, 25, // x, y, width, height
                                       hwnd, (HMENU)IDC_BUTTON_PLAY, hInstance, NULL);

            
            hStaticPlayerDisplay = CreateWindowEx(0, WC_STATIC, "Your choice: -",
                                               WS_CHILD | WS_VISIBLE,
                                               10, 80, 460, 20,
                                               hwnd, (HMENU)IDC_STATIC_PLAYER_DISPLAY, hInstance, NULL);

            
            hStaticComputerDisplay = CreateWindowEx(0, WC_STATIC, "Computer's choice: -",
                                                 WS_CHILD | WS_VISIBLE,
                                                 10, 110, 460, 20,
                                                 hwnd, (HMENU)IDC_STATIC_COMPUTER_DISPLAY, hInstance, NULL);

            
            hStaticResultDisplay = CreateWindowEx(0, WC_STATIC, "Result: Make your move!",
                                               WS_CHILD | WS_VISIBLE,
                                               10, 140, 460, 20,
                                               hwnd, (HMENU)IDC_STATIC_RESULT_DISPLAY, hInstance, NULL);
            SetFocus(hEditInput); 
            break;
        }

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);    
            

            if (wmId == IDC_BUTTON_PLAY) { 
                HandlePlayAction(hwnd);
            }
            break;
        }

        case WM_KEYDOWN: {
            if (wParam == VK_RETURN) { 
                if (GetFocus() == hEditInput) { 
                    
                    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_PLAY, BN_CLICKED), (LPARAM)hButtonPlay);
                    return 0; // Message handled
                } else if (GetFocus() == hButtonPlay) {
                     
                }
            }
            break;
        }

        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0); 
            break;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void *run_terminal(void *arg) {
    (void)arg;

    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "RockPaperScissorsGameWindowClass";

    WNDCLASSEX wc = {0}; 
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = GameWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return (void*)1; 
    }

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Rock Paper Scissors Game",       // Window title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style (visible)
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        WINDOW_WIDTH, WINDOW_HEIGHT,    // Size
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        UnregisterClass(CLASS_NAME, hInstance);
        return (void*)1; 
    }

    
    

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        
        
        
        // if (!IsDialogMessage(hwnd, &msg)) {
             TranslateMessage(&msg);
             DispatchMessage(&msg);
        // }
    }
    
    UnregisterClass(CLASS_NAME, hInstance); 
    return (void*)0; 
}
