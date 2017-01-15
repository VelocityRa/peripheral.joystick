#pragma once
/*
 *      Copyright (C) 2014-2015 Garrett Brown
 *      Copyright (C) 2014-2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <kodi/peripheral/Peripheral.h>

namespace JOYSTICK
{
  class CPeripheralScanner;
}

class CPeripheralJoystick
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstancePeripheral
{
public:
  CPeripheralJoystick() {}
  virtual ~CPeripheralJoystick();

  virtual ADDON_STATUS Create() override;

  virtual void GetCapabilities(PERIPHERAL_CAPABILITIES &capabilities) override;
  virtual PERIPHERAL_ERROR PerformDeviceScan(unsigned int* peripheral_count, PERIPHERAL_INFO** scan_results) override;
  virtual void FreeScanResults(unsigned int peripheral_count, PERIPHERAL_INFO* scan_results) override;
  virtual PERIPHERAL_ERROR GetEvents(unsigned int* event_count, PERIPHERAL_EVENT** events) override;
  virtual void FreeEvents(unsigned int event_count, PERIPHERAL_EVENT* events) override;
  virtual bool SendEvent(const PERIPHERAL_EVENT* event) override;
  virtual PERIPHERAL_ERROR GetJoystickInfo(unsigned int index, JOYSTICK_INFO* info) override;
  virtual void FreeJoystickInfo(JOYSTICK_INFO* info) override;
  virtual PERIPHERAL_ERROR GetFeatures(const JOYSTICK_INFO* joystick, const char* controller_id,
                                       unsigned int* feature_count, JOYSTICK_FEATURE** features) override;
  virtual void FreeFeatures(unsigned int feature_count, JOYSTICK_FEATURE* features) override;
  virtual PERIPHERAL_ERROR MapFeatures(const JOYSTICK_INFO* joystick, const char* controller_id,
                                       unsigned int feature_count, const JOYSTICK_FEATURE* features) override;
  virtual PERIPHERAL_ERROR GetIgnoredPrimitives(const JOYSTICK_INFO* joystick,
                                                unsigned int* primitive_count,
                                                JOYSTICK_DRIVER_PRIMITIVE** primitives) override;
  virtual void FreePrimitives(unsigned int primitive_count, JOYSTICK_DRIVER_PRIMITIVE* primitives) override;
  virtual PERIPHERAL_ERROR SetIgnoredPrimitives(const JOYSTICK_INFO* joystick,
                                                unsigned int primitive_count,
                                                const JOYSTICK_DRIVER_PRIMITIVE* primitives) override;
  virtual void SaveButtonMap(const JOYSTICK_INFO* joystick) override;
  virtual void RevertButtonMap(const JOYSTICK_INFO* joystick) override;
  virtual void ResetButtonMap(const JOYSTICK_INFO* joystick, const char* controller_id) override;
  virtual void PowerOffJoystick(unsigned int index) override;

private:
  JOYSTICK::CPeripheralScanner* m_scanner;
};
