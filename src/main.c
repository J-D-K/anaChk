#include <switch.h>

#include "gfx.h"

void resetAnaGfx(tex *ana)
{
    texClearColor(ana, clrCreateU32(0xFF000000));
    drawRect(ana, 0, 360, 640, 1, clrCreateU32(0xFFAAAAAA));
    drawRect(ana, 320, 32, 1, 656, clrCreateU32(0xFFAAAAAA));
}

int main(void)
{
    //Init system stuff
    hidInitialize();
    graphicsInit(1280, 720);
    font *sysFont = fontLoadSharedFonts();

    //Setup buffers for left and right stick
    tex *leftStickBuff = texCreate(640, 720), *rightStickBuff = texCreate(640, 720);
    resetAnaGfx(leftStickBuff);
    resetAnaGfx(rightStickBuff);

    //vars for min/max x & y
    int lXMin = 0, lXMax = 0, lYMin = 0, lYMax = 0;
    int rXMin = 0, rXMax = 0, rYMin = 0, rYMax = 0;

    //Hid for stick data
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    while(appletMainLoop())
    {
        padUpdate(&pad);
        HidAnalogStickState left = padGetStickPos(&pad, 0);
        HidAnalogStickState right = padGetStickPos(&pad, 1);
        uint64_t down = padGetButtonsDown(&pad);

        if(down & HidNpadButton_B)
        {
            resetAnaGfx(leftStickBuff);
            resetAnaGfx(rightStickBuff);
            lXMin = 0, lXMax = 0, lYMin = 0, lYMax = 0;
            rXMin = 0, rXMax = 0, rYMin = 0, rYMax = 0;
        }
        else if(down & HidNpadButton_Plus)
            break;

        //Update min/max
        if(left.x < lXMin)
            lXMin = left.x;
        else if(left.x > lXMax)
            lXMax = left.x;

        if(left.y < lYMin)
            lYMin = left.y;
        else if(left.y > lYMax)
            lYMax = left.y;

        if(right.x < rXMin)
            rXMin = right.x;
        else if(right.x > rXMax)
            rXMax = right.x;

        if(right.y < rYMin)
            rYMin = right.y;
        else if(right.y > rYMax)
            rYMax = right.y;


        gfxBeginFrame();
        texClearColor(frameBuffer, clrCreateU32(0xFF000000));

        //Left
        drawRect(leftStickBuff, 319 + (left.x / 128), 359 - (left.y / 128), 3, 3, clrCreateU32(0xFFFF0000));
        texDrawNoAlpha(leftStickBuff, frameBuffer, 0, 0);
        drawTextf(frameBuffer, sysFont, 8, 8, 12, clrCreateU32(0xFFFFFFFF), "X: %i\n  xMin: %i\n  xMax: %i\n\nY: %i\n  yMin: %i\n  yMax: %i", left.x, lXMin, lXMax, left.y, lYMin, lYMax);

        //Right
        drawRect(rightStickBuff, 319 + (right.x / 128), 359 - (right.y / 128), 3, 3, clrCreateU32(0xFF0000FF));
        texDrawNoAlpha(rightStickBuff, frameBuffer, 640, 0);
        drawTextf(frameBuffer, sysFont, 648, 8, 12, clrCreateU32(0xFFFFFFFF), "X: %i\n  xMin: %i\n  xMax: %i\n\nY: %i\n  yMin: %i\n  yMax: %i", right.x, rXMin, rXMax, right.y, rYMin, rYMax);
        gfxEndFrame();
    }

    fontDestroy(sysFont);
    texDestroy(leftStickBuff);
    texDestroy(rightStickBuff);
    graphicsExit();
    hidExit();
}
