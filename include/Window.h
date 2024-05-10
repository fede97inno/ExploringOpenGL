#pragma once
#include <string>

typedef struct GLFWwindow GLDWwindow;   // Forward declaration
class Window
{
public:
    Window(int inWidth, int inHeight, const std::string& inTitle);
    ~Window();
    bool IsOpened() const;
    float DeltaTime() const;
    void SetTitle(const std::string& inTitle);
    void Update();
    void PrintInfo() const;
private:
    GLFWwindow* rawWindow;
    float deltaTime;
};