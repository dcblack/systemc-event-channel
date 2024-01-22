#include "event.hpp"

void Event_channel::notify(const sc_core::sc_time& delay)
{
  m_event.notify(delay);
}

void Event_channel::notify()
{
  m_event.notify();
}

const sc_core::sc_event& Event_channel::event() const
{
  return m_event;
}

