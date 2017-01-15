/*
 *      Copyright (C) 2016 Garrett Brown
 *      Copyright (C) 2016 Team Kodi
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
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "ButtonMapUtils.h"

#include <kodi/peripheral/kodi_peripheral_utils.hpp>

using namespace JOYSTICK;

bool ButtonMapUtils::PrimitivesEqual(const ADDON::JoystickFeature& lhs, const ADDON::JoystickFeature& rhs)
{
  if (lhs.Type() == rhs.Type())
  {
    switch (lhs.Type())
    {
    case JOYSTICK_FEATURE_TYPE_SCALAR:
    case JOYSTICK_FEATURE_TYPE_MOTOR:
    {
      return lhs.Primitive(JOYSTICK_SCALAR_PRIMITIVE) == rhs.Primitive(JOYSTICK_SCALAR_PRIMITIVE);
    }
    case JOYSTICK_FEATURE_TYPE_ANALOG_STICK:
    {
      return lhs.Primitive(JOYSTICK_ANALOG_STICK_UP)    == rhs.Primitive(JOYSTICK_ANALOG_STICK_UP) &&
             lhs.Primitive(JOYSTICK_ANALOG_STICK_DOWN)  == rhs.Primitive(JOYSTICK_ANALOG_STICK_DOWN) &&
             lhs.Primitive(JOYSTICK_ANALOG_STICK_RIGHT) == rhs.Primitive(JOYSTICK_ANALOG_STICK_RIGHT) &&
             lhs.Primitive(JOYSTICK_ANALOG_STICK_LEFT)  == rhs.Primitive(JOYSTICK_ANALOG_STICK_LEFT);
    }
    case JOYSTICK_FEATURE_TYPE_ACCELEROMETER:
    {
      return lhs.Primitive(JOYSTICK_ACCELEROMETER_POSITIVE_X) == rhs.Primitive(JOYSTICK_ACCELEROMETER_POSITIVE_X) &&
             lhs.Primitive(JOYSTICK_ACCELEROMETER_POSITIVE_Y) == rhs.Primitive(JOYSTICK_ACCELEROMETER_POSITIVE_Y) &&
             lhs.Primitive(JOYSTICK_ACCELEROMETER_POSITIVE_Z) == rhs.Primitive(JOYSTICK_ACCELEROMETER_POSITIVE_Z);
    }
    default:
      break;
    }
  }
  return false;
}
