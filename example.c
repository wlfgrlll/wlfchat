#include "termbox2.h"

int main(int argc, char **argv) {
    // The tb_event struct stores information about the event that occurred
    // (e.g., key presses, mouse clicks, and terminal resizes).
    struct tb_event ev;

    // tb_init() must be called to initialize the terminal into "TUI mode"
    // before any other termbox functions are used.
    // It returns 0 (TB_OK) on success.
    if (tb_init() != 0) {
        return -1;
    }

    // tb_clear() clears the internal back buffer.
    // By default, it uses the terminal's default foreground/background colors.
    tb_clear();

    // The coordinate system uses (0,0) as the top-left cell of the terminal.
    // tb_printf is a helpful wrapper to format text at (x, y) with specified foreground and background colors.
    // Colors can be combined with attributes like TB_BOLD or TB_UNDERLINE using bitwise OR (|).
    tb_printf(2, 1, TB_GREEN | TB_BOLD, TB_DEFAULT, "Welcome to the termbox2 example!");
    tb_printf(2, 3, TB_WHITE, TB_DEFAULT, "Terminal size: %d columns x %d rows", tb_width(), tb_height());
    tb_printf(2, 5, TB_YELLOW, TB_DEFAULT, "Press any arrow key or a regular character to inspect the event...");
    tb_printf(2, 6, TB_RED, TB_DEFAULT, "Press 'q' or ESC to quit.");

    // tb_present() syncs the internal back buffer with the physical terminal.
    // You MUST call this after drawing to actually see any changes on the screen.
    tb_present();

    // This is the main application event loop
    while (1) {
        // tb_poll_event blocks until an event occurs (like a keypress or a terminal resize)
        if (tb_poll_event(&ev) == 0) {

            // Handle Keyboard Input
            if (ev.type == TB_EVENT_KEY) {
                // If the user pressed 'q' or ESC, break out of the loop to exit
                if (ev.ch == 'q' || ev.key == TB_KEY_ESC) {
                    break;
                }

                // Clear the internal buffer before drawing the new frame
                tb_clear();

                // Redraw our main instructions
                tb_printf(2, 1, TB_GREEN | TB_BOLD, TB_DEFAULT, "Welcome to the termbox2 example!");
                tb_printf(2, 3, TB_WHITE, TB_DEFAULT, "Terminal size: %d columns x %d rows", tb_width(), tb_height());
                tb_printf(2, 5, TB_YELLOW, TB_DEFAULT, "Press any arrow key or a regular character to inspect the event...");
                tb_printf(2, 6, TB_RED, TB_DEFAULT, "Press 'q' or ESC to quit.");

                // Print the details of the key that was pressed.
                // - ev.key represents special keys (e.g. TB_KEY_ENTER, TB_KEY_ARROW_UP, TB_KEY_SPACE)
                // - ev.ch represents regular unicode characters (e.g. 'a', 'b', '1', '2')
                tb_printf(2, 8, TB_CYAN, TB_DEFAULT, "Last Key Event:");
                tb_printf(4, 9,  TB_DEFAULT, TB_DEFAULT, "Type: TB_EVENT_KEY");
                tb_printf(4, 10, TB_DEFAULT, TB_DEFAULT, "Key:  %d", ev.key);
                tb_printf(4, 11, TB_DEFAULT, TB_DEFAULT, "Char: %c (Unicode code: %d)", ev.ch ? ev.ch : ' ', ev.ch);
                tb_printf(4, 12, TB_DEFAULT, TB_DEFAULT, "Mod:  %d", ev.mod);

                // Push the new frame to the terminal
                tb_present();
            }
            // Handle Terminal Resizes
            else if (ev.type == TB_EVENT_RESIZE) {
                tb_clear();
                tb_printf(2, 1, TB_GREEN | TB_BOLD, TB_DEFAULT, "Terminal Resized!");

                // During a resize event, ev.w and ev.h hold the new dimensions
                tb_printf(2, 3, TB_WHITE, TB_DEFAULT, "New Terminal size: %d columns x %d rows", ev.w, ev.h);
                tb_printf(2, 5, TB_RED, TB_DEFAULT, "Press 'q' or ESC to quit.");
                tb_present();
            }
        }
    }

    // tb_shutdown() restores the terminal back to its original normal state.
    // This MUST be called before the program exits, otherwise the terminal will be left in a broken/unusable visual state.
    tb_shutdown();

    return 0;
}