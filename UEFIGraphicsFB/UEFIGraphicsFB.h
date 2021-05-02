//
//  UEFIGraphicsFB.h
//  UEFIGraphicsFB
//
//  Copyright © 2021 Goldfish64. All rights reserved.
//

#ifndef _UEFI_GRAPHICS_FB_H_
#define _UEFI_GRAPHICS_FB_H_

#include <IOKit/graphics/IOFramebuffer.h>

#define super IOFramebuffer

#define SYSLOG(str, ...) logPrint(__FUNCTION__, str, ## __VA_ARGS__)

#ifdef DEBUG
#define DBGLOG(str, ...) logPrint(__FUNCTION__, str, ## __VA_ARGS__)
#else
#define DBGLOG(str, ...) {}
#endif

#define kUEFIDisplayModeCount         1
#define kUEFIDisplayModeID            1
#define kUEFIDisplayDepthIndex        0

#define kUEFIBitDepth16               16
#define kUEFIBitDepth32               32

class UEFIGraphicsFB : public super {
  OSDeclareDefaultStructors(UEFIGraphicsFB);
  
private:
  UInt32 videoBaseAddress;
  UInt32 videoDisplay;
  UInt32 videoWidth;
  UInt32 videoHeight;
  UInt32 videoDepth;
  UInt32 videoBytesPerRow;
  
  IODeviceMemory  *videoMemory;
  
  void logPrint(const char *func, const char *format, ...);
  
public:
  //
  // IOService functions.
  //
  virtual bool start(IOService *provider) APPLE_KEXT_OVERRIDE;
  virtual void stop(IOService *provider) APPLE_KEXT_OVERRIDE;
  
  
  //
  // IOFramebuffer functions.
  //
  virtual IOReturn enableController() APPLE_KEXT_OVERRIDE;
  virtual bool isConsoleDevice() APPLE_KEXT_OVERRIDE;
  virtual IODeviceMemory *getApertureRange(IOPixelAperture aperture) APPLE_KEXT_OVERRIDE;
  virtual const char *getPixelFormats() APPLE_KEXT_OVERRIDE;
  virtual IOItemCount getDisplayModeCount() APPLE_KEXT_OVERRIDE;
  virtual IOReturn getDisplayModes(IODisplayModeID *allDisplayModes) APPLE_KEXT_OVERRIDE;
  virtual IOReturn getInformationForDisplayMode(IODisplayModeID displayMode, IODisplayModeInformation *info) APPLE_KEXT_OVERRIDE;
  virtual UInt64 getPixelFormatsForDisplayMode(IODisplayModeID displayMode, IOIndex depth) APPLE_KEXT_OVERRIDE;
  virtual IOReturn getPixelInformation(IODisplayModeID displayMode, IOIndex depth, IOPixelAperture aperture, IOPixelInformation *pixelInfo) APPLE_KEXT_OVERRIDE;
  virtual IOReturn getCurrentDisplayMode(IODisplayModeID *displayMode, IOIndex *depth) APPLE_KEXT_OVERRIDE;
};

#endif
