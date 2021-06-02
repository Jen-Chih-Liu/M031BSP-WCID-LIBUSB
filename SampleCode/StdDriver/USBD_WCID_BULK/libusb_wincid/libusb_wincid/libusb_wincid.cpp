// libusb_wincid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libusb.h"
#pragma comment(lib, "libusb-1.0.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//printf veriosn informaiton
	const struct libusb_version *ver;
	ver=libusb_get_version();
	printf("%u.%u.%u\n", ver->major, ver->minor, ver->micro);
	int res;
	res = libusb_init(NULL);
	if (res < 0)
		return res;

	libusb_device_handle *dev_handle;
	dev_handle = libusb_open_device_with_vid_pid(nullptr, 0x0416, 0xb002);
	if (dev_handle == nullptr) {
		printf("Can't open deivces.\n\r");
		return 0;
	}

	res = libusb_kernel_driver_active(dev_handle, 0);
	if (res == 1) {
		libusb_detach_kernel_driver(dev_handle, 0);
		printf("driver error\n");
		return res;
	}

	res = libusb_claim_interface(dev_handle, 0);
	if (res < 0) {
		printf("claim interface error:%d\n", res);		
		return res;
	}
	int outed;
	unsigned char pattern[1024];
	unsigned char in_pattern[1024];
	for (int i = 0; i < 1024; i++)
	{
		pattern[i] = i;
	}
	if (dev_handle != nullptr) {
		// Host -> device
		res = libusb_bulk_transfer(dev_handle, 0x02, pattern, 1024, &outed, 0);
		if (res < 0) {
			printf("bulk transfer error, %d\n", res);
		}
	}
	for (int i = 0; i < 1024; i++)
	{
		in_pattern[i] = 0;
	}
	if (dev_handle != nullptr) {
		// Host -> device
		res = libusb_bulk_transfer(dev_handle, 0x81, in_pattern, 1024, &outed, 0);
		if (res < 0) {
			printf("bulk transfer error, %d\n", res);
		}
	}

	for (int i = 0; i < 1024; i++)
	{
		if (in_pattern[i] != pattern[i])
			printf("address:%d, context:0x%x\n", i, in_pattern[i]);
	}

	libusb_release_interface(dev_handle, 0);

	libusb_close(dev_handle);

	libusb_exit(NULL);
	return 0;
}

