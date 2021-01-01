/*-------------------------------------------------------------------*\
|  QMKRGBMatrixController.h                                           |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper         10th Octobber 2020                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "QMKRGBMatrixController.h"
#include <QDebug>

QMKRGBMatrixController::QMKRGBMatrixController(hid_device* dev_handle)
{
    dev = dev_handle;
    GetFirmware();
}

QMKRGBMatrixController::~QMKRGBMatrixController()
{
    hid_close(dev);
}

void QMKRGBMatrixController::SetModeAndSpeed(unsigned char mode, unsigned char speed)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    //unsigned char finalSpeed = mode == 1 || 2 || 3 ? 0 : speed;
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_SET_MODE_AND_SPEED;
    usb_buf[0x02]   = QMK_EEPROM_DONT_SAVE;
    usb_buf[0x03]   = mode;
    usb_buf[0x04]   = speed;

    qDebug() << QMK_EEPROM_DONT_SAVE << QMK_RGBMATRIX_SET_MODE_AND_SPEED << mode;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::SetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_SET_SINGLE_LED;
    usb_buf[0x02]   = led;
    usb_buf[0x03]   = red;
    usb_buf[0x04]   = green;
    usb_buf[0x05]   = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::SetLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{ 
    unsigned int leds_sent = 0;

    while(leds_sent < num_colors)
    {
        unsigned int leds_to_send = 5;

        if((num_colors - leds_sent) < leds_to_send)
        {
            leds_to_send = num_colors - leds_sent;
        }

        unsigned char usb_buf[65];
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_RGBMATRIX_SET_LEDS;
        usb_buf[0x02] = channel == 1 ? pid == 0 ? leds_sent + 67 : leds_sent + 87 : leds_sent;
        usb_buf[0x03] = leds_to_send;

        //qDebug() << leds_to_send << leds_sent;

        for(unsigned int led_idx = 0; led_idx < leds_to_send; led_idx++)
        {
            /*qDebug() << led_idx + leds_sent << RGBGetRValue(colors[led_idx + leds_sent])
                    << RGBGetGValue(colors[led_idx + leds_sent])
                    << RGBGetBValue(colors[led_idx + leds_sent]);*/
            usb_buf[(led_idx * 3) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 65);

        leds_sent += leds_to_send;
    }
}

void QMKRGBMatrixController::GetFirmware()
{
    int  actual;
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_GET_FIRMWARE;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    actual = hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    pid = usb_buf[1];
}

int QMKRGBMatrixController::GetPID()
{
    return pid;
}
