#include "Game.hpp"
#include "constants.hpp"


#if WIN32
#include <windows.h>
#else
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

using namespace cppdungeon;

int main()
{
	u32 width = 0;
	u32 height = 0;
#if WIN32
    width = (int)GetSystemMetrics(SM_CXSCREEN);
    height = (int)GetSystemMetrics(SM_CYSCREEN);
#else
    auto mainDisplayId = CGMainDisplayID();
    width = CGDisplayPixelsWide(mainDisplayId);
    height = CGDisplayPixelsHigh(mainDisplayId);
#endif

	Game demo;
	if (demo.Construct(width / 4, height / 4, 4, 4))
		demo.Start();

	return 0;
}