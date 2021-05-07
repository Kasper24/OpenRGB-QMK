/*-------------------------------------------------------------------*\
|  QMKRGBMatrixController.cpp                                         |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper         10th Octobber 2020                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "QMKRGBMatrixController.h"
#include <cstring>
#include <map>
#include <QDebug>

static std::map<uint8_t, std::string> QMKKeycodeToKeynameMap
{
    { 0, "" }, { 1, "Right Fn" }, { 2, "" }, { 3, "" },
    { 4, "A" }, { 5, "B" }, { 6, "C" }, { 7, "D" },
    { 8, "E" }, { 9, "F" }, { 10, "G" }, { 11, "H" },
    { 12, "I" }, { 13, "J" }, { 14, "K" }, { 15, "L" },
    { 16, "M" }, { 17, "N" }, { 18, "O" }, { 19, "P" },
    { 20, "Q" }, { 21, "R" }, { 22, "S" }, { 23, "T" },
    { 24, "U" }, { 25, "V" }, { 26, "W" }, { 27, "X" },
    { 28, "Y" }, { 29, "Z" }, { 30, "1" }, { 31, "2" },
    { 32, "3" }, { 33, "4" }, { 34, "5" },{ 35, "6" },
    { 36, "7" }, { 37, "8" }, { 38, "9" }, { 39, "0" },
    { 40, "Enter" }, { 41, "Escape" }, { 42, "Backspace" }, { 43, "Tab" },
    { 44, "Space" }, { 45, "-" }, { 46, "=" }, { 47, "[" },
    { 48, "]" }, { 49, "\\ (ANSI)" }, { 50, "" }, { 51, ";" },
    { 52, "'" }, { 53, "`" }, { 54, "," }, { 55, "." },
    { 56, "/" }, { 57, "Caps Lock" }, { 58, "F1" }, { 59, "F2" },
    { 60, "F3" }, { 61, "F4" }, { 62, "F5" }, { 63, "F6" },
    { 64, "F7" }, { 65, "F8" }, { 66, "F9" }, { 67, "F10" },
    { 68, "F11" }, { 69, "F12" }, { 70, "Print Screen" }, { 71, "Scroll Lock" },
    { 72, "Pause/Break" }, { 73, "Insert" }, { 74, "Home" }, { 75, "Page Up" },
    { 76, "Delete" }, { 77, "End" }, { 78, "Page Down" }, { 79, "Right Arrow" },
    { 80, "Left Arrow" }, { 81, "Down Arrow" }, { 82, "Up Arrow" }, { 83, "Num Lock" },
    { 84, "Number Pad /" }, { 85, "Number Pad *" }, { 86, "Number Pad -" }, { 87, "Number Pad +" },
    { 88, "Number Pad Enter" }, { 89, "Number Pad 1" }, { 90, "Number Pad 2" }, { 91, "Number Pad 3" },
    { 92, "Number Pad 4" }, { 93, "Number Pad 5" }, { 94, "Number Pad 6" }, { 95, "Number Pad 7" },
    { 96, "Number Pad 8" }, { 97, "Number Pad 9" }, { 98, "Number Pad 0" }, { 99, "Number Pad ." },
    { 100, "" }, { 101, "Menu" }, { 102, "" }, { 103, "" },
    { 104, "" }, { 105, "" }, { 106, "" }, { 107, "" },
    { 108, "" }, { 109, "" }, { 110, "" }, { 111, "" },
    { 112, "" }, { 113, "" }, { 114, "" }, { 115, "" },
    { 116, "" }, { 117, "" }, { 118, "" }, { 119, "" },
    { 120, "" }, { 121, "" }, { 122, "" }, { 123, "" },
    { 124, "" }, { 125, "" }, { 126, "" }, { 127, "" },
    { 128, "" }, { 129, "" }, { 130, "" }, { 131, "" },
    { 132, "" }, { 133, "" }, { 134, "" }, { 135, "" },
    { 136, "" }, { 137, "" }, { 138, "" }, { 139, "" },
    { 140, "" }, { 141, "" }, { 142, "" }, { 143, "" },
    { 144, "" }, { 145, "" }, { 146, "" }, { 147, "" },
    { 148, "" }, { 149, "" }, { 150, "" }, { 151, "" },
    { 152, "" }, { 153, "" }, { 154, "" }, { 155, "" },
    { 156, "" }, { 157, "" }, { 158, "" }, { 159, "" },
    { 160, "" }, { 161, "" }, { 162, "" }, { 163, "" }, { 164, "" },
    /*Space Cadet Left Shift*/ { 216, "Left Shift"}, /*Space Cadet Right Shift*/{ 217, "Right Shift"},
    { 224, "Left Control" }, { 225, "Left Shift" }, { 226, "Left Alt" }, { 227, "Left Windows" },
    { 228, "Right Control" }, { 229, "Right Shift" }, { 230, "Right Alt" }, { 231, "Right Windows" },
};

QMKRGBMatrixController::QMKRGBMatrixController(hid_device *dev_handle, const char *path)
{
    dev = dev_handle;
    location = path;

    GetDeviceInfo();
    GetModeInfo();
}

QMKRGBMatrixController::~QMKRGBMatrixController()
{
    hid_close(dev);
}

std::string QMKRGBMatrixController::GetLocation()
{
    return location;
}

