/*-----------------------------------------------------*\
| DeviceView.cpp                                        |
|                                                       |
|  OpenRGB Device view widget for Qt                    |
|                                                       |
| Adam Honse (calcprogrammer1@gmail.com)                |
\*-----------------------------------------------------*/

#include "DeviceView.h"
#include "RGBController.h"
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QMouseEvent>

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    initSize(128,128),
    mouseDown(false)
{
    controller = NULL;
    setMouseTracking(1);
}

void DeviceView::setController(RGBController * controller_ptr)
{
    controller = controller_ptr;
    selectionFlags.resize(controller->leds.size());
}

QSize DeviceView::sizeHint () const
{
    return QSize(height(),height());
}

QSize DeviceView::minimumSizeHint () const
{
    return initSize;
}

void DeviceView::mousePressEvent(QMouseEvent *event)
{
    ctrlDown = event->modifiers().testFlag(Qt::ControlModifier);
    mouseDown = true;
    mouseMoved = false;
    if(ctrlDown)
    {
        previousFlags = selectionFlags;
        previousSelection = selectedLeds;
    }
    // It's okay if the size becomes negative
    selectionRect.setLeft(event->x());
    selectionRect.setTop(event->y());
    selectionRect.setRight(event->x());
    selectionRect.setBottom(event->y());
    updateSelection();
    update();
}

void DeviceView::mouseMoveEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
    selectionRect.setRight(event->x());
    selectionRect.setBottom(event->y());
    if(mouseDown)
    {
        mouseMoved = true;
        ctrlDown = event->modifiers().testFlag(Qt::ControlModifier);
        // Clear the previous selection in case ctrl is released
        if(!ctrlDown)
        {
            previousSelection.clear();
            previousFlags.clear();
            previousFlags.resize(controller->leds.size());
        }
        updateSelection();
    }
    update();
}

void DeviceView::mouseReleaseEvent(QMouseEvent* event)
{
    selectionRect = selectionRect.normalized();
    mouseDown = false;

    if(!mouseMoved) // Check if the user clicked a zone name & select it
    {
        int size = width();
        int offset_x = 0;
        if(height() < size * controller->matrix_h)
        {
            size = height() / controller->matrix_h;
            offset_x = (width() - size) / 2;
        }
        for(int zone_idx = 0; zone_idx < controller->zones.size(); ++zone_idx)
        {
            int posx = controller->zones[zone_idx].matrix_x * size + offset_x;
            int posy = controller->zones[zone_idx].matrix_y * size;
            int posw = controller->zones[zone_idx].matrix_w * size;
            int posh = controller->zones[zone_idx].matrix_h * size;
            QRect rect = {posx, posy, posw, posh};
            if(rect.contains(event->pos()))
            {
                selectZone(zone_idx, ctrlDown);
            }
        }
    }
    update();
}

void DeviceView::resizeEvent(QResizeEvent *event)
{
    unsigned int size = 0;

    if(event->size().width() < event->size().height())
    {
        size = event->size().width();
    }
    else
    {
        size = event->size().height();
    }

    update();
}

void DeviceView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont font = painter.font();
    //QStyleOption opt;
    //opt.initFrom(this);

    // Figure out the real width of the whole thing
    int size = width();
    int offset_x = 0;
    if(height() < size * controller->matrix_h)
    {
        size = height() / controller->matrix_h;
        offset_x = (width() - size) / 2;
    }

    // Led rectangles
    for(int led_idx = 0; led_idx < controller->leds.size(); ++led_idx)
    {
        int posx = controller->leds[led_idx].matrix_x * size + offset_x;
        int posy = controller->leds[led_idx].matrix_y * size;
        int posw = controller->leds[led_idx].matrix_w * size;
        int posh = controller->leds[led_idx].matrix_h * size;
        //qDebug() << "Test LED: " << width() << height() << controller->matrix_h;
        QRect rect = {posx, posy, posw, posh};
        // Fill color
        QColor currentColor = QColor::fromRgb(
                    RGBGetRValue(controller->colors[led_idx]),
                    RGBGetGValue(controller->colors[led_idx]),
                    RGBGetBValue(controller->colors[led_idx]));
        painter.setBrush(currentColor);
        // Border color
        if(selectionFlags[led_idx])
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().dark().color());
        }
        painter.drawRect(rect);
        font.setPixelSize(posh / 2);
        // Set the font color so that the text is visible
        if(currentColor.value() > 127)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::white);
        }
        painter.drawText(rect, Qt::AlignVCenter | Qt::AlignHCenter, QString(controller->leds[led_idx].label.c_str()));
    }

    // Zone names
    for(int zone_idx = 0; zone_idx < controller->zones.size(); ++zone_idx)
    {
        int posx = controller->zones[zone_idx].matrix_x * size + offset_x;
        int posy = controller->zones[zone_idx].matrix_y * size;
        int posw = controller->zones[zone_idx].matrix_w * size;
        int posh = controller->zones[zone_idx].matrix_h * size;
        QRect rect = {posx, posy, posw, posh};
        font.setPixelSize(posh / 2);
        if(rect.contains(lastMousePos))
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().windowText().color());
        }
        painter.drawText(rect, Qt::AlignVCenter | Qt::AlignHCenter, QString(controller->zones[zone_idx].name.c_str()));
    }

    // Selection area
    if(mouseDown)
    {
        QRect rect = selectionRect.normalized();
        QColor color = palette().highlight().color();
        color.setAlpha(63);
        painter.fillRect(rect, color);
        color.setAlpha(127);
        painter.setBrush(color);
        painter.drawRect(rect);
    }
}

