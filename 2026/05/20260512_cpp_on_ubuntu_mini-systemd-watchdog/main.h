#ifndef MAIN_H
#define MAIN_H

#include <string>


class Logger {
public:
    explicit Logger(const std::string& logPath = "/tmp/oodc-watchdog.log");

    void info(const std::string& msg);
    void warn(const std::string& msg);
    void check(const std::string& msg);
    void action(const std::string& msg);
    void result(const std::string& msg);
    void error(const std::string& msg);

private:
    std::string logPath_;

    void log(const std::string& msg);
};

class CommandRunner {
public:
    struct CommandResult {
        int exitCode;
        std::string output;
    };

    static CommandResult run(const std::string& command);
};

class SystemdService {
public:
    enum class State {
        Active,
        Inactive,
        Failed,
        Unknown
    };

    explicit SystemdService(std::string name);

    const std::string& name() const;
    State queryState() const;

    // 修正: restart は成功/失敗を返す
    bool restart() const;

private:
    std::string name_;
};

class WatchdogApplication {
public:
    int run(int argc, char* argv[]);

private:
    static constexpr int kCheckCount = 10;

    Logger logger_;
    std::string serviceName_;
    int intervalSec_ = 0;
    bool dryRun_ = false;

    void parseArgs(int argc, char* argv[]);
    void printHeader();
    void printDefaultTarget();
    void execute();
    void handleState(const SystemdService& service, SystemdService::State state);
    void restartIfAllowed(const SystemdService& service);
};

#endif