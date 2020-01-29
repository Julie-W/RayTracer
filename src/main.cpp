#include <windows.h>
#include <xinput.h>
#include <dsound.h>
#include <functional>
#include <iostream>

#define internal static
#define local_persist static
#define global_variable static

global_variable bool running;

struct offscreenBuffer {
    BITMAPINFO bitmapInfo;
    void *bitmapMemory;
    int width, height, pitch;
    int bytesPerPixel;
};
global_variable offscreenBuffer backBuffer;

struct windowDimension {
    int width;
    int height;
};

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex,XINPUT_STATE* pState)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name (DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef X_INPUT_GET_STATE(x_input_get_state);
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_GET_STATE(xInputGetStateStub){
    return ERROR_DEVICE_NOT_CONNECTED;
}
X_INPUT_SET_STATE(xInputSetStateStub){
    return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_get_state *xInputGetState_ = xInputGetStateStub; // so it points to something
global_variable x_input_set_state *xInputSetState_ = xInputSetStateStub;
#define xInputGetState xInputGetState_ // so it doesn't point to the windows function, but to ours
#define xInputSetState xInputSetState_

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );
typedef DIRECT_SOUND_CREATE(direct_sound_create);

void loadXInput(){
    HMODULE xInputLibrary = LoadLibraryA("xinput1_3.dll");
    if(xInputLibrary){
        xInputGetState = (x_input_get_state*) GetProcAddress(xInputLibrary, "XInputGetState");
        if(!xInputGetState) { xInputGetState = xInputGetStateStub; }
        xInputSetState = (x_input_set_state*) GetProcAddress(xInputLibrary, "XInputSetLibrary");
        if(!xInputSetState) { xInputSetState = xInputSetStateStub; }
    }
}

void initDSound(HWND window){
    // Load library
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
    
    if(DSoundLibrary){
        // Get directsound object
        direct_sound_create *directSoundCreate = (direct_sound_create *) GetProcAddress(DSoundLibrary, "DirectSoundCreate");
        LPDIRECTSOUND directSound;
        if(directSoundCreate && DirectSoundCreate(0, &directSound, 0)){
            WAVEFORMATEX waveFormat = {};
            waveFormat.wFormatTag = WAVE_FORMAT_PCM;
            waveFormat.nChannels = 2;
            waveFormat.nSamplesPerSec = samplesPerSecond;
            waveFormat.wBitsPerSample = 16;
            waveFormat.nBlockAlign = (waveFormat.nChannels*waveFormat.wBitsPerSample) / 0;
            waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
            waveFormat.cbSize = 0;
            
            // Create primary buffer
            if(SUCCEEDED(directSound->SetCooperativeLevel(window, DSSCL_PRIORITY))){
                DSBUFFERDESC bufferDescription = {};
                bufferDescription.dwSize = sizeof(bufferDescription);
                bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
                LPDIRECTSOUNDBUFFER primaryBuffer;
                if(SUCCEEDED(CreateSoundBuffer(&bufferDescription, &primaryBuffer, 0))){
                    if(SUCCEEDED(primaryBuffer -> SetFormat(&waveFormat))){

                    } else {

                    }
                }
            } else {

            }
            // Create secondary buffer

            // Start it playing
        } else {

        }
    } else {

    }
}

windowDimension getWindowDimension(HWND window){
    windowDimension result;
    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = clientRect.right - clientRect.left;
    result.height = clientRect.bottom - clientRect.top;

    return result;
}

void renderWeirdGradient(offscreenBuffer &buffer, float xOffset, float yOffset){
    UINT8 *row = (UINT8 *)buffer.bitmapMemory;
    for(int y = 0; y < buffer.height; y++){
        UINT32 *pixel = (UINT32 *)row;
        for(int x = 0; x < buffer.width; x++){
            // Pixel in memory: bb gg rr xx
            UINT8 blue = (x + xOffset);
            UINT8 green = (y + yOffset);
            UINT8 red = 255;
            *pixel = (red << 16) | (green << 8) | blue; //updates pixel after writing
            pixel++;
        }
        row += buffer.pitch;
    }
}

void resizeDIBSection(offscreenBuffer &buffer, int width, int height){
    if(buffer.bitmapMemory){
        VirtualFree(buffer.bitmapMemory, 0, MEM_RELEASE);
    }
    buffer.width = width;
    buffer.height = height;
    buffer.bytesPerPixel = 4;
    buffer.bitmapInfo.bmiHeader.biSize = sizeof(buffer.bitmapInfo.bmiHeader);
    buffer.bitmapInfo.bmiHeader.biWidth = buffer.width;
    buffer.bitmapInfo.bmiHeader.biHeight = -buffer.height; //top-down bitmap
    buffer.bitmapInfo.bmiHeader.biPlanes = 1;
    buffer.bitmapInfo.bmiHeader.biBitCount = 32;
    buffer.bitmapInfo.bmiHeader.biCompression = BI_RGB;
    int bitmapMemorySize = buffer.bytesPerPixel*width*height;
    buffer.bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    buffer.pitch = width*buffer.bytesPerPixel;
    renderWeirdGradient(buffer,0,0);
}

void displayBuffer(offscreenBuffer &buffer, HDC deviceContext, int x, int y, int width, int height){
    StretchDIBits(deviceContext, 
        /* x, y, width, height,
        x, y, width, height, */ 
        0 ,0, width, height,
        0 ,0, buffer.width, buffer.height,
        buffer.bitmapMemory, &buffer.bitmapInfo,
        DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK mainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam) //WindowProc
{
    LRESULT result = 0;
    switch(message){
        case WM_SIZE:
        { // variables will propagate if you don't use brackets
            OutputDebugStringA("WM_SIZE\n");
        } break;
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            running = false;
        } break;
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            running = false;
        } break;
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window,&paint);
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            int width = paint.rcPaint.right - x;
            int height = paint.rcPaint.bottom - y;
            windowDimension dim = getWindowDimension(window);
            displayBuffer(backBuffer,deviceContext,x,y,dim.width,dim.height);
/*             local_persist DWORD operation = WHITENESS; // static -> want this variable to stick around (not a good idea)
            PatBlt(deviceContext,x,y,width,height,operation);
            if (operation == WHITENESS){
                operation = BLACKNESS;
            } else {
                operation = WHITENESS;
            } */
            EndPaint(window,&paint);
        } break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: 
        {
            UINT32 VKCode = wParam;
            bool wasDown = ((lParam & (1 << 30)) != 0);
            bool isDown = ((lParam & (1 << 31)) == 0);
            if(wasDown != isDown){
                if(VKCode == 'W'){

                } else if(VKCode == 'A') {

                } else if(VKCode == 'S') {

                } else if(VKCode == 'D') {

                } else if(VKCode == 'Q') {
    
                } else if(VKCode == 'E') {

                } else if(VKCode == VK_UP) {

                } else if(VKCode == VK_LEFT) {

                } else if(VKCode == VK_DOWN) {

                } else if(VKCode == VK_RIGHT) {

                } else if(VKCode == VK_ESCAPE) {
                    if(isDown){
                        OutputDebugStringA("isDown ");
                    } 
                    if(wasDown) {
                        OutputDebugStringA("wasDown");
                    }
                    OutputDebugStringA("\n");
                } else if( VKCode == VK_SPACE) {

                }
            }
            bool altKeyWasDown = ((lParam & (1 << 29)) != 0);
            if((VKCode == VK_F4) && altKeyWasDown){
                running = false;
            }
        } break;
        default:
        {
            //OutputDebugStringA("default\n");
            result = DefWindowProc(window,message,wParam,lParam);
        } break;
    }

    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    //MessageBoxA(0,"Hello","Hello",MB_OK|MB_ICONEXCLAMATION);
    //WNDCLASSA WindowClass = {}; Initializes everything to zero. 
    WNDCLASSA windowClass = {};
    resizeDIBSection(backBuffer,600,360);

    windowClass.style = CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = mainWindowCallback;
    windowClass.hInstance = hInstance; // can use GetModuleHandle
    windowClass.lpszClassName = "HandmaidsTaleWindowClass";
    if(RegisterClassA(&windowClass)){
        HWND windowHandle = CreateWindowExA(
            0, windowClass.lpszClassName,
            "HandmaidsTale",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0, 0, hInstance, 0);
        if(windowHandle){
            // need to take messages off the queue -> loop through messages
            running = true;
            float xOffset = 0.0;
            float yOffset = 0.0;
            while(running){
                MSG message;
                while(PeekMessage(&message,0,0,0,PM_REMOVE)){
                    if(message.message == WM_QUIT){
                        running = false;
                        break;
                    }
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }
                loadXInput();
                for (DWORD controllerIndex=0; controllerIndex< XUSER_MAX_COUNT; controllerIndex++ ){
                    XINPUT_STATE controllerState;
                    if(xInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS){
                        // controller is plugged in
                        XINPUT_GAMEPAD *pad = &controllerState.Gamepad;
                        bool up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                        bool down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                        bool left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                        bool right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                        bool start = (pad->wButtons & XINPUT_GAMEPAD_START);
                        bool back = (pad->wButtons & XINPUT_GAMEPAD_BACK);
                        bool leftShoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                        bool rightShoulder = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                        bool aButton = (pad->wButtons & XINPUT_GAMEPAD_A);
                        bool bButton = (pad->wButtons & XINPUT_GAMEPAD_B);
                        bool xButton = (pad->wButtons & XINPUT_GAMEPAD_X);
                        bool yButton = (pad->wButtons & XINPUT_GAMEPAD_Y);

                        INT16 stickX = pad->sThumbLX;
                        INT16 stickY = pad->sThumbLY;

                        if(aButton){
                            yOffset += 2;
                        }
                    } else {
                        // controller is not available
                    }
                }

                renderWeirdGradient(backBuffer, xOffset, yOffset);
                HDC deviceContext = GetDC(windowHandle);
                windowDimension dim = getWindowDimension(windowHandle);

                displayBuffer(backBuffer, deviceContext, 0, 0, dim.width, dim.height);
                ReleaseDC(windowHandle,deviceContext);
                xOffset+=0.2; yOffset+=0.2;
            }
        }
    }

    return 0;
}