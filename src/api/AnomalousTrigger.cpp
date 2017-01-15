/*
 *      Copyright (C) 2015 Garrett Brown
 *      Copyright (C) 2015 Team XBMC
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

#include "AnomalousTrigger.h"
#include "log/Log.h"

#include <assert.h>

using namespace JOYSTICK;

#define ANOMOLOUS_MAGNITUDE  0.5f

CAnomalousTrigger::CAnomalousTrigger(unsigned int axisIndex, const kodi::addon::Joystick* joystickInfo)
  : m_axisIndex(axisIndex),
    m_joystickInfo(joystickInfo),
    m_state(STATE_UNKNOWN),
    m_center(CENTER_ZERO),
    m_range(TRIGGER_RANGE_HALF),
    m_bCenterSeen(false),
    m_bPositiveOneSeen(false),
    m_bNegativeOneSeen(false),
    m_bTrigger(true)
{
  assert(m_joystickInfo != nullptr);
}

float CAnomalousTrigger::Filter(float value)
{
  UpdateState(value);

  // Don't return non-zero unless we have some information about the axis.
  // This is needed because an initial value of 1.0 could come from a discrete
  // d-pad or an anomalous trigger.
  if (m_state == STATE_UNKNOWN)
  {
    value = 0.0f;
  }
  else
  {
    // Filter anomalous triggers
    if (IsAnomalousTriggerDetected() && m_bTrigger)
      value = FilterAnomalousTrigger(value, GetCenter(m_center), GetRange(m_range));
  }

  return value;
}

void CAnomalousTrigger::UpdateState(float value)
{
  // First, check for discrete D-pad
  if (m_state == STATE_UNKNOWN)
  {
    // Is value a discrete integer?
    if (value == -1.0f || value == 0.0f || value == 1.0f)
    {
      // Might be a discrete dpad
      if      (value ==  0.0f) m_bCenterSeen      = true;
      else if (value ==  1.0f) m_bPositiveOneSeen = true;
      else if (value == -1.0f) m_bNegativeOneSeen = true;

      if (m_bCenterSeen && m_bPositiveOneSeen && m_bNegativeOneSeen)
      {
        m_state = STATE_DISCRETE_DPAD;
        dsyslog("Discrete D-pad detected on axis %u", m_axisIndex);
      }
    }
    else
    {
      m_state = STATE_NOT_DISCRETE_DPAD;
    }
  }

  // Calculate center position
  if (m_state == STATE_NOT_DISCRETE_DPAD)
  {
    if (value < -ANOMOLOUS_MAGNITUDE)
      m_center = CENTER_NEGATIVE_ONE;
    else if (value > ANOMOLOUS_MAGNITUDE)
      m_center = CENTER_POSITIVE_ONE;
    else
      m_center = CENTER_ZERO;

    if (IsAnomalousTriggerDetected())
      dsyslog("Anomalous trigger detected on axis %u (initial value = %f)", m_axisIndex, value);

    // Assume full range
    m_range = TRIGGER_RANGE_FULL;
    m_state = STATE_RANGE_KNOWN;
  }
}

bool CAnomalousTrigger::IsAnomalousTriggerDetected(void) const
{
  return m_center != CENTER_ZERO;
}

float CAnomalousTrigger::FilterAnomalousTrigger(float value, int center, unsigned int range)
{
  // Translate center to zero
  value -= center;

  // Adjust range to lie in an interval of length 1
  if (range > 1)
    value /= range;

  return value;
}

int CAnomalousTrigger::GetCenter(AXIS_CENTER center)
{
  switch (center)
  {
    case CENTER_NEGATIVE_ONE: return -1;
    case CENTER_POSITIVE_ONE: return  1;
    default:
      break;
  }
  return 0;
}

unsigned int CAnomalousTrigger::GetRange(TRIGGER_RANGE range)
{
  switch (range)
  {
    case TRIGGER_RANGE_HALF: return 1;
    case TRIGGER_RANGE_FULL: return 2;
    default:
      break;
  }
  return 1;
}
