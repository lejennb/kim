// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#ifndef TERMINAL
#define TERMINAL

void die(const char *s);
void disableRawMode();
void enableRawMode();
int getCursorPosition(int *rows, int *cols);
int getWindowSize(int *rows, int *cols);
void updateWindowSize(void);
void handleSigwinch(int sig_num);

#endif // TERMINAL

