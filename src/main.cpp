#include <cassert>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include <ncurses.h>

int cursor{ 2 };
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

void
print_direntry (const char *entry_name, const int row)
{
    if (row == cursor)
        attron (A_REVERSE);
    mvprintw (row, 2, "%s", entry_name);
    if (row == cursor)
        attroff (A_REVERSE);
}

int
main (int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int ch{ 0 };
    initscr ();
    raw ();
    // noecho ();

    assert (
        has_colors () && start_color () == OK && "Colors are not supported");
    use_default_colors (); // allow -1 background
    // pair id, foreground, background (-1 = default)
    init_pair (1, COLOR_BLUE, -1);

    curs_set (0);

    bool should_exit{ false };
    while (!should_exit) {
        clear ();
        printw ("Welcome to TBFM!\n");

        // TODO: Header bar with info
        // TODO: Footer bar with keymaps

        int row{ 2 };
        printw ("%s:\n", path.c_str ());

        attron (COLOR_PAIR (1));
        print_direntry ("..", row++);
        for (const auto &subdirectory : get_subdirectories ())
            print_direntry (subdirectory.filename ().c_str (), row++);
        attroff (COLOR_PAIR (1));

        for (const auto &file : get_files ())
            print_direntry (file.filename ().c_str (), row++);

        refresh ();
        switch (getch ()) {
        case 'k':
            --cursor;
            break;
        case 'j':
            ++cursor;
            break;
        case 'q':
            should_exit = true;
            break;
        }
    }

    endwin ();

    return 0;
}
