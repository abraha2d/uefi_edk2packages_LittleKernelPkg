/** @file

  Copyright (c) 2011, ARM Ltd. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __ARM_VE_GRAPHICS_DXE_H__
#define __ARM_VE_GRAPHICS_DXE_H__


#include <Base.h>

#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Library/BltLib.h>

#include <Protocol/DevicePath.h>
#include <Protocol/LKDisplay.h>
#include <Protocol/Cpu.h>

#include <LittleKernel.h>

extern lkapi_t* LKApi;
extern LK_DISPLAY_FLUSH_MODE gLCDFlushMode;
extern BOOLEAN gDisplayNeedsFlush;

//#define DOUBLE_BUFFER 1
//#define ROTATION_SUPPORT 1

//
// Device structures
//
typedef struct {
  VENDOR_DEVICE_PATH            Guid;
  EFI_DEVICE_PATH_PROTOCOL      End;
} LCD_GRAPHICS_DEVICE_PATH;

typedef struct {
  UINT32                                Signature;
  EFI_HANDLE                            Handle;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  ModeInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE     Mode;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          Gop;
  LCD_GRAPHICS_DEVICE_PATH              DevicePath;
  EFI_LK_DISPLAY_PROTOCOL               LKDisplay;
  EFI_EVENT                             ExitBootServicesEvent;
  EFI_PHYSICAL_ADDRESS                  FrameBufferBase;
} LCD_INSTANCE;

#define LCD_INSTANCE_SIGNATURE  SIGNATURE_32('l', 'c', 'd', '0')

#define LCD_INSTANCE_FROM_GOP_THIS(a)     CR (a, LCD_INSTANCE, Gop, LCD_INSTANCE_SIGNATURE)
#define LCD_INSTANCE_FROM_LKDISPLAY_THIS(a)     CR (a, LCD_INSTANCE, LKDisplay, LCD_INSTANCE_SIGNATURE)

//
// Function Prototypes
//

VOID
LcdGraphicsExitBootServicesEvent (
    IN EFI_EVENT  Event,
    IN VOID       *Context
);

EFI_STATUS
EFIAPI
LcdGraphicsQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
);

EFI_STATUS
EFIAPI
LcdGraphicsSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *This,
  IN UINT32                        ModeNumber
);

EFI_STATUS
EFIAPI
LcdGraphicsBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL       *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BltBuffer,     OPTIONAL
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta           OPTIONAL
);

UINTN
GetBytesPerPixel (
  VOID
  );

EFI_STATUS
EFIAPI
GraphicsOutputDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
);

EFI_STATUS
InitializeDisplay (
  IN LCD_INSTANCE* Instance
);

EFI_STATUS
LcdIndentify (
  VOID
);

EFI_STATUS
LcdInitialize (
  VOID
);

EFI_STATUS
LcdSetMode (
  IN UINT32  ModeNumber
);

VOID
LcdShutdown (
  VOID
);

UINT32
LKDisplayGetDensity (
  IN EFI_LK_DISPLAY_PROTOCOL* This
);

VOID
LKDisplaySetFlushMode (
  IN EFI_LK_DISPLAY_PROTOCOL* This,
  IN LK_DISPLAY_FLUSH_MODE Mode
);

LK_DISPLAY_FLUSH_MODE
LKDisplayGetFlushMode (
  IN EFI_LK_DISPLAY_PROTOCOL* This
);

VOID
LKDisplayFlushScreen (
  IN EFI_LK_DISPLAY_PROTOCOL* This
);

UINT32
LKDisplayGetPortraitMode (
  VOID
);

UINT32
LKDisplayGetLandscapeMode (
  VOID
);

INTN
LcdGetPixelFormat (
  VOID
  );

STATIC inline
UINT64
GetTimeMs (
  VOID
)
{
  return GetTimeInNanoSecond(GetPerformanceCounter()) / 1000000ULL;
}

#endif /* __ARM_VE_GRAPHICS_DXE_H__ */
