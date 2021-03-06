
#include <random>
#include <cstdint>
#include <ctime>
#include <iostream>

#include <QImage>
#include <QBuffer>
#include <QPainter>

#include "UnitrisWrapper.h"
#include "bmplib.h"
#include "debug.h"

UnitrisWrapper::UnitrisWrapper()
{
    // initilize seed
    std::srand(static_cast<unsigned int>( time(nullptr)));
}

void UnitrisWrapper::start()
{
    LCD_Initialize(&mCtx);
}

QString UnitrisWrapper::tick(unsigned long key_event)
{
    LCD_Tick(&mCtx, key_event);
    LCD_RenderBitmap(&mCtx);
    DBG_BufferToFile(mCtx.bitmap, sizeof(mCtx.bitmap), "tetris_screen.bmp");

    QByteArray bytes(mCtx.bitmap, sizeof(mCtx.bitmap));
    QString imgBase64 = "data:image/bmp;base64," + QString::fromLatin1(bytes.toBase64().data());

    return imgBase64;
}


