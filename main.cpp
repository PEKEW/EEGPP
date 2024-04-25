#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <algorithm>
#include <fstream>

//todo
/*

将命令处理逻辑与仓库管理逻辑分开。可以创建一个类来处理命令行接口（CLI）相关的功能，如解析命令和显示帮助，而 `EEGRepo` 类专注于管理仓库。
引入服务层来处理业务逻辑，比如处理具体的文件操作和版本控制逻辑。
考虑使用更多的函数式编程元素，如使用函数对象、更多的 STL 算法等，来提高代码的表达力和减少冗余。
对 EEG 数据进行压缩存储可以显著减少存储需求。可以使用如 zlib 或 lzma 等库来实现。
 `init` 函数中，如果仓库已存在，应该给用户一个明确的警告，可能通过命令行参数来允许覆盖。添加一个 `-f` 或 `--force` 选项
使用智能指针和 RAII（Resource Acquisition Is Initialization）技术来管理资源
 */

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
            std::filesystem::create_directories(".eegv"); // 存储关于版本控制系统配置的信息，例如用户设置和系统行为配置。
            std::filesystem::create_directories("eegobjs"); // 用于存储所有EEG数据文件的对象，可能是按哈希值命名的文件，这些对象代表不同版本的数据文件。
            std::filesystem::create_directories("refs"); // 用于存储引用到各个EEG数据文件的最新或特定版本的指针，类似于Git中的分支引用。
            // 创建配置文件 包含电极位置 等
            std::ofstream configFile(".eegv/config"); // 配置文件
            /*
             * head存储的是当前操作的文件引用 这一点和git的分离head有点类似
             * 但是git的head是指向分支的指针
             * 而这里的head是指向文件的指针 对应的是eegobjs文件夹中的文件名(hash) 后续可能有一张hash和eeg文件名对照表
             * head永远指向现在操作的文件的hash，当切换eeg数据文件时，通过refs文件夹中的内容来辅助，指针的逻辑可能是这样的:
             * // 切换操作文件的逻辑伪代码示例
                    current_hash = read from HEAD
                    if refs exists for current_hash:
                        new_hash = find corresponding file in refs
                    if new_hash exists:
                        update HEAD to new_hash
                    open corresponding EEG file
                    update HEAD file
             * 这就可能需要*.eegpp文件中保存更多的内容，比如eeg具体的文件名 但可以不保存电极位置之类的信息
             */
            std::ofstream headFile(".eegv/HEAD");
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
