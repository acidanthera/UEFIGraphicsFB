//
//  UEFIGraphicsFB.h
//  UEFIGraphicsFB
//
//  Copyright © 2021 Goldfish64. All rights reserved.
//

#ifndef _UEFI_GRAPHICS_FB_H_
#define _UEFI_GRAPHICS_FB_H_

#include <IOKit/IOPlatformExpert.h>
#include <pexpert/pexpert.h>

#define super IOPlatformDevice

#define SYSLOG(str, ...) logPrint(__FUNCTION__, str, ## __VA_ARGS__)

#ifdef DEBUG
#define DBGLOG(str, ...) logPrint(__FUNCTION__, str, ## __VA_ARGS__)
#else
#define DBGLOG(str, ...) {}
#endif

class UEFIGraphicsFB : public super {
  OSDeclareDefaultStructors(UEFIGraphicsFB);
  
private:
  inline void logPrint(const char *func, const char *format, ...) {
    char tmp[512];
    tmp[0] = '\0';
    va_list va;
    va_start(va, format);
    vsnprintf(tmp, sizeof (tmp), format, va);
    va_end(va);
    
    IOLog("UEFIGraphicsFB::%s(): %s\n", func, tmp);
  }
  
public:
  //
  // IOService overrides.
  //
  virtual bool start(IOService *provider) APPLE_KEXT_OVERRIDE;
};

#endif
