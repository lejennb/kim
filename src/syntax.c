// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "input.h"
#include "output.h"
#include "row.h"
#include "syntax.h"

/*

Syntax HL Database Entries (Definitions):
_________________________________________________

char *X_HL_extensions[] = { ".x", NULL };
char *X_HL_keywords[] = {
  ".kw1",

  ".kw2|",

  ".kw3~",

  ".kw4*", NULL
};
char *X_HL_formats[] = { NULL };

*/

char *C_HL_extensions[] = { ".c", ".h", NULL };
char *C_HL_keywords[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "struct", "union", "typedef", "enum", "class", "case", "default", 
  "malloc", "calloc", "realloc", "free", "goto", "sizeof", "asm", "do", 

  "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
  "void|", "short|", "const|", "extern|", "static|",

  "true~", "false~", "NULL~", "EXIT_FAILURE~", "EXIT_SUCCESS~", 
  
  "#include*", "#pragma*", "#define*", "#undef*", "#error*", "#warning*",
  "#if*", "#elif*", "#else*", "#endif*", "#ifdef*", "#ifndef*", "#line*", NULL
};
char *C_HL_formats[] = {
  "\\\\", "\\\%", "\\\"", "\\'", "\\n", "\\r", "\\t", "\\f", "\\b", "\\v", "\%%",
  "\\x1b", "\\027", "\\033", "\\0", "\\e", "\%c", "\%s", "\%d", "\%i", "\%f",
  "\%F", "\%e", "\%E", "\%o", "\%O", "\%p", "\%u", "\%n", "\%x", "\%X", "\%g",
  "\%G", "\%a", "\%A", "\%hi", "\%hu", "\%hd", "\%ld", "\%li", "\%lu", "\%lf",
  "\%lF", "\%lg", "\%lG", "\%le", "\%lE", "\%la", "\%lA", "\%Lf", "\%LF", "\%Lg",
  "\%LG", "\%Le", "\%LE", "\%La", "\%LA", "\%lld", "\%llu", "\%lli", NULL
};

char *CPP_HL_extensions[] = { ".cpp", ".hpp", NULL };
char *CPP_HL_keywords[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "union", "typedef", "enum", "case", "default", "malloc", "calloc",
  "realloc", "free", "new", "delete", "try", "catch", "sizeof", "asm",
  "using", "public", "private", "protected", "operator", "this", "do",
  "friend", 

  "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
  "void|", "short|", "const|", "extern|", "static|", "namespace|",
  "class|", "struct|", "constexpr|", "auto|", "bool|", "virtual|",
  "override|", "final|",

  "true~", "false~", "NULL~", "EXIT_FAILURE~", "EXIT_SUCCESS~",
  
  "#include*", "#pragma*", "#define*", "#undef*", "#error*", "#warning*",
  "#if*", "#elif*", "#else*", "#endif*", "#ifdef*", "#ifndef*", "#elifdef*",
  "#elifndef*", "#line*", NULL
};
char *CPP_HL_formats[] = {  // INFO: Synced with C formats
  "\\\\", "\\\%", "\\\"", "\\'", "\\n", "\\r", "\\t", "\\f", "\\b", "\\v", "\%%",
  "\\x1b", "\\027", "\\033", "\\0", "\\e", "\%c", "\%s", "\%d", "\%i", "\%f",
  "\%F", "\%e", "\%E", "\%o", "\%O", "\%p", "\%u", "\%n", "\%x", "\%X", "\%g",
  "\%G", "\%a", "\%A", "\%hi", "\%hu", "\%hd", "\%ld", "\%li", "\%lu", "\%lf",
  "\%lF", "\%lg", "\%lG", "\%le", "\%lE", "\%la", "\%lA", "\%Lf", "\%LF", "\%Lg",
  "\%LG", "\%Le", "\%LE", "\%La", "\%LA", "\%lld", "\%llu", "\%lli", NULL
};

