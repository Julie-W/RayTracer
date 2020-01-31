#include <windows.h>
#include <xinput.h>
#include <dsound.h>
#include <functional>
#include <iostream>
#include <thread>
#include "scene.hpp"
#include "viewPlane.hpp"

#define internal static
#define local_persist static
#define global_variable static

global_variable bool running;
global_variable Scene scene;

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

windowDimension getWindowDimension(HWND window){
    windowDimension result;
    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = clientRect.right - clientRect.left;
    result.height = clientRect.bottom - clientRect.top;

    return result;
}

void render(offscreenBuffer &buffer){
    ViewPlane viewPlane = ViewPlane(1, buffer.width, buffer.height, 90, 90);
    UINT8 *row = (UINT8 *)buffer.bitmapMemory;
    int offset = 0;
    for(int y = 0; y < buffer.height; y++){
        UINT32 *pixel = (UINT32 *)row;
        for(int x = 0; x < buffer.width; x++){
            Vector3f color = viewPlane.getPixelColor(x,y,scene);
            // Pixel in memory: bb gg rr xx
            UINT8 blue = color[2];
            UINT8 green = color[1];
            UINT8 red = color[0];
            *pixel = (red << 16) | (green << 8) | blue;
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
    render(buffer);
}

void displayBuffer(offscreenBuffer &buffer, HDC deviceContext, int x, int y, int width, int height){
    StretchDIBits(deviceContext, 
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
            EndPaint(window,&paint);
        } break;
        default:
        {
            //OutputDebugStringA("default\n");
            result = DefWindowProc(window,message,wParam,lParam);
        } break;
    }

    return result;
}

void initializeScene() {
    Sphere sphere = Sphere(1,Vector3f(0,0,-10));
    Light light = Light(Vector3f(0,0,0));
    scene.addObject(sphere);
    scene.addLight(light);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    initializeScene();
    WNDCLASSA windowClass = {};
    resizeDIBSection(backBuffer,192,108);

    windowClass.style = CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = mainWindowCallback;
    windowClass.hInstance = hInstance; // can use GetModuleHandle
    windowClass.lpszClassName = "HandmaidsTaleWindowClass";
    if(RegisterClassA(&windowClass)){
        HWND windowHandle = CreateWindowExA(
            0, windowClass.lpszClassName,
            "HandmaidsTale",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_MAXIMIZE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0, 0, hInstance, 0);
        if(windowHandle){
            // need to take messages off the queue -> loop through messages
            running = true;
            render(backBuffer);
            while(running){
                MSG message;
                std::this_thread::sleep_for (std::chrono::milliseconds(100));
                BOOL messageResult = GetMessage(&message,0,0,0);
                if(messageResult > 0){
                    std::cout << "print" << std::endl;
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                } else {
                    break; // windows cleans up
                }
                HDC deviceContext = GetDC(windowHandle);
                windowDimension dim = getWindowDimension(windowHandle);
                displayBuffer(backBuffer, deviceContext, 0, 0, dim.width, dim.height);
                ReleaseDC(windowHandle,deviceContext);
            }
        }
    }

    return 0;
}