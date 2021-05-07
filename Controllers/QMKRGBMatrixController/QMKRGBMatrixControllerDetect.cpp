/*-------------------------------------------------------------------*\
|  QMKRGBMatrixControllerDetect.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "QMKRGBMatrixController.h"
#include "RGBController.h"
#include "RGBController_QMKRGBMatrix.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Protocol version                                      |
\*-----------------------------------------------------*/
#define QMK_RGBMATRIX_PROTOCOL_VERSION          0x06

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

/*-----------------------------------------------------*\
| Massdrop                                              |
\*-----------------------------------------------------*/
#define MASSDROP_VID                            0x04D8
#define MASSDROP_ALT_PID                        0xEED3
#define MASSDROP_CTRL_PID                       0xEED2

/*-----------------------------------------------------*\
| Redragon                                              |
\*-----------------------------------------------------*/
#define REDRAGON_VID                            0x0C45
#define REDRAGON_K556_PID                       0x5004

/*-----------------------------------------------------*\
| KBDFans                                              |
\*-----------------------------------------------------*/
#define KBDFANS_VID                             0x4B42
#define KBDFANS_K67_MKII_RGB_PID                0x1225

void DetectQMKRGBMatrixControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);
    if (dev)
    {
        QMKRGBMatrixController *controller = new QMKRGBMatrixController(dev, info->path);
        unsigned int version = controller->GetProtocolVersion();
        if (version == QMK_RGBMATRIX_PROTOCOL_VERSION)
        {
            RGBController_QMKRGBMatrix *rgb_controller = new RGBController_QMKRGBMatrix(controller, version);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

REGISTER_HID_DETECTOR_IPU("Massdrop Alt", DetectQMKRGBMatrixControllers, MASSDROP_VID, MASSDROP_ALT_PID, 1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("Massdrop Ctrl", DetectQMKRGBMatrixControllers, MASSDROP_VID, MASSDROP_CTRL_PID, 1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("Redragon K556", DetectQMKRGBMatrixControllers, REDRAGON_VID, REDRAGON_K556_PID, 1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("KBDFans KBD67 MKII RGB", DetectQMKRGBMatrixControllers, KBDFANS_VID, KBDFANS_K67_MKII_RGB_PID, 1, QMK_USAGE_PAGE, QMK_USAGE);