std::string QMKRGBMatrixController::GetDeviceName()
{
    return device_name;
}

std::string QMKRGBMatrixController::GetDeviceVendor()
{
    return device_vendor;
}

unsigned int QMKRGBMatrixController::GetNumberOfKeyLEDs()
{
    return number_of_key_leds;
}

unsigned int QMKRGBMatrixController::GetNumberOfUnderglowLEDs()
{
    return number_of_underglow_leds;
}

unsigned int QMKRGBMatrixController::GetMode()
{
    return mode;
}

unsigned int QMKRGBMatrixController::GetModeSpeed()
{
    return mode_speed;
}

unsigned int QMKRGBMatrixController::GetModeColor()
{
    return mode_color;
}

std::vector<point_t> QMKRGBMatrixController::GetLEDPoints()
{
    return led_points;
}

std::vector<unsigned int> QMKRGBMatrixController::GetLEDFlags()
{
    return led_flags;
}

std::vector<std::string> QMKRGBMatrixController::GetLEDNames()
{
    return led_names;
}

unsigned int QMKRGBMatrixController::GetProtocolVersion()
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_PROTOCOL_VERSION;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    return usb_buf[1];
}

void QMKRGBMatrixController::GetDeviceInfo()
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_DEVICE_INFO;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    number_of_key_leds = usb_buf[QMK_RGBMATRIX_NUMBER_OF_KEY_LEDS_BYTE];
    number_of_underglow_leds = usb_buf[QMK_RGBMATRIX_NUMBER_OF_UNEDRGLOW_LEDS_BYTE];

    int i = QMK_RGBMATRIX_NUMBER_OF_UNEDRGLOW_LEDS_BYTE + 1;
    while (usb_buf[i] != 0)
    {
        device_name.push_back(usb_buf[i]);
        i++;
    }

    i++;
    while (usb_buf[i] != 0)
    {
        device_vendor.push_back(usb_buf[i]);
        i++;
    }
}

void QMKRGBMatrixController::GetModeInfo()
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_MODE_INFO;

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    mode = usb_buf[QMK_RGBMATRIX_MODE_BYTE];
    mode_speed = usb_buf[QMK_RGBMATRIX_SPEED_BYTE];

    // qmk hue range is between 0-255 so hue needs to be converted
    unsigned int oldRange = 255;
    unsigned int newRange = 359;
    unsigned int convertedHue = (usb_buf[QMK_RGBMATRIX_HUE_BYTE] * newRange / oldRange);

    hsv_t hsv;
    hsv.hue = convertedHue;
    hsv.saturation = usb_buf[QMK_RGBMATRIX_SATURATION_BYTE];
    hsv.value = usb_buf[QMK_RGBMATRIX_VALUE_BYTE];

    mode_color = hsv2rgb(&hsv);
}

void QMKRGBMatrixController::GetLEDInfo(unsigned int led)
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_INFO;
    usb_buf[0x02] = led;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    led_points.push_back(point_t{usb_buf[1], usb_buf[2]});
    led_flags.push_back(usb_buf[3]);

    if(usb_buf[3] & 2)
    {
        led_names.push_back("Underglow: " + std::to_string(led));
        return;
    }
    if(usb_buf[4] != 0)
    {
        qDebug() << led << usb_buf[4] << QMKKeycodeToKeynameMap[usb_buf[4]].data();
        led_names.push_back("Key: " + QMKKeycodeToKeynameMap[usb_buf[4]]);
    }
}

bool QMKRGBMatrixController::GetIsModeEnabled(unsigned int mode)
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_IS_MODE_ENABLED;
    usb_buf[0x02] = mode;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    return usb_buf[1] == QMK_RGBMATRIX_SUCCESS ? true : false;
}

RGBColor QMKRGBMatrixController::GetDirectModeLEDColor(unsigned int led)
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_DIRECT_MODE_LED_COLOR;
    usb_buf[0x02] = led;

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    return ToRGBColor(usb_buf[1], usb_buf[2], usb_buf[3]);
}

void QMKRGBMatrixController::SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed)
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_SET_MODE;
    usb_buf[0x02] = hsv_color.hue * 255 / 359;
    usb_buf[0x03] = hsv_color.saturation;
    usb_buf[0x04] = hsv_color.value;
    usb_buf[0x05] = mode;
    usb_buf[0x06] = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/

    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_DIRECT_MODE_SET_SINGLE_LED;
    usb_buf[0x02] = led;
    usb_buf[0x03] = red;
    usb_buf[0x04] = green;
    usb_buf[0x05] = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int leds_count)
{
    unsigned int leds_sent = 0;
    unsigned int leds_per_update = 20;

    while (leds_sent < leds_count)
    {
        if ((leds_count - leds_sent) < leds_per_update)
        {
            leds_per_update = leds_count - leds_sent;
        }

        unsigned char usb_buf[QMK_RGBMATRIX_PACKET_SIZE];

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, QMK_RGBMATRIX_PACKET_SIZE);

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_RGBMATRIX_DIRECT_MODE_SET_LEDS;
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = leds_per_update;

        for (unsigned int led_idx = 0; led_idx < leds_per_update; led_idx++)
        {
            usb_buf[(led_idx * 3) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 65);

        leds_sent += leds_per_update;
    }
}
