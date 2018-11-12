#include "Config.h"
#include "Network.h"
#include "SysLog.h"
#include "StatusIndicator.h"

Config config("rclc");
Network network(config);

void setup() {
  Serial.begin(115200);
  Serial.println("\nRemote Controlled Lego Car (RCLC) firmware 1.0");
  config.begin();
  Status.begin();
  network.enableDebug();
  network.begin();
  SysLog.begin(
    config.getSyslogHost(), config.getSyslogPort(),
    config.getAppName(), config.getHardwareId());
}

void loop() {
  network.run();
}
