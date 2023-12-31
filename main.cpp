#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>

//DriverEntry
NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
);

//
EVT_WDF_DRIVER_DEVICE_ADD TimerEvtDeviceAdd;
EVT_WDF_DRIVER_UNLOAD TimerEvtWdfDriverUnload;

NTSTATUS TimerEvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
);

void TimerEvtWdfDriverUnload(
    _In_ WDFDRIVER Driver
);

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    // NTSTATUS variable to record success or failure
    NTSTATUS status = STATUS_SUCCESS;

    // Allocate the driver configuration object
    WDF_DRIVER_CONFIG config;

    // Initialize the driver configuration object to register the
    // entry point for the EvtDeviceAdd callback, TimerEvtDeviceAdd
    WDF_DRIVER_CONFIG_INIT(&config, TimerEvtDeviceAdd);
    config.EvtDriverUnload = TimerEvtWdfDriverUnload;

    // Finally, create the driver object
    status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);

    if (status == STATUS_SUCCESS) {
        // Set the timer resolution to 1ms
        ExSetTimerResolution(10000, TRUE);
    }

    return status;
}

// This function is mandatory for driver device addition
NTSTATUS TimerEvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit)
{
    // We're not using the driver object,
    // so we need to mark it as unreferenced
    UNREFERENCED_PARAMETER(Driver);

    NTSTATUS status;

    // Allocate the device object
    WDFDEVICE hDevice;

    // Create the device object
    status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &hDevice);

    return status;
}
// Remove the timer request when unloading the driver
void TimerEvtWdfDriverUnload(_In_ WDFDRIVER Driver)
{
    UNREFERENCED_PARAMETER(Driver);

    // Set the timer resolution back to default
    ExSetTimerResolution(0, FALSE);
}
