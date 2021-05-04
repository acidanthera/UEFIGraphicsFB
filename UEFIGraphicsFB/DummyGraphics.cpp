//
//  DummyGraphics.cpp
//  UEFIGraphicsFB
//
//  Copyright © 2021 Goldfish64. All rights reserved.
//

#include "DummyGraphics.h"

OSDefineMetaClassAndStructors(DummyGraphics, IOService);

IOService *DummyGraphics::probe(IOService *provider, SInt32 *score) {
  if (!IOService::probe(provider, score)) {
    return nullptr;
  }
  return this;
}

bool DummyGraphics::start(IOService *provider) {
  // Discard all other drivers as no coexistence is allowed.
  if (!IOService::start(provider)) {
    return false;
  }
  return true;
}

void DummyGraphics::stop(IOService *provider) {
  IOService::stop(provider);
}
