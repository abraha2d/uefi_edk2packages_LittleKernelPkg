/** @file
  Template library implementation to support ResetSystem Runtime call.

  Fill in the templates with what ever makes you system reset.


  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/MemoryAllocationLib.h>

#include <LittleKernel.h>

lkapi_t* LKApi = NULL;

CHAR8*
Unicode2Ascii (
  CONST CHAR16* UnicodeStr
)
{
  UINTN BufSize = StrSize (UnicodeStr) * sizeof (CHAR8);
  CHAR8* AsciiStr = AllocatePool(BufSize);
  if (AsciiStr == NULL) {
    return NULL;
  }

  UnicodeStrToAsciiStrS(UnicodeStr, AsciiStr, BufSize);

  return AsciiStr;
}


/**
  Resets the entire platform.

  @param  ResetType             The type of reset to perform.
  @param  ResetStatus           The status code for the reset.
  @param  DataSize              The size, in bytes, of WatchdogData.
  @param  ResetData             For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                Unicode string, optionally followed by additional binary data.

**/
EFI_STATUS
EFIAPI
LibResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
{
  // convert to ascii
  CHAR8* AsciiResetStr = NULL;

  if (ResetData) {
    AsciiResetStr = Unicode2Ascii(ResetData);
    if (AsciiResetStr==NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  switch (ResetType) {
  case EfiResetCold:
    // system power cycle
    LKApi->reset_cold(AsciiResetStr);
    break;

  case EfiResetWarm:
    // not a full power cycle, maybe memory stays around.
    // if not support do the same thing as EfiResetCold.
    LKApi->reset_warm(AsciiResetStr);
    break;

  case EfiResetShutdown:
    // turn off the system.
    // if not support do the same thing as EfiResetCold.
    LKApi->reset_shutdown(AsciiResetStr);
    break;

  default:
    if (AsciiResetStr)
      FreePool(AsciiResetStr);
    return EFI_INVALID_PARAMETER;
  }

  //
  // If we reset, we would not have returned...
  //
  if (AsciiResetStr)
    FreePool(AsciiResetStr);
  return EFI_DEVICE_ERROR;
}



/**
  Initialize any infrastructure required for LibResetSystem () to function.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
LibInitializeResetSystem (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  LKApi = GetLKApi();

  return EFI_SUCCESS;
}

