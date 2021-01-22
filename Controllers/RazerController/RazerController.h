/*-----------------------------------------*\
|  RazerController.h                        |
|                                           |
|  Definitions and types for Razer devices  |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/22/2021   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/*---------------------------------------------------------*\
| Struct packing macro for GCC and MSVC                     |
\*---------------------------------------------------------*/
#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

/*---------------------------------------------------------*\
| Razer Command IDs                                         |
\*---------------------------------------------------------*/
enum
{
    /*-----------------------------------------------------*\
    | Set Commands                                          |
    \*-----------------------------------------------------*/
    RAZER_COMMAND_ID_SET_LED_STATE              = 0x00,
    RAZER_COMMAND_ID_SET_DEVICE_MODE            = 0x04,

    /*-----------------------------------------------------*\
    | Get Commands                                          |
    \*-----------------------------------------------------*/
    RAZER_COMMAND_ID_GET_LED_STATE              = 0x80,
    RAZER_COMMAND_ID_GET_FIRMWARE_VERSION       = 0x81,
    RAZER_COMMAND_ID_GET_SERIAL_STRING          = 0x82,
    RAZER_COMMAND_ID_GET_DEVICE_MODE            = 0x84,
};

/*---------------------------------------------------------*\
| Razer Storage Flags                                       |
\*---------------------------------------------------------*/
enum
{
    RAZER_STORAGE_NO_SAVE                       = 0x00,
    RAZER_STORAGE_SAVE                          = 0x01,
};

/*---------------------------------------------------------*\
| Razer LED IDs                                             |
\*---------------------------------------------------------*/
enum
{
    RAZER_LED_ID_ZERO                           = 0x00,
    RAZER_LED_ID_SCROLL_WHEEL                   = 0x01,
    RAZER_LED_ID_BATTERY                        = 0x03,
    RAZER_LED_ID_LOGO                           = 0x04,
    RAZER_LED_ID_BACKLIGHT                      = 0x05,
    RAZER_LED_ID_MACRO                          = 0x07,
    RAZER_LED_ID_GAME                           = 0x08,
    RAZER_LED_ID_PROFILE_RED                    = 0x0C,
    RAZER_LED_ID_PROFILE_GREEN                  = 0x0D,
    RAZER_LED_ID_PROFILE_BLUE                   = 0x0E,
    RAZER_LED_ID_RIGHT_SIDE                     = 0x10,
    RAZER_LED_ID_LEFT_SIDE                      = 0x11,
};

/*---------------------------------------------------------*\
| Razer Report Type (taken from OpenRazer)                  |
\*---------------------------------------------------------*/
struct razer_rgb
{
    unsigned char r,g,b;
};

union transaction_id_union
{
    unsigned char id;
    struct transaction_parts
    {
        unsigned char device : 3;
        unsigned char id : 5;
    } parts;
};

union command_id_union
{
    unsigned char id;
    struct command_id_parts
    {
        unsigned char direction : 1;
        unsigned char id : 7;
    } parts;
};

PACK(typedef struct razer_report
{
    unsigned char               report_id;
    unsigned char               status;
    union transaction_id_union  transaction_id;
    unsigned short              remaining_packets;
    unsigned char               protocol_type;
    unsigned char               data_size;
    unsigned char               command_class;
    union command_id_union      command_id;
    unsigned char               arguments[80];
    unsigned char               crc;
    unsigned char               reserved;
};)

class RazerController
{
public:
    RazerController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~RazerController();

    unsigned int            GetDeviceIndex();
    device_type             GetDeviceType();
    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();

    void                    SetLEDs(RGBColor* colors);

    void                    SetModeOff();
    void                    SetModeSpectrumCycle();
    void                    SetModeStatic(unsigned char red, unsigned char grn, unsigned char blu);
    void                    SetModeWave();

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device-specific protocol settings                         |
    \*---------------------------------------------------------*/
    unsigned char           dev_transaction_id;
    unsigned char           dev_led_id;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;

    /*---------------------------------------------------------*\
    | Index of device in Razer device list                      |
    \*---------------------------------------------------------*/
    unsigned int            device_index;

    /*---------------------------------------------------------*\
    | HID report index for request and response                 |
    \*---------------------------------------------------------*/
    unsigned char           report_index;
    unsigned char           response_index;

    /*---------------------------------------------------------*\
    | Private functions based on OpenRazer                      |
    \*---------------------------------------------------------*/
    unsigned char           razer_calculate_crc(razer_report* report);
    razer_report            razer_create_report(unsigned char command_class, unsigned char command_id, unsigned char data_size);
    razer_report            razer_create_response();

    razer_report            razer_create_brightness_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char brightness);
    razer_report            razer_create_brightness_standard_report(unsigned char variable_storage, unsigned char led_id, unsigned char brightness);
    razer_report            razer_create_custom_frame_linear_report(unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data);
    razer_report            razer_create_custom_frame_extended_matrix_report(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data);
    razer_report            razer_create_custom_frame_standard_matrix_report(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data);
    razer_report            razer_create_mode_custom_extended_matrix_report();
    razer_report            razer_create_mode_custom_standard_matrix_report(unsigned char variable_storage);
    razer_report            razer_create_mode_none_extended_matrix_report(unsigned char variable_storage, unsigned char led_id);
    razer_report            razer_create_mode_none_standard_matrix_report(unsigned char variable_storage, unsigned char led_id);
    razer_report            razer_create_mode_spectrum_cycle_extended_matrix_report(unsigned char variable_storage, unsigned char led_id);
    razer_report            razer_create_mode_spectrum_cycle_standard_matrix_report(unsigned char variable_storage, unsigned char led_id);
    razer_report            razer_create_mode_static_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char red, unsigned char grn, unsigned char blu);
    razer_report            razer_create_mode_static_standard_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char red, unsigned char grn, unsigned char blu);
    razer_report            razer_create_mode_wave_extended_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char direction);
    razer_report            razer_create_mode_wave_standard_matrix_report(unsigned char variable_storage, unsigned char led_id, unsigned char direction);
    razer_report            razer_create_set_led_effect_report(unsigned char variable_storage, unsigned char led_id, unsigned char effect);
    razer_report            razer_create_set_led_rgb_report(unsigned char variable_storage, unsigned char led_id, unsigned char* rgb_data);

    std::string             razer_get_firmware();
    std::string             razer_get_serial();

    void                    razer_set_brightness(unsigned char brightness);
    void                    razer_set_custom_frame(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char* rgb_data);

    void                    razer_set_mode_breathing();
    void                    razer_set_mode_custom();
    void                    razer_set_mode_none();
    void                    razer_set_mode_spectrum_cycle();
    void                    razer_set_mode_static(unsigned char red, unsigned char grn, unsigned char blu);
    void                    razer_set_mode_wave();

    int                     razer_usb_receive(razer_report* report);
    int                     razer_usb_send(razer_report* report);


};