char *PY_HL_extensions[] = { ".py", NULL };  // TODO: add more data types for type hints!
char *PY_HL_keywords[] = {
  "def", "if", "while", "for", "break", "return", "continue", "else", "elif",
  "import", "try", "except", "in", "and", "or", "is", "not", "with", "as",
  "class", "case", "match", "from", 
  
  "int|", "str|", "unicode|", "dict|", "float|", "repr|", "long|", "eval|",
  "tuple|", "list|", "set|", "frozenset|", "chr|", "unichr|", "ord|", "hex|",

  "True~", "False~", "None~", NULL
};
char *PY_HL_formats[] = { NULL };

char *GO_HL_extensions[] = { ".go", NULL };
char *GO_HL_keywords[] = {
  "const", "func", "import", "package", "type", "var", "interface", "map",
  "struct", "break", "case", "continue", "default", "else", "fallthrough",
  "for", "goto", "if", "range", "return", "select", "switch", "defer", "go",

  "bool|", "string|", "int|", "int8|", "int16|", "int32|", "int64|", "byte|",
  "rune|", "float|", "float32|", "float64|", "complex|", "complex64|", "complex128|",
  "unit|", "unit8|", "uint16|", "uint32|", "uint64|", "uintptr|", "chan|",

  "true~", "false~", "nil~", 
  
  "make*", NULL
};
char *GO_HL_formats[] = {
  "\\\\", "\\\%", "\\\"", "\\'", "\\n", "\\r", "\\t", "\\f", "\\b", "\\v", "\%%",
  "\\x1b", "\\027", "\\033", "\%v", "\%t", "\%T", "\%b", "\%c", "\%d", "\%o", "\%O",
  "\%q", "\%x", "\%X", "\%U", "\%e", "\%E", "\%f", "\%F", "\%g", "\%G", "\%s", "\%p",
  "\%w", NULL
};

char *JAVA_HL_extensions[] = { ".java", NULL };
char *JAVA_HL_keywords[] = {
  "abstract", "contine", "for", "new", "switch", "assert", "default", "goto", "package",
  "synchronized", "do", "if", "private", "this", "break", "implements", "protected",
  "throw", "case", "enum", "instanceof", "return", "transient", "catch", "extends",
  "try", "void", "final", "interface", "static", "class", "finally", "volatile", "const",
  "native", "super", "while", "else", "else if", "public",

  "boolean|", "short|", "int|", "long|", "float|", "double|", "char|", "byte|", 

  "true~", "false~", "null~",
  
  "@Override*", "@Deprecated*", "@SuppressWarnings*", "@SafeVarargs*", "@Documented*",
  "@Inherited*", "@Retention*", "@Target*", "@Author*", "@Company*", "@Repeatable*",
  "@interface*", "@Interface*", "import*", NULL
};
char *JAVA_HL_formats[] = {
  "\\\\", "\\\"", "\\'", "\\n", "\\r", "\\t", "\\f", "\\b", "\\027", "\\033", NULL
};

char *SQL_HL_extensions[] = { ".sql", NULL };  
char *SQL_HL_keywords[] = {
  "ADD", "ALL", "ALTER", "AND", "ANY", "AS", "ASC", "BACKUP", "BETWEEN", "CASE", "CHECK",
  "COLUMN", "CONSTRAINT", "CREATE", "DATABASE", "DEFAULT", "DELETE", "DESC", "DISTINCT",
  "DROP", "EXEC", "EXISTS", "FOREIGN", "FROM", "FULL", "GROUP", "HAVING", "IN", "ORDER",
  "INDEX", "INNER", "INSERT", "IS", "INTO", "NOT", "JOIN", "KEY", "LEFT", "LIKE", "LIMIT",
  "OR", "ON", "OUTER", "PRIMARY", "PROCEDURE", "REFERENCES", "RIGHT", "ROWNUM", "SELECT",
  "SET", "TOP", "TABLE", "TRUNCATE", "UNION", "UPDATE", "USE", "VALUES", "VIEW", "WHERE",
  "BY", "BEGIN", "START", "TRANSACTION", "COMMIT", "ROLLBACK", "SAVEPOINT", "TO", "DESC",
  "RELEASE", "END", "IF", "TRIGGER", "AUTO_INCREMENT", "ROWID", "EXPLAIN", "ANALYZE",
  "RETURNING",

  "CHAR|", "VARCHAR|", "TINYTEXT|", "TEXT|", "BLOB|", "MEDIUMTEXT|", "MEDIUMBLOB|",
  "LONGTEXT|", "LONGBLOB|", "TINYINT|", "SMALLINT|", "MEDIUMINT|", "INT|", "BIGINT|",
  "DECIMAL|", "FLOAT|", "DOUBLE|", "BIT|", "BOOLEAN|", "BOOL|", "BINARY|", "ENUM|",
  "DATE|", "TIME|", "DATETIME|", "TIMESTAMP|", "GEOMETRY|", "POINT|", "DEC|",
  "LINESTRING|", "POLYGON|", "GEOMETRYCOLLECTION|", "MULTILINESTRING|", "MULTIPOINT|",
  "MULTIPOLYGON|", "INTEGER|", "UNSIGNED|", "ZEROFILL|",

  "NULL~", 
  
  "AVG*", "MIN*", "MAX*", "FIRST*", "LAST*", "YEAR*", "MONTH*", "DAY*", "IDENTITY*", NULL
};
char *SQL_HL_formats[] = { NULL };

