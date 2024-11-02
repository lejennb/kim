// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "row.h"

int SCROLL_STEP = SCROLL_STEP_DEFAULT;
int TAB_SPACE   = TAB_SPACE_DEFAULT;
int TAB_INDENT  = TAB_INDENT_DEFAULT;
int COLOR       = COLOR_DEFAULT;

void getRcConfig() {
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  if ((file = fopen(RC_CONFIG, "r")) == NULL) {
    return;  // Just use defaults
  }

  while ((read = getline(&line, &len, file)) != -1) {
    sscanf(line, "SCROLL_STEP = %d", &SCROLL_STEP);
    sscanf(line, "TAB_SPACE = %d", &TAB_SPACE);
    sscanf(line, "TAB_INDENT = %d", &TAB_INDENT);
    sscanf(line, "COLOR = %d", &COLOR);
  }

  fclose(file);
  if (line) { free(line); }
}

void checkRcConfig() {  // Check if user settings are within sane limits
  if (SCROLL_STEP < 1 || SCROLL_STEP > 256) {
    SCROLL_STEP = SCROLL_STEP_DEFAULT;
  }

  if (TAB_SPACE < 0 || TAB_SPACE > 1) {
    TAB_SPACE = TAB_SPACE_DEFAULT;
  }

  if (TAB_INDENT < 2 || TAB_INDENT > 8) {
    TAB_INDENT = TAB_INDENT_DEFAULT;
  }

  if (COLOR < 8 || COLOR > 256) {
    COLOR = COLOR_DEFAULT;
  }
}

