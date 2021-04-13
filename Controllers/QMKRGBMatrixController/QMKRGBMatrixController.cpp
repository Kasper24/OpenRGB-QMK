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

QMKRGBMatrixController::QMKRGBMatrixController(hid_device *dev_handle, const char *path)
{
    dev = dev_handle;
    location = path;

    GetDeviceInfo();
    GetModeInfo();
    GetLEDMatrixInfo();
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

unsigned int QMKRGBMatrixController::GetZonesCount()
{
    return zones_count;
}

std::string QMKRGBMatrixController::GetCurrentZoneName()
{
    return current_zone_name;
}

unsigned int QMKRGBMatrixController::GetCurrentZoneType()
{
    return current_zone_type;
}

unsigned int QMKRGBMatrixController::GetCurrentZoneSize()
{
    return current_zone_size;
}

unsigned int QMKRGBMatrixController::GetLEDMatirxRows()
{
    return led_matrix_rows;
}

unsigned int QMKRGBMatrixController::GetLEDMatirxColumns()
{
    return led_matrix_columns;
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

    zones_count = usb_buf[QMK_RGBMATRIX_ZONE_COUNT_BYTE];

    int i = QMK_RGBMATRIX_ZONE_COUNT_BYTE + 1;
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

void QMKRGBMatrixController::GetZoneInfo(unsigned int zone)
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
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ZONE_INFO;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    current_zone_type = usb_buf[QMK_RGBMATRIX_ZONE_TYPE_BYTE];
    current_zone_size = usb_buf[QMK_RGBMATRIX_ZONE_SIZE_BYTE];

    int i = QMK_RGBMATRIX_ZONE_SIZE_BYTE + 1;
    while (usb_buf[i] != 0)
    {
        current_zone_name.push_back(usb_buf[i]);
        i++;
    }
}

void QMKRGBMatrixController::GetLEDMatrixInfo()
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
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_MATRIX_INFO;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    led_matrix_columns = usb_buf[QMK_RGBMATRIX_LED_MATRIX_COLUMNS_COUNT_BYTE];
    led_matrix_rows = usb_buf[QMK_RGBMATRIX_LED_MATRIX_ROWS_COUNT_BYTE];
}

unsigned int QMKRGBMatrixController::GetLEDValueInMatrix(unsigned int column, unsigned int row)
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
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_VALUE_IN_MATRIX;
    usb_buf[0x02] = column;
    usb_buf[0x03] = row;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetLEDName(unsigned int led_column, unsigned int led_row)
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
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_NAME;
    usb_buf[0x02] = led_column;
    usb_buf[0x03] = led_row;

    hid_write(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_RGBMATRIX_PACKET_SIZE);

    return usb_buf[1];
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