char *KIMRC_HL_extensions[] = { ".kimrc", NULL };
char *KIMRC_HL_keywords[] = {
  "SCROLL_STEP", "TAB_SPACE", "TAB_INDENT",

  "COLOR|", NULL
};
char *KIMRC_HL_formats[] = { NULL };

char *JS_HL_extensions[] = { ".js", NULL };
char *JS_HL_keywords[] = {
  "await", "break", "case", "catch", "class", "const", "continue", "debugger", "default",
  "delete", "do", "else", "enum", "export", "extends", "finally", "for", "if", "implements",
  "import", "in", "instanceof", "interface", "new", "package", "private", "protected", "public",
  "return", "super", "switch", "static", "throw", "try", "typeof", "while", "with", "abstract",
  "transient", "goto", "native", "throws", "boolean", "byte", "final", "synchronized", "volatile",
  "constructor", "async", "console",

  // no data types really as JS is a dynamic-type language

  "true~", "false~", "null~",

  "function*", "let*", "var*", "this*", "arguments*", NULL
};
char *JS_HL_formats[] = { NULL };

char *JSON_HL_extensions[] = { ".json", NULL };
char *JSON_HL_keywords[] = {
  "true~", "false~", "null~", NULL
};
char *JSON_HL_formats[] = { NULL };


struct editorSyntax HLDB[] = {
  //         extensions, keywords, formats                             comments                     strings                   number & string flags
  { "c",     C_HL_extensions, C_HL_keywords, C_HL_formats,             "//", "/*", "*/",            "\0\0\0", "'", "\"",      F_NUMBERS | F_STRINGS },
  { "c++",   CPP_HL_extensions, CPP_HL_keywords, CPP_HL_formats,       "//", "/*", "*/",            "\0\0\0", "'", "\"",      F_NUMBERS | F_STRINGS },
  { "py",    PY_HL_extensions, PY_HL_keywords, PY_HL_formats,          "#","\0\0\0", "\0\0\0",      "\0\0\0", "'", "\"",      F_NUMBERS | F_STRINGS },
  { "go",    GO_HL_extensions, GO_HL_keywords, GO_HL_formats,          "//", "/*", "*/",            "`", "'", "\"",           F_NUMBERS | F_STRINGS },
  { "java",  JAVA_HL_extensions, JAVA_HL_keywords, JAVA_HL_formats,    "//", "/*", "*/",            "`", "'", "\"",           F_NUMBERS | F_STRINGS },
  { "sql",   SQL_HL_extensions, SQL_HL_keywords, SQL_HL_formats,       "--","\0\0\0", "\0\0\0",     "\0\0\0", "'", "\"",      F_NUMBERS | F_STRINGS },
  { "kimrc", KIMRC_HL_extensions, KIMRC_HL_keywords, KIMRC_HL_formats, "#","\0\0\0", "\0\0\0",      "\0\0\0", "'", "\"",      F_NUMBERS | F_STRINGS },
  { "js",    JS_HL_extensions, JS_HL_keywords, JS_HL_formats,          "//","/*", "*/",             "`", "'", "\"",           F_NUMBERS | F_STRINGS },
  { "json",  JSON_HL_extensions, JSON_HL_keywords, JSON_HL_formats,    "\0\0\0","\0\0\0", "\0\0\0", "\0\0\0", "\0\0\0", "\"", F_NUMBERS | F_STRINGS },
};

