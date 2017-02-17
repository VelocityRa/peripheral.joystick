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
 */
#pragma once

#include <kodi/peripheral/PeripheralUtils.h>

#include <string>
#include <vector>

namespace JOYSTICK
{
  class CJoystick : public kodi::addon::Joystick
  {
  public:
    CJoystick(const std::string& strProvider);
    virtual ~CJoystick(void) { Deinitialize(); }

    /*!
     * Compare joystick properties
     */
    virtual bool Equals(const CJoystick* rhs) const;

    /*!
     * Override subclass to sanitize name (strip trailing whitespace, etc)
     */
    void SetName(const std::string& strName);

    /*!
     * The time that this joystick was discovered
     */
    int64_t DiscoverTimeMs(void) const { return m_discoverTimeMs; }

    /*!
     * The time that this joystick received its first input
     */
    int64_t ActivateTimeMs(void) const { return m_activateTimeMs; }

    /*!
     * The time that this joystick delivered its first event
     */
    int64_t FirstEventTimeMs(void) const { return m_firstEventTimeMs; }

    /*!
     * The most recent time that this joystick delivered an event
     */
    int64_t LastEventTimeMs(void) const { return m_lastEventTimeMs; }

    /*!
     * Initialize the joystick object. Joystick will be initialized before the
     * first call to GetEvents().
     */
    virtual bool Initialize(void);

    /*!
     * Deinitialize the joystick object. GetEvents() will not be called after
     * deinitialization.
     */
    virtual void Deinitialize(void);

    /*!
     * Get events that have occurred since the last call to GetEvents()
     */
    virtual bool GetEvents(std::vector<kodi::addon::PeripheralEvent>& events);

    /*!
     * Send an event to a joystick
     */
    virtual bool SendEvent(const kodi::addon::PeripheralEvent& event);

    /*!
     * Process events sent to the joystick
     */
    virtual void ProcessEvents() { }

    /*!
     * Tries to power off the joystick.
     */
    virtual void PowerOff() { }

  protected:
    /*!
     * Implemented by derived class to scan for events
     */
    virtual bool ScanEvents(void) = 0;

    virtual bool SetMotor(unsigned int motorIndex, float magnitude) { return false; }

    virtual void SetButtonValue(unsigned int buttonIndex, JOYSTICK_STATE_BUTTON buttonValue);
    virtual void SetHatValue(unsigned int hatIndex, JOYSTICK_STATE_HAT hatValue);
    virtual void SetAxisValue(unsigned int axisIndex, JOYSTICK_STATE_AXIS axisValue);
    void SetAxisValue(unsigned int axisIndex, long value, long maxAxisAmount);

  private:
    void GetButtonEvents(std::vector<kodi::addon::PeripheralEvent>& events);
    void GetHatEvents(std::vector<kodi::addon::PeripheralEvent>& events);
    void GetAxisEvents(std::vector<kodi::addon::PeripheralEvent>& events);

    void UpdateTimers(void);

    /*!
     * Normalize the axis to the closed interval [-1.0, 1.0].
     */
    static float NormalizeAxis(long value, long maxAxisAmount);
    static float ScaleDeadzone(float value);

    struct JoystickAxis
    {
      JOYSTICK_STATE_AXIS state = 0.0f;
      bool bSeen = false;
    };

    struct JoystickState
    {
      std::vector<JOYSTICK_STATE_BUTTON> buttons;
      std::vector<JOYSTICK_STATE_HAT>    hats;
      std::vector<JoystickAxis>          axes;
    };

    JoystickState                     m_state;
    JoystickState                     m_stateBuffer;
    int64_t                           m_discoverTimeMs;
    int64_t                           m_activateTimeMs;
    int64_t                           m_firstEventTimeMs;
    int64_t                           m_lastEventTimeMs;
  };
}
