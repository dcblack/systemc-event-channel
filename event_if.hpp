#include <systemc>

struct Event_notify_if
  : virtual sc_core::sc_interface
{
  virtual void notify() = 0;
  virtual void notify(const sc_core::sc_time& delay) = 0;
};

struct Event_view_if
  : virtual sc_core::sc_interface
{
  virtual const sc_core::sc_event& event() const = 0;
};

struct Event_if : virtual Event_notify_if, virtual Event_view_if
{};
