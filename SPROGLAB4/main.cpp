#include <windows.h>

HINSTANCE hInst;
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "LAB4";

HWND hButtonRect, hButtonEllipse, hButtonRectOutlineColor, hButtonRectFillColor, hButtonEllipseOutlineColor, hButtonEllipseFillColor;
HWND hComboBoxRect, hComboBoxEllipse;  // Дескриптори ComboBox для вибору масштабу
BOOL bShowRect = FALSE, bShowEllipse = FALSE;
COLORREF rectOutlineColor = RGB(0, 0, 0), rectFillColor = RGB(255, 255, 255);
COLORREF ellipseOutlineColor = RGB(0, 0, 0), ellipseFillColor = RGB(255, 255, 255);
int rectScale = 100, ellipseScale = 100;  // Значення масштабу за замовчуванням
int sharedCursorType = 0;  // 0 for default, 1 for cross cursor


ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_HAND);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    hInst = hInstance;
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    // Створення кнопок checkbox
    hButtonRect = CreateWindow("button", "Rect", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        10, 200, 80, 20, hWnd, (HMENU)1, hInstance, NULL);

    hButtonEllipse = CreateWindow("button", "Ellipse", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        100, 200, 80, 20, hWnd, (HMENU)2, hInstance, NULL);

    // Створення кнопок pushbutton для вибору кольорів для прямокутника
    hButtonRectOutlineColor = CreateWindow("button", "Rect Outline Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 230, 150, 30, hWnd, (HMENU)3, hInstance, NULL);

    hButtonRectFillColor = CreateWindow("button", "Rect Fill Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        170, 230, 150, 30, hWnd, (HMENU)4, hInstance, NULL);

    // Створення кнопок pushbutton для вибору кольорів для еліпса
    hButtonEllipseOutlineColor = CreateWindow("button", "Ellipse Outline Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 270, 150, 30, hWnd, (HMENU)5, hInstance, NULL);

    hButtonEllipseFillColor = CreateWindow("button", "Ellipse Fill Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        170, 270, 150, 30, hWnd, (HMENU)6, hInstance, NULL);

    // Створення ComboBox для вибору масштабу для прямокутника
    hComboBoxRect = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        330, 230, 80, 300, hWnd, (HMENU)7, hInstance, NULL);

    // Додаємо елементи в ComboBox для прямокутника
    SendMessage(hComboBoxRect, CB_ADDSTRING, 0, (LPARAM)"10%");
    SendMessage(hComboBoxRect, CB_ADDSTRING, 0, (LPARAM)"50%");
    SendMessage(hComboBoxRect, CB_ADDSTRING, 0, (LPARAM)"100%");
    SendMessage(hComboBoxRect, CB_ADDSTRING, 0, (LPARAM)"200%");

    // Вибираємо елемент за замовчуванням
    SendMessage(hComboBoxRect, CB_SETCURSEL, 2, 0);

    // Створення ComboBox для вибору масштабу для еліпса
    hComboBoxEllipse = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        330, 270, 80, 300, hWnd, (HMENU)8, hInstance, NULL);

    // Додаємо елементи в ComboBox для еліпса
    SendMessage(hComboBoxEllipse, CB_ADDSTRING, 0, (LPARAM)"10%");
    SendMessage(hComboBoxEllipse, CB_ADDSTRING, 0, (LPARAM)"50%");
    SendMessage(hComboBoxEllipse, CB_ADDSTRING, 0, (LPARAM)"100%");
    SendMessage(hComboBoxEllipse, CB_ADDSTRING, 0, (LPARAM)"200%");

    // Вибираємо елемент за замовчуванням
    SendMessage(hComboBoxEllipse, CB_SETCURSEL, 2, 0);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rt;
    int rectWidth = MulDiv(150, rectScale, 100);
    int rectHeight = MulDiv(100, rectScale, 100);
    int ellipseWidth = MulDiv(150, ellipseScale, 100);
    int ellipseHeight = MulDiv(100, ellipseScale, 100);

    switch (message)
    {
    case WM_CREATE: {

        // Create radio buttons for rectangle cursor type
        CreateWindow("button", "Uparrow", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            10, 300, 80, 20, hWnd, (HMENU)9, hInst, NULL);

        CreateWindow("button", "Cross", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            100, 300, 80, 20, hWnd, (HMENU)10, hInst, NULL);
    }
        break;


    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELCHANGE) {
            if (LOWORD(wParam) == 7) { // ComboBox for Rect
                int index = SendMessage(hComboBoxRect, CB_GETCURSEL, 0, 0);
                switch (index) {
                case 0: rectScale = 10; break;
                case 1: rectScale = 50; break;
                case 2: rectScale = 100; break;
                case 3: rectScale = 200; break;
                }
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (LOWORD(wParam) == 8) { // ComboBox for Ellipse
                int index = SendMessage(hComboBoxEllipse, CB_GETCURSEL, 0, 0);
                switch (index) {
                case 0: ellipseScale = 10; break;
                case 1: ellipseScale = 50; break;
                case 2: ellipseScale = 100; break;
                case 3: ellipseScale = 200; break;
                }
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else if (LOWORD(wParam) == 1) {  // Кнопка "Rect"
            bShowRect = !bShowRect;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (LOWORD(wParam) == 2) {  // Кнопка "Ellipse"
            bShowEllipse = !bShowEllipse;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (LOWORD(wParam) == 3) {  // Кнопка "Rect Outline Color"
            CHOOSECOLOR cc;
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hWnd;
            cc.lpCustColors = (LPDWORD)(&rectOutlineColor);
            cc.rgbResult = rectOutlineColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc)) {
                rectOutlineColor = cc.rgbResult;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else if (LOWORD(wParam) == 4) {  // Кнопка "Rect Fill Color"
            CHOOSECOLOR cc;
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hWnd;
            cc.lpCustColors = (LPDWORD)(&rectFillColor);
            cc.rgbResult = rectFillColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc)) {
                rectFillColor = cc.rgbResult;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else if (LOWORD(wParam) == 5) {  // Кнопка "Ellipse Outline Color"
            CHOOSECOLOR cc;
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hWnd;
            cc.lpCustColors = (LPDWORD)(&ellipseOutlineColor);
            cc.rgbResult = ellipseOutlineColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc)) {
                ellipseOutlineColor = cc.rgbResult;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else if (LOWORD(wParam) == 6) {  // Кнопка "Ellipse Fill Color"
            CHOOSECOLOR cc;
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hWnd;
            cc.lpCustColors = (LPDWORD)(&ellipseFillColor);
            cc.rgbResult = ellipseFillColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc)) {
                ellipseFillColor = cc.rgbResult;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        if (HIWORD(wParam) == BN_CLICKED) {
            // Cursor type selection
            if (LOWORD(wParam) == 9) {
                sharedCursorType = 0;  // Default cursor
            }
            else if (LOWORD(wParam) == 10) {
                sharedCursorType = 1;  // Cross cursor
            }
        }
    }
        break;

    case WM_PAINT: {
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rt);


        // Малюємо прямокутник, якщо кнопка "Rect" вибрана
        if (bShowRect) {
            // Застосовуємо масштаб до координат прямокутника

            HBRUSH hBrush = CreateSolidBrush(rectFillColor);
            HPEN hPen = CreatePen(PS_SOLID, 2, rectOutlineColor);
            SelectObject(hdc, hBrush);
            SelectObject(hdc, hPen);
            Rectangle(hdc, 50, 50, 50 + rectWidth, 50 + rectHeight);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }

        // Малюємо еліпс, якщо кнопка "Ellipse" вибрана
        if (bShowEllipse) {
            // Застосовуємо масштаб до координат еліпса

            HBRUSH hBrush = CreateSolidBrush(ellipseFillColor);
            HPEN hPen = CreatePen(PS_SOLID, 2, ellipseOutlineColor);
            SelectObject(hdc, hBrush);
            SelectObject(hdc, hPen);
            Ellipse(hdc, 250, 50, 250 + ellipseWidth, 50 + ellipseHeight);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }
        EndPaint(hWnd, &ps);
    }
        break;
    case WM_MOUSEMOVE: {
        // Determine mouse position
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);

        // Check if the mouse is over the rectangle and set cursor type
        bool radioSelected = IsDlgButtonChecked(hWnd, 9) || IsDlgButtonChecked(hWnd, 10);

        // Check if the mouse is over the rectangle and set cursor type
        if (bShowRect && mouseX >= 50 && mouseX <= 50 + rectWidth &&
            mouseY >= 50 && mouseY <= 50 + rectHeight) {
            SetCursor(radioSelected ? ((sharedCursorType == 0) ? LoadCursor(NULL, IDC_UPARROW) : LoadCursor(NULL, IDC_CROSS))
                : LoadCursor(NULL, IDC_ARROW));
        }

        // Check if the mouse is over the ellipse and set cursor type
        if (bShowEllipse && mouseX >= 250 && mouseX <= 250 + ellipseWidth &&
            mouseY >= 50 && mouseY <= 50 + ellipseHeight) {
            SetCursor(radioSelected ? ((sharedCursorType == 0) ? LoadCursor(NULL, IDC_UPARROW) : LoadCursor(NULL, IDC_CROSS))
                : LoadCursor(NULL, IDC_ARROW));
        }

    }
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
