enum State {
  WAIT_CONFIG,
  CONFIGURING,
  RUNNING,
  RESET_CONFIG,
  ERROR,

  MAX_CONFIG_VALUE
};

#if defined(APP_DEBUG)
const char* StateStr[MAX_CONFIG_VALUE] = {
  "WAIT_CONFIG",
  "CONFIGURING",
  "RUNNING",
  "RESET_CONFIG",
  "ERROR"
};
#endif

namespace DeviceState {
  volatile State state;

  State get()        { return state; }
  bool  is (State m) { return (state == m); }
  void  set(State m);
};
