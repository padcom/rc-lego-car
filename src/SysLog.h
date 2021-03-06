#ifndef SYSLOG_H
#define SYSLOG_H

#include <WiFiUdp.h>

/* priorities */
#define LOG_EMERG           0 /* system is unusable */
#define LOG_ALERT           1 /* action must be taken immediately */
#define LOG_CRIT            2 /* critical conditions */
#define LOG_ERR             3 /* error conditions */
#define LOG_WARNING         4 /* warning conditions */
#define LOG_NOTICE          5 /* normal but significant condition */
#define LOG_INFO            6 /* informational */
#define LOG_DEBUG           7 /* debug-level messages */

/* facility codes */
#define LOG_KERN        (0<<3) /* kernel messages */
#define LOG_USER        (1<<3) /* random user-level messages */
#define LOG_MAIL        (2<<3) /* mail system */
#define LOG_DAEMON      (3<<3) /* system daemons */
#define LOG_AUTH        (4<<3) /* security/authorization messages */
#define LOG_SYSLOG      (5<<3) /* messages generated internally by syslogd */
#define LOG_LPR         (6<<3) /* line printer subsystem */
#define LOG_NEWS        (7<<3) /* network news subsystem */
#define LOG_UUCP        (8<<3) /* UUCP subsystem */
#define LOG_CRON        (9<<3) /* clock daemon */
#define LOG_AUTHPRIV   (10<<3) /* security/authorization messages (private) */
#define LOG_FTP        (11<<3) /* ftp daemon */
/* codes through 15 are reserved for system use */
#define LOG_LOCAL0     (16<<3) /* reserved for local use */
#define LOG_LOCAL1     (17<<3) /* reserved for local use */
#define LOG_LOCAL2     (18<<3) /* reserved for local use */
#define LOG_LOCAL3     (19<<3) /* reserved for local use */
#define LOG_LOCAL4     (20<<3) /* reserved for local use */
#define LOG_LOCAL5     (21<<3) /* reserved for local use */
#define LOG_LOCAL6     (22<<3) /* reserved for local use */
#define LOG_LOCAL7     (23<<3) /* reserved for local use */

class SysLogClient {
  public:
    void begin(IPAddress host, uint16_t port, String app, String source);
    void debug(const String &message);
    void debugf(const String &message, ...);
    void log(const String &message);
    void logf(const String &message, ...);
    void log(const uint16 level, const String &message);
    void logf(const uint16 level, const String &message, ...);
    void log(const uint16 facility, const uint16 level, const String &message);
    void logf(const uint16 facility, const uint16 level, const String &message, ...);
  private:
    WiFiUDP udp;
    bool initialized;
    IPAddress host;
    uint16_t port;
    String app;
    String source;
};

extern SysLogClient SysLog;

#endif
