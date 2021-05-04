//
//  DummyGraphics.h
//  UEFIGraphicsFB
//
//  Copyright © 2021 Goldfish64. All rights reserved.
//

#ifndef _DUMMY_GRAPHICS_H_
#define _DUMMY_GRAPHICS_H_

#include "UEFIGraphicsFB.h"

class DummyGraphics : public IOService {
  OSDeclareDefaultStructors(DummyGraphics);

public:
  //
  // IOService functions.
  //
  virtual IOService *probe(IOService *provider, SInt32 *score) APPLE_KEXT_OVERRIDE;
  virtual bool start(IOService *provider) APPLE_KEXT_OVERRIDE;
  virtual void stop(IOService *provider) APPLE_KEXT_OVERRIDE;

};

#endif /* _DUMMY_GRAPHICS_H_ */
