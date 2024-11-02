# KIM: Kilo IMproved

### Description
- KIM (Kilo IMproved) is a vastly improved version of Kilo, a text editor for UNIX systems.
- See `doc/Kim_Diff.txt` for differences with Kilo. 

### Key Bindings
- `Arrow Keys`: Move around in rows/cols
- `Ctrl + n`: Create a new file
- `Ctrl + o`: Open a file (existing or create new)
- `Ctrl + u`: Change / disable syntax highlighting
- `Ctrl + q`: Quit, pressed twice to discard unsaved changes
- `Ctrl + s`: Save, creates new file if not existent (may prompt to name it too)
- `Ctrl + a`: Save as (always prompts where and what to save as)
- `Home (pos1)`: Go to start of line
- `End`: Go to end end of line
- `Ctrl + j`: Jump to line
- `Ctrl + f`: Search in file (ESC to cancel, ENTER to continue editing at the current result)
- `Ctrl + r`: Search and replace
- `Ctrl + d`: Delete line
- `Ctrl + k`: Delete N lines
- `Ctrl + h / Del / Backspace`: Delete character
- `Ctrl + c`: Copy N lines
- `Ctrl + p`: Paste 1 line (or insert into another line)
- `Ctrl + v`: Paste block of N lines, starting at the current line index
- `Page Up`: Scroll up
- `Page Down`: Scroll down

### Mouse Bindings
- The Suckless Terminal (ST) may emit `^E` and `^Y` control characters when scrolling with the mouse, KIM can use these to scroll
- A patch also exists to enable KIM to make use of xterm mouse tracking, however, it is not fully functional as of now
- Furthermore, gpm mouse tracking support by using libgpm-dev may be added in the future
- Most importantly, the ".kimrc" configuration file features an option to traverse multiple lines per scroll action

### Project Structure 
| Dir          | Description              |
|--------------|--------------------------|
| doc          | Documentation            |
| pkg          | Packaging-related        |
| src          | Source code              |

### Porting
- Porting KIM to other platforms should generally be rather simple, for example there's an unreleased windows port
- KIM also does not necessarily depend on the GNU C Library (glibc), the musl libc (musl-libc) has been proven to work just as well

### Debugging
- GDB:
    - Compile with debug symbols, "gcc -g ...": `gcc -g *.c -o kim`
    - Coredumps: `ulimit -c unlimited`
    - Make the editor segfault (...)
    - Coredump location: e.g. `/var/lib/systemd/coredump` (Mint) or `/var/lib/apport/coredump` (Ubuntu)
    - Move the coredump to a better location: `sudo mv -f /var/lib/systemd/coredump/core.kim.* ./`
    - Extract the .zst archive: `zstd -d core.kim.*`
    - If needed, change ownership: `sudo chown main:main core.kim.*`
    - Run `gdb path/to/the/binary path/to/the/core/dump/file` (e.g. `gdb ./kim core.kim.*`)
    - Full back trace (in gdb): `bt full` (or: `bt`)
- Valgrind:
    - To check for memory leaks: `valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./kim test_file.c`

