#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

auto path
    = std::filesystem::absolute (std::filesystem::path{ "." }).parent_path ();
std::vector<std::filesystem::path> sub_directories{};

int
main (int argc, char **argv)
{
    std::cout << "Welcome to TBFM!" << std::endl;

    bool exit{ false };
    while (!exit) {
        std::cout << std::endl;
        std::cout << "--- files ---" << std::endl;

        sub_directories.clear ();
        for (const auto &file : std::filesystem::directory_iterator (path))
            if (file.is_directory ()) {
                sub_directories.push_back (file.path ());
                std::cout << "dir:  " << file << std::endl;
            }
        for (auto &file : std::filesystem::directory_iterator (path))
            if (file.is_regular_file ())
                std::cout << "file: " << file << std::endl;

        std::cout << std::endl;
        std::cout << "--- actions ---" << std::endl;
        std::cout << "1. go up a diretory" << std::endl;
        std::cout << "2. navigate to a directory" << std::endl;
        std::cout << "3. print the full path of a file and exit" << std::endl;
        std::cout << "0. exit" << std::endl;

        // Prompt for action
        int raw_action{};
        std::cin >> raw_action;

        switch (raw_action) {
        case 0:
            exit = true;
            break;
        case 1:
            path = path.parent_path ();
            break;
        case 2: {
            std::cout << std::endl;
            std::cout << "--- Pick a directory to move to ---" << std::endl;
            for (auto const [index, sub_directory] :
                 sub_directories | std::views::enumerate
                     | std::views::transform ([] (auto p) {
                           return std::pair{ std::get<0> (p) + 1,
                                             std::get<1> (p) };
                       }))
                std::cout << index << ": " << sub_directory << std::endl;

            int diretory_to_move_to{};
            std::cin >> diretory_to_move_to;
            --diretory_to_move_to;
            assert (
                diretory_to_move_to < sub_directories.size ()
                && "Sub directory index out of range");
            assert (
                diretory_to_move_to >= 0
                && "Sub directory index cannot be below 0");
            path = sub_directories[diretory_to_move_to];
            break;
        }
        default:
            std::cout << "Invalid input" << std::endl;
            break;
        }
    }
}
