#include "main.h"

#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <sys/wait.h>

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \n\r\t");
    if (start == std::string::npos) return "";

    size_t end = s.find_last_not_of(" \n\r\t");
    return s.substr(start, end - start + 1);
}

static bool startsWith(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

static bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size()
        && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static bool isSafeServiceName(const std::string& s) {
    for (char ch : s) {
        bool ok =
            ('a' <= ch && ch <= 'z') ||
            ('A' <= ch && ch <= 'Z') ||
            ('0' <= ch && ch <= '9') ||
            ch == '-' ||
            ch == '_' ||
            ch == '.' ||
            ch == '@';

        if (!ok) return false;
    }

    return true;
}

static int parseInterval(const std::string& s) {
    try {
        size_t pos = 0;
        int value = std::stoi(s, &pos);

        if (pos != s.size()) {
            throw std::runtime_error("invalid interval-sec");
        }

        return value;
    } catch (...) {
        throw std::runtime_error("invalid interval-sec");
    }
}

Logger::Logger(const std::string& logPath)
    : logPath_(logPath) {}

void Logger::info(const std::string& msg) {
    log("[INFO] " + msg);
}

void Logger::warn(const std::string& msg) {
    log("[WARN] " + msg);
}

void Logger::check(const std::string& msg) {
    log("[CHECK] " + msg);
}

void Logger::action(const std::string& msg) {
    log("[ACTION] " + msg);
}

void Logger::result(const std::string& msg) {
    log("[RESULT] " + msg);
}

void Logger::error(const std::string& msg) {
    log("[ERROR] " + msg);
}

void Logger::log(const std::string& msg) {
    std::cout << msg << std::endl;

    std::ofstream ofs(logPath_, std::ios::app);
    if (ofs.is_open()) {
        ofs << msg << std::endl;
    }
}

CommandRunner::CommandResult CommandRunner::run(const std::string& command) {
    std::string fullCommand = command + " 2>&1";

    std::array<char, 256> buffer{};
    std::string output;

    FILE* pipe = popen(fullCommand.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("failed to execute command: " + command);
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        output += buffer.data();
    }

    int status = pclose(pipe);

    int exitCode = -1;
    if (status != -1) {
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        } else {
            exitCode = status;
        }
    }

    return {exitCode, trim(output)};
}

SystemdService::SystemdService(std::string name)
    : name_(std::move(name)) {}

const std::string& SystemdService::name() const {
    return name_;
}

SystemdService::State SystemdService::queryState() const {
    auto result = CommandRunner::run("systemctl is-active " + name_);

    if (result.output == "active") {
        return State::Active;
    }

    if (result.output == "inactive") {
        return State::Inactive;
    }

    if (result.output == "failed") {
        return State::Failed;
    }

    return State::Unknown;
}

bool SystemdService::restart() const {
    auto result = CommandRunner::run("systemctl restart " + name_);
    return result.exitCode == 0;
}


int WatchdogApplication::run(int argc, char* argv[]) {
    try {
        parseArgs(argc, argv);
        printHeader();
        printDefaultTarget();
        execute();

        logger_.info("watchdog finished");
        return 0;
    } catch (const std::exception& e) {
        logger_.error(e.what());
        return 1;
    }
}

void WatchdogApplication::parseArgs(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::runtime_error("usage: ./main [--dry-run] <service-name> <interval-sec>");
    }

    int index = 1;

    if (std::string(argv[index]) == "--dry-run") {
        dryRun_ = true;
        ++index;
    }

    if (argc - index < 2) {
        throw std::runtime_error("usage: ./main [--dry-run] <service-name> <interval-sec>");
    }

    serviceName_ = argv[index++];
    intervalSec_ = parseInterval(argv[index]);

    if (intervalSec_ < 1) {
        throw std::runtime_error("interval-sec must be at least 1");
    }

    if (!endsWith(serviceName_, ".service")) {
        throw std::runtime_error("service-name must end with .service");
    }

    if (!isSafeServiceName(serviceName_)) {
        throw std::runtime_error("service-name contains unsafe characters");
    }
}

void WatchdogApplication::printHeader() {
    logger_.info("target service: " + serviceName_);
    logger_.info("interval: " + std::to_string(intervalSec_) + " sec");

    if (dryRun_) {
        logger_.info("dry-run: enabled");
    }
}

void WatchdogApplication::printDefaultTarget() {
    auto result = CommandRunner::run("systemctl get-default");

    if (result.exitCode == 0) {
        logger_.info("default target: " + result.output);
    } else {
        logger_.warn("failed to get default target: " + result.output);
    }
}

void WatchdogApplication::execute() {
    SystemdService service(serviceName_);

    for (int i = 1; i <= kCheckCount; ++i) {
        logger_.info("check count: " + std::to_string(i) + "/" + std::to_string(kCheckCount));

        auto state = service.queryState();
        handleState(service, state);

        if (i < kCheckCount) {
            std::this_thread::sleep_for(std::chrono::seconds(intervalSec_));
        }
    }
}

void WatchdogApplication::handleState(const SystemdService& service, SystemdService::State state) {
    switch (state) {
        case SystemdService::State::Active:
            logger_.check(service.name() + " is active");
            break;

        case SystemdService::State::Inactive:
            logger_.check(service.name() + " is inactive");
            restartIfAllowed(service);
            break;

        case SystemdService::State::Failed:
            logger_.check(service.name() + " is failed");
            restartIfAllowed(service);
            break;

        case SystemdService::State::Unknown:
            logger_.check(service.name() + " is unknown");
            break;
    }
}

void WatchdogApplication::restartIfAllowed(const SystemdService& service) {
    if (!startsWith(service.name(), "oodc-")) {
        logger_.warn("restart skipped: service name must start with oodc-");
        return;
    }

    logger_.action("restart " + service.name());

    if (dryRun_) {
        logger_.result("restart skipped (dry-run)");
        return;
    }

    bool ok = service.restart();

    if (ok) {
        logger_.result("restart success");
    } else {
        logger_.result("restart failed");
    }
}


int main(int argc, char* argv[]) {
    WatchdogApplication app;
    return app.run(argc, argv);
}