int is_separator(int c) {
  return isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];:&{}", c) != NULL;
}

void editorUpdateSyntax(erow *row) {
  row->hl = realloc(row->hl, row->rsize);
  memset(row->hl, HL_NORMAL, row->rsize);

  if (E.syntax == NULL) return;

  char **keywords = E.syntax->keywords;
  char **formats = E.syntax->formats;

  // Comments
  char *scs = E.syntax->singleline_comment_start;
  char *mcs = E.syntax->multiline_comment_start;
  char *mce = E.syntax->multiline_comment_end;

  int scs_len = scs ? strlen(scs) : 0;
  int mcs_len = mcs ? strlen(mcs) : 0;
  int mce_len = mce ? strlen(mce) : 0;

  // Strings 
  char *string_s = E.syntax->string_single;
  char *string_d = E.syntax->string_double;
  char *string_r = E.syntax->string_raw;

  int string_s_len = string_s ? strlen(string_s) : 0;
  int string_d_len = string_d ? strlen(string_d) : 0;
  int string_r_len = string_r ? strlen(string_r) : 0;

  // Checkmarks
  int prev_sep = 1;
  int prev_bin = 0;  // Bin literals
  int prev_hex = 0;  // Hex literals
  int in_string_s = (row->idx > 0 && E.row[row->idx - 1].string_single_open);
  int in_string_d = (row->idx > 0 && E.row[row->idx - 1].string_double_open);
  int in_string_r = (row->idx > 0 && E.row[row->idx - 1].string_raw_open);
  int in_comment = (row->idx > 0 && E.row[row->idx - 1].comment_open);

  int i = 0;
  while (i < row->rsize) {
    char c = row->render[i];
    unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

    if (scs_len && !in_string_s && !in_string_d && !in_string_r && !in_comment) {  // Single-line comments
      if (!strncmp(&row->render[i], scs, scs_len)) {
        memset(&row->hl[i], HL_COMMENT, row->rsize - i);
        break;
      }
    }

    if (mcs_len && mce_len && !in_string_s && !in_string_d && !in_string_r) {  // Multi-line comments
      if (in_comment) {
        row->hl[i] = HL_MLCOMMENT;
        if (!strncmp(&row->render[i], mce, mce_len)) {
          memset(&row->hl[i], HL_MLCOMMENT, mce_len);
          i += mce_len;
          in_comment = 0;
          prev_sep = 1;
          continue;
        } else {
          i++;
          continue;
        }
      } else if (!strncmp(&row->render[i], mcs, mcs_len)) {
        memset(&row->hl[i], HL_MLCOMMENT, mcs_len);
        i += mcs_len;
        in_comment = 1;
        continue;
      }
    }

    if (E.syntax->flags & F_STRINGS) {  // Raw strings
      if (string_r_len && !in_string_s && !in_string_d) {
        if (in_string_r) {
          row->hl[i] = HL_STRING;
          if (!strncmp(&row->render[i], string_r, string_r_len)) {
            memset(&row->hl[i], HL_STRING, string_r_len);
            i += string_r_len;
            in_string_r = 0;
            prev_sep = 1;
            continue;
          } else {
            i++;
            continue;
          }
        } else if (!strncmp(&row->render[i], string_r, string_r_len)) {
          memset(&row->hl[i], HL_STRING, string_r_len);
          i += string_r_len;
          in_string_r = 1;
          continue;
        }
      }
    }

    if (E.syntax->flags & F_STRINGS) {  // Single-quote strings
      if (string_s_len && !in_string_d && !in_string_r) {
        if (in_string_s) {
          row->hl[i] = HL_STRING;
          if (!strncmp(&row->render[i], string_s, string_s_len)) {
            memset(&row->hl[i], HL_STRING, string_s_len);
            i += string_s_len;
            in_string_s = 0;
            prev_sep = 1;
            continue;
          } else {
            int j;
            for (j = 0; formats[j]; j++) {
              int klen = strlen(formats[j]);
              if (!strncmp(&row->render[i], formats[j], klen)) {
                memset(&row->hl[i], HL_FORMAT, klen);
                i += klen;
                break;
              }
            }
            if (formats[j] != NULL) {
              continue;
            }
            i++;
            continue;
          }
        } else if (!strncmp(&row->render[i], string_s, string_s_len)) {
          memset(&row->hl[i], HL_STRING, string_s_len);
          i += string_s_len;
          in_string_s = 1;
          continue;
        }
      }
    }

    if (E.syntax->flags & F_STRINGS) {  // Double-quote strings
      if (string_d_len && !in_string_s && !in_string_r) {
        if (in_string_d) {
          row->hl[i] = HL_STRING;
          if (!strncmp(&row->render[i], string_d, string_d_len)) {
            memset(&row->hl[i], HL_STRING, string_d_len);
            i += string_d_len;
            in_string_d = 0;
            prev_sep = 1;
            continue;
          } else {
            int j;
            for (j = 0; formats[j]; j++) {
              int klen = strlen(formats[j]);
              if (!strncmp(&row->render[i], formats[j], klen)) {
                memset(&row->hl[i], HL_FORMAT, klen);
                i += klen;
                break;
              }
            }
            if (formats[j] != NULL) {
              continue;
            }
            i++;
            continue;
          }
        } else if (!strncmp(&row->render[i], string_d, string_d_len)) {
          memset(&row->hl[i], HL_STRING, string_d_len);
          i += string_d_len;
          in_string_d = 1;
          continue;
        }
      }
    }

    if (E.syntax->flags & F_NUMBERS) {  // Numbers
      if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) || (c == '.' && prev_hl == HL_NUMBER)) {
        row->hl[i] = HL_NUMBER;
        i++;
        prev_sep = 0;
        continue;
      }
      // Bin (0b), Hex (0x)
      if (strchr("bx", c) != NULL && prev_hl == HL_NUMBER && !prev_bin && !prev_hex) {
        if (c == 'b') { prev_bin = 1; }
        if (c == 'x') { prev_hex = 1; }
        row->hl[i] = HL_NUMBER;
        i++;
        prev_sep = 0;
        continue;
      }
      // Hex digits
      if (strchr("ABCDEFabcdef", c) != NULL && prev_hl == HL_NUMBER && prev_hex) {
        row->hl[i] = HL_NUMBER;
        i++;
        prev_sep = 0;
        continue;
      }
      prev_bin = 0;  // Reset
      prev_hex = 0;  // Prevent "hex-inheritance" for next number in same row
    }

    if (prev_sep) {  // HLDB Keywords
      int j;
      for (j = 0; keywords[j]; j++) {
        int klen = strlen(keywords[j]);
        int kw2 = keywords[j][klen - 1] == '|';  
        int kw3 = keywords[j][klen - 1] == '~';
        int kw4 = keywords[j][klen - 1] == '*'; 
        if (kw2 || kw3 || kw4) klen--;

        if (!strncmp(&row->render[i], keywords[j], klen) && is_separator(row->render[i + klen])) {
          memset(&row->hl[i], kw4 ? HL_KEYWORD4 : kw3 ? HL_KEYWORD3 : kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
          i += klen;
          break;
        }
      }
      if (keywords[j] != NULL) {
        prev_sep = 0;
        continue;
      }
    }

    prev_sep = is_separator(c);
    i++;
  }

  int comment_changed = (row->comment_open != in_comment);
  row->comment_open = in_comment;
  if (comment_changed && row->idx + 1 < E.numrows) { editorUpdateSyntax(&E.row[row->idx + 1]); }

  int string_raw_changed = (row->string_raw_open != in_string_r);
  row->string_raw_open = in_string_r;
  if (string_raw_changed && row->idx + 1 < E.numrows) { editorUpdateSyntax(&E.row[row->idx + 1]); }

  int string_single_changed = (row->string_single_open != in_string_s);
  row->string_single_open = in_string_s;
  if (string_single_changed && row->idx + 1 < E.numrows) { editorUpdateSyntax(&E.row[row->idx + 1]); }

  int string_double_changed = (row->string_double_open != in_string_d);
  row->string_double_open = in_string_d;
  if (string_double_changed && row->idx + 1 < E.numrows) { editorUpdateSyntax(&E.row[row->idx + 1]); }
}

