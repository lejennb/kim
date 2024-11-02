// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// Debian/Ubuntu need libc-dev / libc6-dev
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "copy.h"
#include "find.h"
#include "input.h"
#include "kim.h"
#include "ops.h"
#include "output.h"
#include "replace.h"
#include "row.h"
#include "syntax.h"
#include "terminal.h"

void getArgs(int argc, char *argv[]) {
  if (argc >= 2) {
    if ((strcmp(argv[1], "-V") == 0) || (strcmp(argv[1], "--version") == 0)) {
      printf("KIM - Kilo IMproved\nVersion %s\nCopyright (C) 2024 LJ\n", REL_VERSION);
      exit(0);
    }
  }
}

void initEditor() {
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.rowoff = 0;
  E.coloff = 0;
  E.numrows = 0;
  E.row = NULL;
  E.dirty = 0;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  E.hl_queue = 0;
  E.hl_queue_pos = 0;
  E.hl_queue_time = 0;
  E.syntax = NULL;

  updateWindowSize();
  signal(SIGWINCH, handleSigwinch);
}

int main(int argc, char *argv[]) {
  getArgs(argc, argv);

  getRcConfig();
  checkRcConfig();
  enableRawMode();
  initEditor();
  initClipboard();

  if (argc >= 2) {
    editorOpen(argv[1]);
  }

  editorSetStatusMessage("KIM - version %s", REL_VERSION);

  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
    editorSyntaxQueue();
  }

  return 0;
}

