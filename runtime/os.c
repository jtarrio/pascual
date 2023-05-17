#include "os.h"

#include <stdlib.h>

#include "string.h"

void HALT(PInteger code) { exit(code); }

static PInteger param_count;
static const char** param_str;
static PInteger env_count;
static const char** env_str;

PInteger PARAMCOUNT() { return param_count; }

PString PARAMSTR(PInteger i) {
  if (i < 0 || i > param_count) return (PString){.len = 0};
  return str_of_pchar(param_str[i], 0);
}

PInteger ENVCOUNT() { return env_count; }

PString ENVSTR(PInteger i) {
  if (i < 0 || i > env_count) return (PString){.len = 0};
  return str_of_pchar(env_str[i], 0);
}

static PBoolean IsVar(const PString* var, const char* envstr) {
  for (int j = 0; j < var->len; j++) {
    if (envstr[j] == 0 || var->value[j] != envstr[j]) return 0;
  }
  return envstr[var->len] == '=';
}

static PString CopyVarValue(const PString* var, const char* envstr) {
  PString ret;
  int skip = var->len + 1;
  int len = 0;
  for (len = 0; envstr[len + skip] != 0; len++) {
    ret.value[len] = envstr[len + skip];
    if (len == 255) break;
  }
  ret.len = len;
  return ret;
}

PString GETENV(const PString* varname) {
  for (int i = 0; i < env_count; ++i) {
    if (IsVar(varname, env_str[i])) return CopyVarValue(varname, env_str[i]);
  }
  return (PString){.len = 0};
}

void InitOs(int argc, const char** argv, const char** env) {
  param_count = argc - 1;
  param_str = argv;
  for (env_count = 0; env[env_count] != PNil; ++env_count) /* count */;
  env_str = env;
}
