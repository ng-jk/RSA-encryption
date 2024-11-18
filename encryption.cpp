#include <windows.h>
#include "RSA.h"
#include <string>

// Define control IDs
#define IDC_PEDIT_TEXT 101
#define IDC_QEDIT_TEXT 102
#define IDC_MEDIT_TEXT 103
#define IDC_P_TEXT 104
#define IDC_Q_TEXT 105
#define IDC_M_TEXT 106
#define IDC_RSA_ENCRYPT_CHECK 107
#define IDC_ASCII_CHECK 108
#define IDC_ENCRYPT_BUTTON 109
#define IDC_DECRYPT_BUTTON 110

int ciphertext[256];
char plaintext[256];
std::string plaintextString;
std::string ciphertextString;

// Function prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Function to handle encryption based on checkboxes
void PerformEncryption(HWND hwnd, char m[],char p[],char q[]) {
        int mInt,pInt,qInt,text;
        std::string temp;
        ciphertextString = "";  // Clear previous ciphertext
        temp = "";

        for (size_t i = 0; i < strlen(p); i++){
            temp += p[i];
        }
        pInt=stoi(temp);
        temp = "";
        for (size_t i = 0; i < strlen(q); i++){
            temp += q[i];
        }
        qInt=stoi(temp);
        temp = "";

        // ASCII checkbox checked
        if (SendMessage(GetDlgItem(hwnd, IDC_ASCII_CHECK), BM_GETCHECK, 0, 0) == BST_CHECKED) {
            for (size_t i = 0; i < strlen(m); i++) {
                text = (int)m[i];
                ciphertext[i] = RSAencryption(text,pInt,qInt);
                ciphertextString += std::to_string(ciphertext[i]) + " ";
            }
        } else {
            // Convert characters directly if ASCII checkbox is unchecked
            for (size_t i = 0; i < strlen(m); i++) {
                temp += m[i];
            }
            text = stoi(temp);
            ciphertextString = to_string(RSAencryption(text,pInt,qInt)); 
        }
    MessageBox(hwnd, ciphertextString.c_str(), "The ciphertext is:", MB_OK);
}

// Function to handle decryption based on checkboxes
void PerformDecryption(HWND hwnd, char m[],char p[],char q[]) {
    int mInt,pInt,qInt,text;
    std::string temp;
    plaintextString = "";  // Clear previous ciphertext
    temp = "";
    for (size_t i = 0; i < strlen(p); i++){
        temp += p[i];
    }
    pInt=stoi(temp);
    temp = "";
    for (size_t i = 0; i < strlen(q) ; i++){
        temp += q[i];
    }
    qInt=stoi(temp);
    temp = "";
    // ASCII checkbox checked
    if (SendMessage(GetDlgItem(hwnd, IDC_ASCII_CHECK), BM_GETCHECK, 0, 0) == BST_CHECKED) {
        for (size_t i = 0; i < strlen(m); i++) {
            text = static_cast<int>(m[i]);
            plaintext[i] = RSAdecryption(text,pInt,qInt);
            plaintextString += std::to_string(static_cast<char>(ciphertext[i])) + " ";
        }
    } else {
        // Convert characters directly if ASCII checkbox is unchecked
        for (size_t i = 0; i < strlen(m); i++) {
            temp += m[i];
        }
        text = stoi(temp);
        plaintextString = to_string(RSAdecryption(text,pInt,qInt)); 
    }
    MessageBox(hwnd, plaintextString.c_str(), "The plaintext is:", MB_OK);
}

// WinMain: The application's entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "CryptoApp";

    RegisterClass(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, "RSA Encryption Application",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                             100, 100, 500, 400, NULL, NULL, hInstance, NULL);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

// Window procedure to handle messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND pEditText;
    static HWND qEditText;
    static HWND mEditText;

    switch(msg) {
        case WM_CREATE: {
            // Create edit box for input
            CreateWindow("STATIC", "Enter P", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 20, 450, 25, hwnd, (HMENU)IDC_Q_TEXT, NULL, NULL);
            qEditText = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 50, 450, 25, hwnd, (HMENU)IDC_QEDIT_TEXT, NULL, NULL);
            CreateWindow("STATIC", "Enter Q", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 80, 450, 25, hwnd, (HMENU)IDC_P_TEXT, NULL, NULL);
            pEditText = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 110, 450, 25, hwnd, (HMENU)IDC_PEDIT_TEXT, NULL, NULL);
            CreateWindow("STATIC", "Enter message", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 140, 450, 25, hwnd, (HMENU)IDC_M_TEXT, NULL, NULL);
            mEditText = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 170, 450, 25, hwnd, (HMENU)IDC_MEDIT_TEXT, NULL, NULL);
            // ASCII checkbox
            CreateWindow("BUTTON", "ASCII", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                         20, 200, 100, 25, hwnd, (HMENU)IDC_ASCII_CHECK, NULL, NULL);

            // Encrypt and Decrypt buttons
            CreateWindow("BUTTON", "Encrypt", WS_CHILD | WS_VISIBLE,
                         100, 260, 100, 30, hwnd, (HMENU)IDC_ENCRYPT_BUTTON, NULL, NULL);
            CreateWindow("BUTTON", "Decrypt", WS_CHILD | WS_VISIBLE,
                         300, 260, 100, 30, hwnd, (HMENU)IDC_DECRYPT_BUTTON, NULL, NULL);
            break;
        }

        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case IDC_ENCRYPT_BUTTON: {
                    char m[256],p[256],q[256];
                    GetWindowText(mEditText, m, sizeof(m));
                    GetWindowText(pEditText, p, sizeof(p));
                    GetWindowText(qEditText, q, sizeof(q));

                    PerformEncryption(hwnd, m,p,q);
                    break;
                }
                case IDC_DECRYPT_BUTTON: {
                    char m[256],p[256],q[256];
                    GetWindowText(mEditText, m, sizeof(m));
                    GetWindowText(pEditText, p, sizeof(p));
                    GetWindowText(qEditText, q, sizeof(q));

                    PerformDecryption(hwnd, m,p,q);
                    break;
                }
                default:
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