void DeviceView::updateSelection()
{
    selectedLeds.clear();
    selectionFlags.resize(controller->leds.size()); // Could this change?
    QRect sel = selectionRect.normalized();
    std::vector<led>& leds = controller->leds;

    int size = width();
    int offset_x = 0;
    if(height() < size * controller->matrix_h)
    {
        size = height() / controller->matrix_h;
        offset_x = (width() - size) / 2;
    }

    for(int led_idx = 0; led_idx < leds.size(); ++led_idx)
    {
        // Check intersection
        int posx = controller->leds[led_idx].matrix_x * size + offset_x;
        int posy = controller->leds[led_idx].matrix_y * size;
        int posw = controller->leds[led_idx].matrix_w * size;
        int posh = controller->leds[led_idx].matrix_h * size;
        QRect rect = {posx, posy, posw, posh};
        selectionFlags[led_idx] = 0;
        if(sel.intersects(rect))
        {
            selectedLeds.push_back(led_idx);
            selectionFlags[led_idx] = 1;
        }
        if(ctrlDown)
        {
            selectionFlags[led_idx] ^= previousFlags[led_idx];
        }
    }
    emit selectionChanged(selectedLeds);
}

bool DeviceView::selectLed(int target)
{
    if(target < 0 || target >= controller->leds.size())
    {
        return false;
    }
    selectedLeds.resize(1);
    selectedLeds[0] = target;
    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());
    selectionFlags[target] = 1;
    emit selectionChanged(selectedLeds);
    return true;
}

bool DeviceView::selectLeds(QVector<int> target)
{
    for(int item: target)
    {
        if(item < 0 || item >= controller->leds.size())
        {
            return false;
        }
    }
    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());
    for(int item: target)
    {
        selectionFlags[item] = 1;
    }
    //selectedLeds = target;
    // Filter out duplicate items
    selectedLeds.clear();
    for(int i = 0; i < selectionFlags.size(); ++i)
    {
        if(selectionFlags[i])
        {
            selectedLeds.push_back(i);
        }
    }
    emit selectionChanged(selectedLeds);
    return true;
}

bool DeviceView::selectZone(int zone, bool add)
{
    if(zone < 0 || zone >= controller->zones.size())
    {
        return false;
    }
    if(!add)
    {
        selectedLeds.clear();
        selectionFlags.clear();
        selectionFlags.resize(controller->leds.size());
    }
    int zoneStart = controller->zones[zone].leds - &(controller->leds[0]); // Index of the first LED in the zone
    for(int led_idx = 0; led_idx < controller->zones[zone].leds_count; ++led_idx)
    {
        if(!selectionFlags[zoneStart + led_idx])
        {
            selectedLeds.push_back(zoneStart + led_idx);
            selectionFlags[zoneStart + led_idx] = 1;
        }
    }
    emit selectionChanged(selectedLeds);
    return true;
}

void DeviceView::clearSelection() // Same as selecting the entire device
{
    selectedLeds.clear();
    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());
}

void DeviceView::setSelectionColor(RGBColor color)
{
    if(selectedLeds.isEmpty())
    {
        controller->SetAllLEDs(color);
    }
    else
    {
        for(int led_idx: selectedLeds)
        {
            controller->SetLED(led_idx, color);
        }
    }
    controller->UpdateLEDs();
}
