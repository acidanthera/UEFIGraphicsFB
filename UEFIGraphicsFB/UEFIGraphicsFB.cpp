//
//  UEFIGraphicsFB.cpp
//  UEFIGraphicsFB
//
//  Copyright © 2021 Goldfish64. All rights reserved.
//

#include "UEFIGraphicsFB.h"

#include <pexpert/pexpert.h>

OSDefineMetaClassAndStructors(UEFIGraphicsFB, super);

static char const pixelFormatString16[] = IO16BitDirectPixels "\0";
static char const pixelFormatString32[] = IO32BitDirectPixels "\0";

void UEFIGraphicsFB::logPrint(const char *func, const char *format, ...) {
  char tmp[1024];
  tmp[0] = '\0';
  va_list va;
  va_start(va, format);
  vsnprintf(tmp, sizeof (tmp), format, va);
  va_end(va);
  
  IOLog("UEFIGraphicsFB::%s(): %s\n", func, tmp);
}

bool UEFIGraphicsFB::start(IOService *provider) {
  if (!PE_state.initialized) {
    SYSLOG("PE state is not initialized");
    return false;
  }
  
  //
  // Pull video parameters from kernel for later.
  //
  videoBaseAddress  = (UInt32) PE_state.video.v_baseAddr;
  videoDisplay      = (UInt32) PE_state.video.v_display;
  videoWidth        = (UInt32) PE_state.video.v_width;
  videoHeight       = (UInt32) PE_state.video.v_height;
  videoDepth        = (UInt32) PE_state.video.v_depth;
  videoBytesPerRow  = (UInt32) PE_state.video.v_rowBytes;
  
  if (videoDepth != kUEFIBitDepth16 && videoDepth != kUEFIBitDepth32) {
    SYSLOG("Unsupported bit depth %u", videoDepth);
    return false;
  }
  
  if (!super::start(provider)) {
    return false;
  }
  
  SYSLOG("Framebuffer is at %X (width %u height %u bits %u)", videoBaseAddress, videoWidth, videoHeight, videoDepth);
  return true;
}

void UEFIGraphicsFB::stop(IOService *provider) {
  if (videoMemory != NULL) {
    videoMemory->release();
    videoMemory = NULL;
  }
  
  super::stop(provider);
}

IOReturn UEFIGraphicsFB::enableController() {
  //
  // Map framebuffer memory.
  //
  videoMemory = IODeviceMemory::withRange(videoBaseAddress, videoHeight * videoBytesPerRow);
  if (videoMemory == NULL) {
    return kIOReturnDeviceError;
  }
  
  DBGLOG("Framebuffer is enabled");
  return kIOReturnSuccess;
}

bool UEFIGraphicsFB::isConsoleDevice() {
  return true;
}

IODeviceMemory* UEFIGraphicsFB::getApertureRange(IOPixelAperture aperture) {
  if (kIOFBSystemAperture != aperture) {
    return NULL;
  }

  if (videoMemory != NULL) {
    videoMemory->retain();
  }
  return videoMemory;
}

const char *UEFIGraphicsFB::getPixelFormats() {
  return NULL;
}

IOItemCount UEFIGraphicsFB::getDisplayModeCount() {
  return kUEFIDisplayModeCount;
}

IOReturn UEFIGraphicsFB::getDisplayModes(IODisplayModeID *allDisplayModes) {
  allDisplayModes[0] = kUEFIDisplayModeID;
  return kIOReturnSuccess;
}

IOReturn UEFIGraphicsFB::getInformationForDisplayMode(IODisplayModeID displayMode, IODisplayModeInformation *info) {
  if (displayMode != kUEFIDisplayModeID) {
    return kIOReturnBadArgument;
  }
  
  bzero(info, sizeof (*info));
  
  info->nominalWidth  = videoWidth;
  info->nominalHeight = videoHeight;
  info->refreshRate   = 60<<16;
  info->maxDepthIndex = kUEFIDisplayDepthIndex;
  
  return kIOReturnSuccess;
}

UInt64 UEFIGraphicsFB::getPixelFormatsForDisplayMode(IODisplayModeID displayMode, IOIndex depth) {
  //
  // Obsolete method that always returns zero.
  //
  return 0;
}

IOReturn UEFIGraphicsFB::getPixelInformation(IODisplayModeID displayMode, IOIndex depth, IOPixelAperture aperture, IOPixelInformation *pixelInfo) {
  if (displayMode != kUEFIDisplayModeID || depth != kUEFIDisplayDepthIndex) {
    return kIOReturnBadArgument;
  }
  if (aperture != kIOFBSystemAperture) {
    return kIOReturnUnsupportedMode;
  }
  
  bzero(pixelInfo, sizeof (*pixelInfo));
  
  pixelInfo->bytesPerRow          = videoBytesPerRow;
  pixelInfo->bitsPerPixel         = videoDepth;
  pixelInfo->pixelType            = kIORGBDirectPixels;
  pixelInfo->bitsPerComponent     = 8;
  pixelInfo->componentCount       = 3;
  pixelInfo->componentMasks[0]    = 0xFF0000;
  pixelInfo->componentMasks[1]    = 0x00FF00;
  pixelInfo->componentMasks[2]    = 0x0000FF;
  pixelInfo->activeWidth          = videoWidth;
  pixelInfo->activeHeight         = videoHeight;
  
  if (videoDepth == kUEFIBitDepth32) {
    strlcpy(&pixelInfo->pixelFormat[0], &pixelFormatString32[0], sizeof (IOPixelEncoding));
  } else if (videoDepth == kUEFIBitDepth16) {
    strlcpy(&pixelInfo->pixelFormat[0], &pixelFormatString16[0], sizeof (IOPixelEncoding));
  }
  
  return kIOReturnSuccess;
}

IOReturn UEFIGraphicsFB::getCurrentDisplayMode(IODisplayModeID *displayMode, IOIndex *depth) {
  *displayMode = kUEFIDisplayModeID;
  *depth       = kUEFIDisplayDepthIndex;

  return kIOReturnSuccess;
}
