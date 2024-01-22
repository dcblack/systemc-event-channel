#include "event_if.hpp"
#include <systemc>
#include <string>

struct Event_channel
  : Event_if, sc_core::sc_prim_channel
{
    explicit Event_channel(const std::string& instance)
      : sc_prim_channel(instance.c_str())
      {}
    void notify() override;
    void notify(const sc_core::sc_time& delay) override;
    const sc_core::sc_event& event() const override;
    // Allow static port sensitivity
    const sc_core::sc_event& default_event() const override { return event(); }
    // Convenience
    virtual const sc_core::sc_event& operator()() const { return event(); }
private:
    sc_core::sc_event m_event;
};
