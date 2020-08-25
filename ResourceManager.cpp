#include "ResourceManager.h"
#include "ProfileManager.h"

std::unique_ptr<ResourceManager> ResourceManager::instance;

using namespace std::chrono_literals;

ResourceManager *ResourceManager::get()
{
    if(!instance)
    {
        instance = std::make_unique<ResourceManager>();
    }
        
    return instance.get();
}

ResourceManager::ResourceManager()
{
    detection_percent = 100;
    detection_string = "";
    detection_is_required = false;
    DetectDevicesThread = nullptr;
}

ResourceManager::~ResourceManager()
{
    Cleanup();
}

void ResourceManager::RegisterI2CBus(i2c_smbus_interface *bus)
{
    busses.push_back(bus);
}

std::vector<i2c_smbus_interface*> & ResourceManager::GetI2CBusses()
{
    return busses;
}

void ResourceManager::RegisterRGBController(RGBController *rgb_controller)
{
    rgb_controllers.push_back(rgb_controller);
    DeviceListChanged();
}

std::vector<RGBController*> & ResourceManager::GetRGBControllers()
{
    return rgb_controllers;
}

void ResourceManager::RegisterI2CBusDetector(I2CBusDetectorFunction detector)
{
    i2c_bus_detectors.push_back(detector);
}

void ResourceManager::RegisterI2CDeviceDetector(std::string name, I2CDeviceDetectorFunction detector)
{
    i2c_device_detector_strings.push_back(name);
    i2c_device_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceDetector(std::string name, DeviceDetectorFunction detector)
{
    device_detector_strings.push_back(name);
    device_detectors.push_back(detector);
}

void ResourceManager::RegisterDeviceListChangeCallback(ResourceManagerCallback new_callback, void * new_callback_arg)
{
    DeviceListChangeCallbacks.push_back(new_callback);
    DeviceListChangeCallbackArgs.push_back(new_callback_arg);
}

void ResourceManager::DeviceListChanged()
{
    DeviceListChangeMutex.lock();

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < DeviceListChangeCallbacks.size(); callback_idx++)
    {
        DeviceListChangeCallbacks[callback_idx](DeviceListChangeCallbackArgs[callback_idx]);
    }

    DeviceListChangeMutex.unlock();
}

unsigned int ResourceManager::GetDetectionPercent()
{
    return (detection_percent.load());
}

const char *ResourceManager::GetDetectionString()
{
    return (detection_string);
}

void ResourceManager::Cleanup()
{
    ResourceManager::get()->WaitForDeviceDetection();

    for(RGBController* rgb_controller : rgb_controllers)
    {
        delete rgb_controller;
    }
    rgb_controllers.clear();

    for(i2c_smbus_interface* bus : busses)
    {
        delete bus;
    }
    busses.clear();

    if(DetectDevicesThread)
    {
        DetectDevicesThread->join();
        delete DetectDevicesThread;
        DetectDevicesThread = nullptr;
    }
}

void ResourceManager::DetectDevices()
{
    /*-------------------------------------------------*\
    | Do nothing is it is already detecting devices     |
    \*-------------------------------------------------*/
    if(detection_is_required.load())
    {
        return;
    }

    /*-------------------------------------------------*\
    | If there's anything left from the last time,      |
    | we shall remove it first                          |
    \*-------------------------------------------------*/
    detection_percent = 0;
    Cleanup();

    /*-------------------------------------------------*\
    | Start the device detection thread                 |
    \*-------------------------------------------------*/
    detection_is_required = true;
    DetectDevicesThread = new std::thread(&ResourceManager::DetectDevicesThreadFunction, this);

    /*-------------------------------------------------*\
    | Release the current thread to allow detection     |
    | thread to start                                   |
    \*-------------------------------------------------*/
    std::this_thread::sleep_for(1ms);
}

void ResourceManager::DetectDevicesThreadFunction()
{
    DetectDeviceMutex.lock();
    unsigned int prev_count = 0;
    float        percent = 0.0f;

    ProfileManager profile_manager(rgb_controllers);

    /*-------------------------------------------------*\
    | Start at 0% detection progress                    |
    \*-------------------------------------------------*/
    detection_percent = 0;

    /*-------------------------------------------------*\
    | Detect i2c busses                                 |
    \*-------------------------------------------------*/
    for(int i2c_bus_detector_idx = 0; i2c_bus_detector_idx < i2c_bus_detectors.size() && detection_is_required.load(); i2c_bus_detector_idx++)
    {
        i2c_bus_detectors[i2c_bus_detector_idx](busses);
    }

    /*-------------------------------------------------*\
    | Detect i2c devices                                |
    \*-------------------------------------------------*/
    for(int i2c_detector_idx = 0; i2c_detector_idx < i2c_device_detectors.size() && detection_is_required.load(); i2c_detector_idx++)
    {
        detection_string = i2c_device_detector_strings[i2c_detector_idx].c_str();
        DeviceListChanged();

        i2c_device_detectors[i2c_detector_idx](busses, rgb_controllers);

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers.size() != prev_count)
        {
            DeviceListChanged();
        }
        prev_count = rgb_controllers.size();

        percent = (i2c_detector_idx + 1.0f) / (i2c_device_detectors.size() + device_detectors.size());

        detection_percent = percent * 100.0f;
    }

    /*-------------------------------------------------*\
    | Detect other devices                              |
    \*-------------------------------------------------*/
    for(int detector_idx = 0; detector_idx < device_detectors.size() && detection_is_required.load(); detector_idx++)
    {
        detection_string = device_detector_strings[detector_idx].c_str();
        DeviceListChanged();

        device_detectors[detector_idx](rgb_controllers);

        /*-------------------------------------------------*\
        | If the device list size has changed, call the     |
        | device list changed callbacks                     |
        \*-------------------------------------------------*/
        if(rgb_controllers.size() != prev_count)
        {
            DeviceListChanged();
        }
        prev_count = rgb_controllers.size();

        percent = (detector_idx + 1.0f + i2c_device_detectors.size()) / (i2c_device_detectors.size() + device_detectors.size());

        detection_percent = percent * 100.0f;
    }

    profile_manager.LoadSizeFromProfile("sizes.ors");

    /*-------------------------------------------------*\
    | Make sure that when the detection is done,        |
    | progress bar is set to 100%                       |
    \*-------------------------------------------------*/

    detection_is_required = false;
    detection_percent = 100;
    detection_string = "";

    DetectDeviceMutex.unlock();
}

void ResourceManager::StopDeviceDetection()
{
    detection_is_required = false;
    detection_percent = 100;
    detection_string = "Stopping";
}

void ResourceManager::WaitForDeviceDetection()
{
    DetectDeviceMutex.lock();
    DetectDeviceMutex.unlock();
}
