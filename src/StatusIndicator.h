#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

class StatusIndicator {
  public:
    void begin();
    void on();
    void off();
    void toggle();
    bool state();
};

extern StatusIndicator Status;

#endif
