#ifndef UNITRISWRAPPER_H
#define UNITRISWRAPPER_H

#include <QObject>
#include <QTimer>

#include "render_lcd_mono.h"

class UnitrisWrapper : public QObject
{
    Q_OBJECT

public:
    UnitrisWrapper();

    Q_INVOKABLE void start();
    Q_INVOKABLE QString tick();

private:

    render_lcd_mono_t mCtx;
};

#endif // UNITRISWRAPPER_H
