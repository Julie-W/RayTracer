#include <windows.h>
#include <xinput.h>
#include <dsound.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include "scene.hpp"
#include "viewPlane.hpp"
#include "readfile.h"

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

windowDimension getWindowDimension(HWND window){
    windowDimension result;
    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = clientRect.right - clientRect.left;
    result.height = clientRect.bottom - clientRect.top;

    return result;
}

void displayBuffer(offscreenBuffer &buffer, HDC deviceContext, int x, int y, int width, int height){
    StretchDIBits(deviceContext, 
        0 ,0, width, height,
        0 ,0, buffer.width, buffer.height,
        buffer.bitmapMemory, &buffer.bitmapInfo,
        DIB_RGB_COLORS, SRCCOPY);
}

void renderBlock(offscreenBuffer &buffer, ViewPlane &viewPlane, int xBlock, int xPixelsPerBlock, int yBlock, int yPixelsPerBlock){
    int xStart = xBlock*xPixelsPerBlock;
    int yStart = yBlock*yPixelsPerBlock;
    for(int y = yStart; y < yStart + yPixelsPerBlock; y++){
        for(int x = xStart; x < xStart + xPixelsPerBlock; x++){
            UINT32 *pixel = (UINT32 *)(buffer.bitmapMemory) + y*buffer.width + x;
            Vector3f color = viewPlane.getPixelColor(x,y); 
            // Pixel in memory: bb gg rr xx
            UINT8 blue = color[2]*255 > 255? 255 : color[2]*255;
            UINT8 green = color[1]*255 > 255? 255 : color[1]*255;
            UINT8 red = color[0]*255 > 255? 255 : color[0]*255;
            *pixel = (red << 16) | (green << 8) | blue;
        }
    }
}

void render(offscreenBuffer &buffer, HWND windowHandle, const char* filename){
    ViewPlane viewPlane = ViewPlane(1, buffer.width, buffer.height, 90);
    Scene scene = Scene();
    readfile(filename, &scene);
    viewPlane.setScene(&scene);
    int xPixelsPerBlock = buffer.width / 4;
    int yPixelsPerBlock = buffer.height / 4;
    std::vector<std::thread> threads;
    for(int blockY = 0; blockY < 4; blockY++){
        for(int blockX = 0; blockX < 4; blockX++){
            //renderBlock(buffer,viewPlane,blockX,xPixelsPerBlock,blockY,yPixelsPerBlock);
            threads.push_back(std::thread(renderBlock,std::ref(buffer),std::ref(viewPlane),blockX,xPixelsPerBlock,blockY,yPixelsPerBlock));
        }
    }
    HDC deviceContext = GetDC(windowHandle);
    windowDimension dim = getWindowDimension(windowHandle);
    for (std::thread & t : threads){
        if (t.joinable()) {
            t.join();
            displayBuffer(backBuffer, deviceContext, 0, 0, dim.width, dim.height);
        }
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

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    std::string str = lpCmdLine;
    std::stringstream ss (str);
    std::string a;
    char filename[20]; ss >> a >> filename;
    std::cout << filename << std::endl;
    const char* fn = filename;
    WNDCLASSA windowClass = {};
    resizeDIBSection(backBuffer,960,540);//1920,1080);

    windowClass.style = CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = mainWindowCallback;
    windowClass.hInstance = hInstance; // can use GetModuleHandle
    windowClass.lpszClassName = "RayTracerWindowClass";
    if(RegisterClassA(&windowClass)){
        HWND windowHandle = CreateWindowExA(
            0, windowClass.lpszClassName,
            "RayTracer",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0, 0, hInstance, 0);
        if(windowHandle){
            // need to take messages off the queue -> loop through messages
            running = true;
            render(backBuffer,windowHandle, fn);
            while(running){
                MSG message;
                BOOL messageResult = GetMessage(&message,0,0,0);
                if(messageResult > 0){
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                } else {
                    break; // windows cleans up
                }
                HDC deviceContext = GetDC(windowHandle);
                windowDimension dim = getWindowDimension(windowHandle);
                displayBuffer(backBuffer, deviceContext, 0, 0, dim.width, dim.height);
                ReleaseDC(windowHandle,deviceContext);
                std::this_thread::sleep_for (std::chrono::milliseconds(100));
            }
        }
    }

    return 0;
}

int main() {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}