#include <exception>
#include <iostream>


class Logger {
public:
    explicit Logger(const std::string& logPath = "tmp/oodc-watchdog.log") : logPath_(logPath) {}

	void info(const std::string& msg) { log_("[INFO] " + msg); }
	void warn(const std::string& msg) { log_("[WARN] " + msg); }
	void check(const std::string& msg) { log_("[CHECK] " + msg); }
	void action(const std::string& msg) { log_("[ACTION] " + msg); }
	void result(const std::string& msg) { log_("[RESULT] " + msg); }
	void error(const std::string& msg) { log_("[ERROR] " + msg); }

private:
    std::string logPath_;
    void log_(const std::string& msg) {
        // For simplicity, we print to console. In a real application, this would write to a file.
        std::cout << msg << std::endl;
    }
};


class WatchdogApplication {

public:
    int run(int argc, char *argv[]){
        try{
            parseArgs(argc, argv);
            printHeader();
            printDefaultTarget();
            execute();
            logger_.info("Watchdog application finished successfully.");
            return 0;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
        return 1;
    }

private:

    static constexpr int kCheckCount = 10;
    Logger logger_;

    void parseArgs(int argc, char *argv[]);
    void printHeader();
    void printDefaultTarget();
    void execute();
};

int main(int argc, char *argv[]){
    WatchdogApplication app;
    return app.run(argc, argv);
}