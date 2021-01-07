#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>

//Obsługuję tylko szerokości na wschód od Londynu
#define MAX_LONGITUDE 180 // X = [0,180]
// Obsługuję tylko północną półkulę
#define MAX_LATITUDE 90 // Y = [0, 90]

void Errors(const char *comment); // Prints  Comment and error
class Window
{
public:
    GLFWwindow *window = nullptr;
    Window(){};
    Window(int _wd, int _ht, const char *name, int fullscr = 0, int vers = 33)
    {
        Init(_wd, _ht, name, vers, fullscr);
    }; // fullscr=1/2 vers=33
    void Init(int _wd, int _ht, const char *name, int fullscr = 0, int vers = 33);
    // ~AGLWindow();
    GLFWwindow *win() { return window; };

    int wd, ht;   // Window dimensions
    float aspect; // Window aspect ration

    float Viewport(int _vtx, int _vty, int _vwd, int _vht);    //return aspect ratio
    float ViewportOne(int _vtx, int _vty, int _vwd, int _vht); //store vp, return aspect
    void GetCursorNDC(float *x, float *y);                     // mouse position VpOne

    // Handles key 'f' for switching fullscreen mode:
    virtual void Resize(int _wd, int _ht); // Sets Win:wd,ht,aspec +glViewp
    virtual void KeyCB(int key, int scancode, int action, int mods);

    virtual void MouseButtonCB(int button, int action, int mods);
    virtual void MousePosCB(double xp, double yp);

    void MainLoop(void);
    void ReadData(void);
    std::vector<short> ReadFile(std::string filepath);

    void WaitForFixedFPS(float frame_time = 1. / 60.); // Use before glfwSwapBuffers()

private:
    bool IsFullScreen(void);
    void SetFullScreen(bool fullscreen);

    int vtx, vty, vwd, vht; // GtCursorNDC() and ViewportOne()
    GLFWmonitor *monitor = nullptr;
    std::array<int, 2> winPos = {0, 0};
    std::array<int, 2> winSize = {0, 0};

    std::array<int, 2> vwpSize = {0, 0};
    bool updateViewport = true;
    std::array<GLFWmonitor *, 2> monitors = {nullptr, nullptr}; // first and last m.

    double prev_time = 0.0; // for fixed FPS

    std::vector<short> map_data[MAX_LONGITUDE][MAX_LATITUDE];

    static void CallbackResize(GLFWwindow *window, int cx, int cy);
    static void CallbackKey(GLFWwindow *window, int key, int scancode, int action, int mods);
    // static void CallbackMouseButton(GLFWwindow *window, int button, int action, int mods);
    // static void CallbackMousePos(GLFWwindow *window, double xp, double yp);
};