int editorSyntaxToColor(int hl) {
  if (COLOR == 256) {
    switch (hl) {
      case HL_COMMENT:
      case HL_MLCOMMENT: return 123;
      case HL_KEYWORD1: return 227;
      case HL_KEYWORD2: return 49;
      case HL_KEYWORD3: return 178;
      case HL_KEYWORD4: return 75;
      case HL_FORMAT: return 111;
      case HL_STRING: return 98;
      case HL_NUMBER: return 207;
      case HL_MATCH: return 197;
      default: return 255;
    }
  } else if (COLOR == 16) {
    switch (hl) {
      case HL_COMMENT:
      case HL_MLCOMMENT: return 96;
      case HL_KEYWORD1: return 93;
      case HL_KEYWORD2: return 92;
      case HL_KEYWORD3: return 33;
      case HL_KEYWORD4: return 94;
      case HL_FORMAT: return 94;
      case HL_STRING: return 95;
      case HL_NUMBER: return 91;
      case HL_MATCH: return 34;
      default: return 37;
    }
  } else {  // Default (8)
    switch (hl) {
      case HL_COMMENT:
      case HL_MLCOMMENT: return 36;
      case HL_KEYWORD1: return 33;
      case HL_KEYWORD2: return 32;
      case HL_KEYWORD3: return 33;
      case HL_KEYWORD4: return 34;
      case HL_FORMAT: return 34;
      case HL_STRING: return 35;
      case HL_NUMBER: return 31;
      case HL_MATCH: return 34;
      default: return 37;
    }
  }
}

