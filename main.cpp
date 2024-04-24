#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <algorithm>

//todo
// !info 这是一个快速搓的demo 后续的优化方向:
// 1. 代码组织 抽象 封装和解耦
// 2. 函数式编程
// 3. 文件压缩
// 4. todo

namespace eegpp {

    class EEGRepo {
    public:
        // 初始化仓库
        /**
            * @brief Initializes a new EEG repository or re-initialize an existing one.
            *
            * This function sets up the necessary directory structure and initialization files
            * required for a new EEG version control repository. It creates directories for
            * storing version control objects and references, and prepares a HEAD file to keep
            * track of the current branch. This setup is essential for the functioning of the
            * EEG version control system.
            *
            * @param eegRawDataName The name of the EEG file that will be tracked. This file
            *        must exist in the working directory and should have a valid EEG file
            *        extension (.set, .eegpp, .xdf) to be considered valid.
            * @throws std::invalid_argument If the `eegRawDataName` is empty or does not
            *         contain a valid extension, this exception is thrown indicating that
            *         the initialization cannot proceed.
            *
            * Example usage:
            *     EEGPP > init EEG_RAW_DATA.xdf  // Initializes a repository with example.set as the initial tracked file.
            *
            * Note: If the repository already exists, this function will reinitialize all settings,
            *       potentially overwriting existing configurations. Always ensure that reinitialization
            *       is intended to avoid data loss.
        */
        static void init(const std::string& eegRawDataName) {
            if (!isValidFilename(eegRawDataName)) {
                throw std::invalid_argument("Invalid EEG file format.");
            }
            // todo 创建文件结构
            // todo 如果文件存在应该给出警告 进一步问询是否覆盖
            std::filesystem::create_directories(".eegv");
            std::filesystem::create_directories("eegobjs");
            std::filesystem::create_directories("refs");
            std::cout << "Repository initialized." << std::endl;
        }

        // todo 其他功能实现...

    private:
        static bool isValidFilename(const std::string& name) {
            const std::vector<std::string> validExtensions = {".set", ".eegpp", ".xdf"};
            return std::any_of(validExtensions.begin(), validExtensions.end(),
                               [&name](const std::string& ext) { return name.find(ext) != std::string::npos; });
        }
    };

    void showHelp() {
        std::cout << "EEGPP is a simple version control system for EEG data.\n"
                  << "Usage: eegpp <command> [<args>]\n"
                  << "The most commonly used eegpp commands are:\n"
                  << "   init        Create an empty eegpp repository or reinitialize an existing one with a EEG file\n";
    }

    std::vector<std::string> parseCommand(const std::string& input) {
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

} // namespace eegpp

int main() {
    using namespace eegpp;
    std::string input;
    std::cout << "EEGPP started. Type 'help' for a list of commands." << std::endl;

    while (true) {
        std::cout << "EEGPP > ";
        std::getline(std::cin, input);

        try {
            std::vector<std::string> commands = parseCommand(input);

            if (commands.empty()) continue;

            if (commands[0] == "init" && commands.size() == 2) {
                EEGRepo::init(commands[1]);
            } else if (commands[0] == "help") {
                showHelp();
            } else if (commands[0] == "quit") {
                std::cout << "Exiting EEGPP." << std::endl;
                break;
            } else {
                std::cout << "Unknown command or incorrect parameters. Type 'help' for a list of commands." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}
