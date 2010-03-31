// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// [Guard]
#ifndef _FOG_CORE_TIMER_H
#define _FOG_CORE_TIMER_H

// [Dependencies]
#include <Fog/Core/Object.h>
#include <Fog/Core/Task.h>
#include <Fog/Core/Time.h>

//! @addtogroup Fog_Core
//! @{

namespace Fog {

struct EventLoop;

// ============================================================================
// [Fog::Timer]
// ============================================================================

struct FOG_API Timer : public Object
{
  FOG_DECLARE_OBJECT(Timer, Object)

  Timer();
  virtual ~Timer();

  bool isRunning();

  bool start();
  void stop();

  FOG_INLINE TimeDelta getInterval() const { return _interval; }
  void setInterval(TimeDelta interval);

  virtual void onTimer(TimerEvent* e);

  FOG_EVENT_BEGIN()
    FOG_EVENT_DEF(EVENT_TIMER, onTimer, TimerEvent, OVERRIDE)
  FOG_EVENT_END()

protected:
  Task* _task;
  TimeDelta _interval;

private:
  friend struct TimerTask;
};

} // Fog namespace

//! @}

#endif  // _FOG_CORE_TIMER_H