void editorResetSyntax() {
  E.syntax = NULL;
  for (int filerow = 0; filerow < E.numrows; filerow++) {
    editorUpdateSyntax(&E.row[filerow]);
  }
}

void editorSelectSyntax() {
  editorResetSyntax();

  if (E.filename == NULL) return;

  char *ext = strrchr(E.filename, '.');

  for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
    struct editorSyntax *s = &HLDB[j];
    unsigned int i = 0;
    while (s->filematch[i]) {
      int is_ext = (s->filematch[i][0] == '.');
      if ((is_ext && ext && !strcmp(ext, s->filematch[i])) || (!is_ext && strstr(E.filename, s->filematch[i]))) {
        E.syntax = s;

        for (int filerow = 0; filerow < E.numrows; filerow++) {
          editorUpdateSyntax(&E.row[filerow]);
        }

        return;
      }
      i++;
    }
  }
}

void editorChangeSyntax() {
  char *ext = editorPrompt("Change highlighting: %s (ESC for none)", NULL);

  editorResetSyntax();

  if (ext == NULL) return;

  for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
    struct editorSyntax *s = &HLDB[j];
    unsigned int i = 0;
    while (s->filematch[i]) {
      int is_ext = (s->filematch[i][0] == '.');
      if ((is_ext && ext && !strcmp(ext, s->filematch[i])) || (!is_ext && strstr(E.filename, s->filematch[i]))) {
        E.syntax = s;

        for (int filerow = 0; filerow < E.numrows; filerow++) {
          editorUpdateSyntax(&E.row[filerow]);
        }

        free(ext);
        return;
      }
      i++;
    }
  }
  free(ext);
}

void editorSyntaxQueue() {
  if (E.hl_queue) {
    if (E.hl_queue_pos < 0 || E.hl_queue_pos > E.numrows-1) {
      E.hl_queue = 0;
      return;
    }

    clock_t queue_timer = clock() - E.hl_queue_time;
    double queue_delta = ((double)queue_timer)/CLOCKS_PER_SEC;
    //editorSetStatusMessage("Time elapsed %f", queue_delta);

    float queue_duration = 0.00002; // TODO: Should be a parameter of the "editorSyntaxQueue()" call
    if (queue_delta > queue_duration && E.numrows > 0){
      editorUpdateSyntax(&E.row[E.hl_queue_pos]);
      E.hl_queue = 0;
    } 
  }
}
