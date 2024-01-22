#include <systemc>

struct Producer_module;
struct Consumer_module;
struct Event_channel;

SC_MODULE( Top_module )
{
  explicit Top_module(const sc_core::sc_module_name& instance);
  ~Top_module() override;
  Producer_module* producer{nullptr};
  Consumer_module* consumer1{nullptr};
  Consumer_module* consumer2{nullptr};
  Event_channel*   event_chan{nullptr};
};