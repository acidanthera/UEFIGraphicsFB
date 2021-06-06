UEFIGraphicsFB
==============

[![Build Status](https://github.com/acidanthera/UEFIGraphicsFB/workflows/CI/badge.svg?branch=master)](https://github.com/acidanthera/UEFIGraphicsFB/actions) [![Scan Status](https://scan.coverity.com/projects/23074/badge.svg?flat=1)](https://scan.coverity.com/projects/23074)

IOGraphics framebuffer based on UEFI framebuffer passed over XNU boot arguments. Provides basic
display output without hardware acceleration on generic hardware.

#### Disabling other GPUs

If there are other conflicting GPUs in the system, one can disable them by injecting the following properties:

- `name` = `<unused>`
- `class-code` = `<FF FF FF FF>`

#### Credits
- [Apple](https://www.apple.com) for macOS
- [Goldfish64](https://github.com/Goldfish64) for this software
