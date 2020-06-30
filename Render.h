#include <vector>
#include <Windows.h>
class Render
{
public:
    Render();
    ~Render();

    void Init(HWND HWnd);
    void Update();
    void Exit();

    //typedef std::vector<std::vector<uint32_t>> FrameBuffer;
private:
    static const int ViewWidth = 800;
    static const int ViewHeight = 600;

    HWND DrawWnd = nullptr;
    HDC ScreenDC = nullptr;
    HDC BufferDC = nullptr;
    HBITMAP BitMap = nullptr;

    uint32_t* ColorBuffer = nullptr;
};