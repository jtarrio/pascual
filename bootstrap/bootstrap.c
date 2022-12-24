#include <stdio.h>

#include "runtime.h"

struct token {
  int type;
  PString value;
};

const char symbols[] = {'+', '-', '*', '/', '=', '<', '>', '[',
                        ']', '.', ',', ':', ';', '^', '(', ')'};
const PString words[] = {
    PSTRING("and"),      PSTRING("array"),  PSTRING("begin"),
    PSTRING("case"),     PSTRING("const"),  PSTRING("div"),
    PSTRING("do"),       PSTRING("downto"), PSTRING("else"),
    PSTRING("end"),      PSTRING("file"),   PSTRING("for"),
    PSTRING("function"), PSTRING("goto"),   PSTRING("if"),
    PSTRING("in"),       PSTRING("label"),  PSTRING("mod"),
    PSTRING("nil"),      PSTRING("not"),    PSTRING("of"),
    PSTRING("or"),       PSTRING("packed"), PSTRING("procedure"),
    PSTRING("program"),  PSTRING("record"), PSTRING("repeat"),
    PSTRING("set"),      PSTRING("then"),   PSTRING("to"),
    PSTRING("type"),     PSTRING("until"),  PSTRING("var"),
    PSTRING("while"),    PSTRING("with")};

enum {
  token_unknown = 0,
  token_eof,
  token_comment,
  token_identifier,
  token_number,
  token_plus,
  token_minus,
  token_asterisk,
  token_slash,
  token_equals,
  token_lessthan,
  token_morethan,
  token_lbracket,
  token_rbracket,
  token_dot,
  token_comma,
  token_colon,
  token_semicolon,
  token_caret,
  token_lparen,
  token_rparen,
  token_notequals,
  token_lessorequals,
  token_moreorequals,
  token_assign,
  token_range,
  token_and,
  token_array,
  token_begin,
  token_case,
  token_const,
  token_div,
  token_do,
  token_downto,
  token_else,
  token_end,
  token_file,
  token_for,
  token_function,
  token_goto,
  token_if,
  token_in,
  token_label,
  token_mod,
  token_nil,
  token_not,
  token_of,
  token_or,
  token_packed,
  token_procedure,
  token_program,
  token_record,
  token_repeat,
  token_set,
  token_then,
  token_to,
  token_type,
  token_until,
  token_var,
  token_while,
  token_with
};

PString current_line;

PBoolean token_waiting() {
  char chr;
  while (1) {
    while (Plength(&current_line) == 0) {
      if (Peof(Pinput)) return PFalse;
      Pread_string(Pinput, &current_line);
      Preadln(Pinput);
    }

    chr = STRELEM(current_line, 1);
    if (chr != ' ') return PTrue;
    Pdelete(&current_line, 1, 1);
  }
}

void getidentifier(VAR struct token* token) {
  int pos = 0;
  PBoolean in_token = PTrue;
  char chr;
  while (pos < STRLEN(current_line) && in_token) {
    chr = STRELEM(current_line, pos + 1);
    in_token = ((chr >= 'a') && (chr <= 'z')) || ((chr >= 'A' && chr <= 'Z')) ||
               ((chr >= '0') && (chr <= '9'));
    if (in_token) pos = pos + 1;
  }
  token->type = token_identifier;
  token->value = Pcopy(&current_line, 1, pos);
  Pdelete(&current_line, 1, pos);
  for (pos = 0; pos < 35; ++pos) {
    if (Pcompare(&token->value, &words[pos]) == 0) {
      token->type = pos + token_and;
      return;
    }
  }
}

void getnumber(VAR struct token* token) {
  enum {
    part_none = 0,
    part_sign,
    part_int,
    part_dot,
    part_frac,
    part_e,
    part_mag
  };
  int pos = 0;
  int part = part_sign;
  char chr;
  while (pos < STRLEN(current_line) && part != part_none) {
    chr = STRELEM(current_line, pos + 1);
    if ((chr == '-') || (chr == '+')) {
      if (part == part_sign)
        part = part_int;
      else if (part == part_e)
        part = part_mag;
      else
        part = part_none;
    } else if (chr == 'e') {
      if (part == part_int || part == part_frac)
        part = part_e;
      else
        part = part_none;
    } else if (chr == '.') {
      if (part == part_int)
        part = part_dot;
      else
        part = part_none;
    } else if ((chr >= '0') && (chr <= '9')) {
      if (part == part_sign)
        part = part_int;
      else if (part == part_dot)
        part = part_frac;
      else if (part == part_e)
        part = part_mag;
    } else {
      part = part_none;
    }
    if (part != part_none) pos = pos + 1;
  }
  token->type = token_number;
  token->value = Pcopy(&current_line, 1, pos);
  Pdelete(&current_line, 1, pos);
}

void getcomment(VAR struct token* token) {
  int pos = 0;
  PBoolean in_comment = PTrue;
  char chr;
  char nxt;
  while (in_comment) {
    while (pos < STRLEN(current_line) && in_comment) {
      pos = pos + 1;
      chr = STRELEM(current_line, pos);
      if (chr == '}') in_comment = PFalse;
      if ((chr == '*') && ((pos + 1) < STRLEN(current_line))) {
        pos = pos + 1;
        nxt = STRELEM(current_line, pos);
        if (nxt == ')') in_comment = PFalse;
      }
    }
    Pdelete(&current_line, 1, pos);
    if (in_comment) {
      in_comment = token_waiting();
    }
  }
  token->type = token_comment;
}

void getsymbol(VAR struct token* token, int type, int length) {
  token->value = Pcopy(&current_line, 1, length);
  Pdelete(&current_line, 1, length);
  token->type = type;
}

void getnexttoken(VAR struct token* token) {
  char chr;
  char nxt;
  if (!token_waiting()) {
    Pclear(&token->value);
    token->type = token_eof;
    return;
  }

  chr = STRELEM(current_line, 1);

  if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
    return getidentifier(token);
  if (chr >= '0' && chr <= '9') return getnumber(token);
  if (STRLEN(current_line) > 1) {
    nxt = STRELEM(current_line, 2);
    if ((chr == '+' || chr == '-') && (nxt >= '0' && nxt <= '9'))
      return getnumber(token);
    if (chr == '(' && nxt == '*') return getcomment(token);
    if (chr == '<' && nxt == '>') return getsymbol(token, token_notequals, 2);
    if (chr == '<' && nxt == '=')
      return getsymbol(token, token_lessorequals, 2);
    if (chr == '>' && nxt == '=')
      return getsymbol(token, token_moreorequals, 2);
    if (chr == ':' && nxt == '=') return getsymbol(token, token_assign, 2);
    if (chr == '.' && nxt == '.') return getsymbol(token, token_range, 2);
  }
  if (chr == '{') return getcomment(token);
  for (int i = 0; i < sizeof(symbols); ++i) {
    if (chr == symbols[i]) return getsymbol(token, i + token_plus, 1);
  }
  Pclear(&token->value);
  token->type = token_unknown;
}

void program() {
  const PString separator = PSTRING(" -> ");
  struct token token;
  do {
    getnexttoken(&token);
    Pwrite_integer(Poutput, token.type);
    Pwrite_string(Poutput, &separator);
    Pwrite_string(Poutput, &token.value);
    Pwriteln(Poutput);
  } while ((token.type != token_eof) && (token.type != token_unknown));
}

int main() {
  InitFiles();
  program();
  return 0;
}
