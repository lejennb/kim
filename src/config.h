// Copyright (C) 2024 lejennb and licensed under the BSD-2-Clause license

#ifndef CONFIG
#define CONFIG

#define RC_CONFIG "/etc/.kimrc"

#define SCROLL_STEP_DEFAULT 1
#define TAB_SPACE_DEFAULT   0
#define TAB_INDENT_DEFAULT  4
#define COLOR_DEFAULT       8

extern int SCROLL_STEP;
extern int TAB_SPACE;
extern int TAB_INDENT;
extern int COLOR;

void getRcConfig();
void checkRcConfig();

#endif // CONFIG

