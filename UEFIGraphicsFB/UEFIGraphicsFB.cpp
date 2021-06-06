//
//  UEFIGraphicsFB.cpp
//  UEFIGraphicsFB
//
//  Copyright © 2021 Goldfish64. All rights reserved.
//

#include "UEFIGraphicsFB.h"

OSDefineMetaClassAndStructors(UEFIGraphicsFB, super);

bool UEFIGraphicsFB::start(IOService *provider) {
  if (!PE_state.initialized) {
    SYSLOG("PE state is not initialized");
    return false;
  }
  
  //
  // Pull video parameters from kernel.
  //
  UInt32 videoBaseAddress  = (UInt32) PE_state.video.v_baseAddr;
  UInt32 videoWidth        = (UInt32) PE_state.video.v_width;
  UInt32 videoHeight       = (UInt32) PE_state.video.v_height;
  UInt32 videoDepth        = (UInt32) PE_state.video.v_depth;
  UInt32 videoBytesPerRow  = (UInt32) PE_state.video.v_rowBytes;
  
  if (!super::start(provider)) {
    return false;
  }
  
  //
  // Create framebuffer memory properties.
  // IONDRVFramebuffer will pull this information on start.
  //
  IODeviceMemory::InitElement fbMemList[1];
  fbMemList[0].start  = videoBaseAddress;
  fbMemList[0].length = videoHeight * videoBytesPerRow;
  DBGLOG("Framebuffer memory start 0x%llX length 0x%llX", fbMemList[0].start, fbMemList[0].length);
  
  OSArray *fbMemArray = IODeviceMemory::arrayFromList(fbMemList, 1);
  setDeviceMemory(fbMemArray);
  fbMemArray->release();
  
  //
  // IONDRVFramebuffer looks for either IOPCIDevice or IOPlatformDevice named "display".
  //
  registerService();
  
  SYSLOG("Framebuffer is at 0x%X (width %u height %u bits %u bytes/row %u)", videoBaseAddress, videoWidth, videoHeight, videoDepth, videoBytesPerRow);
  return true;
}
