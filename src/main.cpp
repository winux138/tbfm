#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <ranges>
// #include <string>
#include <vector>

#include <ncurses.h>

class FolderView
{
  private:
    int m_cursor{};
    std::filesystem::path m_path{};
    std::vector<std::filesystem::path> m_path_subdirs{};
    std::vector<std::filesystem::path> m_path_files{};
    // TODO ncurses stuff

    void
    print_direntry (const char *entry_name, const int row)
    {
        std::cout << "row: " << row << "cursor" << m_cursor << std::endl;
        if (row == m_cursor)
            attron (A_REVERSE);
        mvprintw (row, 2, "%s", entry_name);
        if (row == m_cursor)
            attroff (A_REVERSE);
    }

  public:
    FolderView ()
        : m_cursor{ 2 }, m_path{ std::filesystem::current_path () },
          m_path_subdirs{}, m_path_files{}
    {
        m_path_subdirs
            = std::filesystem::directory_iterator (m_path)
              | std::views::filter ([] (auto e) { return e.is_directory (); })
              | std::views::transform ([] (auto e) { return e.path (); })
              | std::ranges::to<std::vector> ();
        m_path_files
            = std::filesystem::directory_iterator (m_path)
              | std::views::filter (
                  [] (auto e) { return e.is_regular_file (); })
              | std::views::transform ([] (auto e) { return e.path (); })
              | std::ranges::to<std::vector> ();
    }

    void
    move_up (void)
    {
        --m_cursor;
        if (m_cursor < 2)
            m_cursor = 2;
    }

    void
    move_down (void)
    {
        ++m_cursor;
        // TODO: Do something about warning
        if (m_cursor > 2 + m_path_subdirs.size () + m_path_files.size ())
            m_cursor = 2 + m_path_subdirs.size () + m_path_files.size ();
    }

    void
    display ()
    {
        clear ();
        printw ("Welcome to TBFM!\n");
        int row{ 2 };
        printw ("%s:\n", m_path.c_str ());

        attron (COLOR_PAIR (1));
        print_direntry ("..", row++);
        for (const auto &subdirectory :
             std::filesystem::directory_iterator (m_path)
                 | std::views::filter (
                     [] (const auto &e) { return e.is_directory (); })
                 | std::views::transform (
                     [] (const auto &e) { return e.path (); }))
            print_direntry (subdirectory.filename ().c_str (), row++);
        attroff (COLOR_PAIR (1));

        for (const auto &file :
             std::filesystem::directory_iterator (m_path)
                 | std::views::filter (
                     [] (const auto &e) { return e.is_regular_file (); })
                 | std::views::transform (
                     [] (const auto &e) { return e.path (); }))
            print_direntry (file.filename ().c_str (), row++);

        // TODO: Footer bar with keymaps

        refresh ();
    }
};

int
main (int argc, char **argv)
{
    (void)argc;
    (void)argv;

    initscr ();
    raw ();
    noecho ();

    assert (
        has_colors () && start_color () == OK && "Colors are not supported");
    use_default_colors (); // allow -1 background
    // pair id, foreground, background (-1 = default)
    init_pair (1, COLOR_BLUE, -1);

    curs_set (0);

    FolderView folder_view{};
    bool should_exit{ false };
    while (!should_exit) {
        folder_view.display ();

        switch (getch ()) {
        case 'k':
            folder_view.move_up ();
            break;
        case 'j':
            folder_view.move_down ();
            break;
        case 'q':
            should_exit = true;
            break;
        }
    }

    endwin ();

    return 0;
}
