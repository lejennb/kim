// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <stdio.h>
#include <stdlib.h>

#include "row.h"

#ifndef SYNTAX
#define SYNTAX

/*

Syntax HL Database Prototypes:
_________________________________________________

extern char *X_HL_extensions[];
extern char *X_HL_keywords[];
extern char *X_HL_formats[];

*/

extern char *C_HL_extensions[];
extern char *C_HL_keywords[];
extern char *C_HL_formats[];

extern char *CPP_HL_extensions[];
extern char *CPP_HL_keywords[];
extern char *CPP_HL_formats[];

extern char *PY_HL_extensions[];
extern char *PY_HL_keywords[];
extern char *PY_HL_formats[];

extern char *GO_HL_extensions[];
extern char *GO_HL_keywords[];
extern char *GO_HL_formats[];

extern char *JAVA_HL_extensions[];
extern char *JAVA_HL_keywords[];
extern char *JAVA_HL_formats[];

extern char *SQL_HL_extensions[];  
extern char *SQL_HL_keywords[];
extern char *SQL_HL_formats[];

extern char *KIMRC_HL_extensions[];
extern char *KIMRC_HL_keywords[];
extern char *KIMRC_HL_formats[];

extern char *JS_HL_extensions[];
extern char *JS_HL_keywords[];
extern char *JS_HL_formats[];

extern char *JSON_HL_extensions[];
extern char *JSON_HL_keywords[];
extern char *JSON_HL_formats[];


enum editorHighlight {
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_KEYWORD3,
  HL_KEYWORD4,
  HL_FORMAT,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH
};

#define F_NUMBERS (1<<0)  // 00000001
#define F_STRINGS (1<<1)  // 00000010

struct editorSyntax {
  char *filetype;
  char **filematch;
  char **keywords;
  char **formats;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  char *string_raw;
  char *string_single;
  char *string_double; 
  int flags;
};

extern struct editorSyntax HLDB[9];

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

int is_separator(int c);
void editorUpdateSyntax(erow *row);
int editorSyntaxToColor(int hl);
void editorResetSyntax();
void editorSelectSyntax();
void editorChangeSyntax();
void editorSyntaxQueue();

#endif // SYNTAX

