#include <cassert>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include <ncurses.h>

auto path{ std::filesystem::current_path () };
std::vector<std::filesystem::path> sub_directories{};

std::vector<std::filesystem::path>
get_subdirectories (void)
{
    std::vector<std::filesystem::path> subdirectories{};
    for (const auto &file : std::filesystem::directory_iterator (path))
        if (file.is_directory ())
            subdirectories.push_back (file.path ());
    return subdirectories;
}

std::vector<std::filesystem::path>
get_files (void)
{
    std::vector<std::filesystem::path> files{};
    for (const auto &file : std::filesystem::directory_iterator (path))
        if (file.is_regular_file ())
            files.push_back (file.path ());
    return files;
}

bool
prompt_user_action (void)
{

    printw ("--- actions ---\n");
    printw ("1. go up a diretory\n");
    printw ("2. navigate to a directory\n");
    printw ("3. print the full path of a file and exit\n");
    printw ("0. exit\n");

    switch (getch () - '0') {
    case 0:
        return true;
    case 1:
        path = path.parent_path ();
        break;
    case 2: {
        printw ("--- Pick a directory to move to ---");
        for (auto const [index, sub_directory] :
             sub_directories | std::views::enumerate
                 | std::views::transform ([] (auto p) {
                       return std::pair{ std::get<0> (p) + 1,
                                         std::get<1> (p) };
                   }))
            printw ("%d: %s", index, sub_directory.c_str ());

        unsigned int diretory_to_move_to{};
        std::cin >> diretory_to_move_to;
        --diretory_to_move_to;
        assert (
            diretory_to_move_to < sub_directories.size ()
            && "Sub directory index out of range");
        path = sub_directories[diretory_to_move_to];
        break;
    }
    default:
        std::cout << "Invalid input" << std::endl;
        break;
    }

    return false;
}

int
main (int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int ch{ 0 };
    initscr ();
    raw ();
    noecho ();

    bool should_exit{ false };
    while (!should_exit) {
        clear ();
        printw ("Welcome to TBFM!\n");

        // TODO: Header bar with info
        // TODO: Footer bar with keymaps

        printw ("%s:\n", path.c_str ());

        // TODO color on
        for (const auto &subdirectory : get_subdirectories ())
            printw ("  %s\n", subdirectory.filename ().c_str ());
        // TODO color off
        for (const auto &file : get_files ())
            printw ("  %s\n", file.filename ().c_str ());

        // should_exit = prompt_user_action ();
        refresh ();
        getch ();
        break;
    }

    endwin ();

    return 0;
}
