/*-------------------------------------------------------------------*\
|  QMKOpenRGBControllerDetect.cpp                                     |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol                    |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include "Detector.h"
#include "QMKOpenRGBRev9Controller.h"
#include "QMKOpenRGBRevBController.h"
#include "RGBController.h"
#include "RGBController_QMKOpenRGBRev9.h"
#include "RGBController_QMKOpenRGBRevB.h"
#include "LogManager.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Protocol version                                      |
\*-----------------------------------------------------*/
#define QMK_OPENRGB_PROTOCOL_VERSION_9          0x09
#define QMK_OPENRGB_PROTOCOL_VERSION_B          0x0B
#define QMK_OPENRGB_PROTOCOL_VERSION_C          0x0C

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

unsigned int GetProtocolVersion(hid_device *dev)
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_PROTOCOL_VERSION;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    return usb_buf[1];
}

void DetectQMKOpenRGBControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        /*-----------------------------------------------------*\
        | Use Rev9 controller for getting protocol version.     |
        | Protocol version request may not change across        |
        | protocol versions                                     |
        \*-----------------------------------------------------*/
        unsigned int version                                = GetProtocolVersion(dev);

        switch(version)
        {
            case QMK_OPENRGB_PROTOCOL_VERSION_9:
                {
                QMKOpenRGBRev9Controller*     controller     = new QMKOpenRGBRev9Controller(dev, info->path);
                RGBController_QMKOpenRGBRev9* rgb_controller = new RGBController_QMKOpenRGBRev9(controller);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            case QMK_OPENRGB_PROTOCOL_VERSION_B:
                {
                QMKOpenRGBRevBController*     controller     = new QMKOpenRGBRevBController(dev, info->path);
                RGBController_QMKOpenRGBRevB* rgb_controller = new RGBController_QMKOpenRGBRevB(controller, false);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            case QMK_OPENRGB_PROTOCOL_VERSION_C:
                {
                QMKOpenRGBRevBController*     controller     = new QMKOpenRGBRevBController(dev, info->path);
                RGBController_QMKOpenRGBRevB* rgb_controller = new RGBController_QMKOpenRGBRevB(controller, true);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            default:
                LOG_INFO("[QMK OpenRGB] Detection failed - the detected keyboard is using an outdated version %i protocol. Please update to to the lastest version!", version);
        }
    }
}

void RegisterQMKDetectors()
{
    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    json qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");

    if(qmk_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < qmk_settings["devices"].size(); device_idx++)
        {
            if( qmk_settings["devices"][device_idx].contains("usb_pid")
             && qmk_settings["devices"][device_idx].contains("usb_vid")
             && qmk_settings["devices"][device_idx].contains("name"))
            {
                std::string usb_pid_str = qmk_settings["devices"][device_idx]["usb_pid"];
                std::string usb_vid_str = qmk_settings["devices"][device_idx]["usb_vid"];
                std::string name        = qmk_settings["devices"][device_idx]["name"];

                /*-------------------------------------*\
                | Parse hex string to integer           |
                \*-------------------------------------*/
                unsigned short usb_pid  = std::stoi(usb_pid_str, 0, 16);
                unsigned short usb_vid  = std::stoi(usb_vid_str, 0, 16);

                REGISTER_DYNAMIC_HID_DETECTOR_IPU(name, DetectQMKOpenRGBControllers, usb_vid, usb_pid, 1, QMK_USAGE_PAGE, QMK_USAGE);
            }
        }
    }
}

REGISTER_DYNAMIC_DETECTOR("QMK OpenRGB Devices", RegisterQMKDetectors);
