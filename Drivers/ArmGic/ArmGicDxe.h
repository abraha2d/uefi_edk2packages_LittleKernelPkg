/*++

Copyright (c) 2013-2014, ARM Ltd. All rights reserved.<BR>

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

--*/

#ifndef __ARM_GIC_DXE_H__
#define __ARM_GIC_DXE_H__

#include <Library/ArmGicLib.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Cpu.h>
#include <Protocol/HardwareInterrupt.h>

#include <LittleKernel.h>

extern UINTN                        mGicNumInterrupts;
extern HARDWARE_INTERRUPT_HANDLER  *gRegisteredInterruptHandlers;
extern lkapi_t                     *mLKApi;

//
// Common API
//
void lkapi_int_register_handler(unsigned int vector, lkapi_int_handler func, void *arg);
int lkapi_int_mask(unsigned int vector);
int lkapi_int_unmask(unsigned int vector);

EFI_STATUS
InstallAndRegisterInterruptService (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL   *InterruptProtocol,
  IN EFI_CPU_INTERRUPT_HANDLER          InterruptHandler,
  IN EFI_EVENT_NOTIFY                   ExitBootServicesEvent
  );

EFI_STATUS
EFIAPI
RegisterInterruptSource (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL    *This,
  IN HARDWARE_INTERRUPT_SOURCE          Source,
  IN HARDWARE_INTERRUPT_HANDLER         Handler
  );

//
// GicV2 API
//
EFI_STATUS
GicV2DxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

//
// GicV3 API
//
EFI_STATUS
GicV3DxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

#endif
