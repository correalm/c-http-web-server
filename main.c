#include <ctype.h>
#include <stdio.h>
#include <string.h>

enum Code {
  OK = 200,
  BAD_REQUEST = 400,
  INTERNAL_SERVER_ERR = 500,
  NOT_IMPLEMENTED = 501,
  HTTP_VERSION_NOT_SUPPORTED = 505
};

enum Method { GET, POST, PUT, DELETE, HEAD, OPTIONS, PATCH };

static const char *get_method(int m) {
  switch (m) {
  case GET:
    return "GET";
  case POST:
    return "POST";
  case PUT:
    return "PUT";
  case DELETE:
    return "DELETE";
  case HEAD:
    return "HEAD";
  case OPTIONS:
    return "OPTIONS";
  case PATCH:
    return "PATCH";
  default:
    return NULL;
  }
}

static int is_method_implemented(const char *s) {
  const char *m[] = {get_method(GET),   get_method(POST),
                     get_method(PUT),   get_method(DELETE),
                     get_method(HEAD),  get_method(OPTIONS),
                     get_method(PATCH), NULL};
  for (int i = 0; m[i]; i++)
    if (!strcmp(s, m[i]))
      return 1;
  return 0;
}

static int is_method(const char *s) {
  int len = strlen(s);
  for (int i = 0; i < len; i++) {
    if (!isupper((unsigned char)s[i]))
      return 0;
  }

  return 1;
}

static int has_version_code(const char *s) {
  if (!isdigit((unsigned char)s[5]) || s[6] != '.' ||
      !isdigit((unsigned char)s[7]))
    return 0;

  return 1;
}

static int is_version(const char *s) {
  if (strlen(s) != 8 || (strncmp(s, "HTTP/", 5) != 0) || !has_version_code(s))
    return 0;

  return 1;
}

static int is_valid_path(const char *path, const char *method) {
  return path[0] == '/' ||
         (strcmp(path, "*") == 0 && strcmp(method, get_method(OPTIONS)) == 0);
}

static void log_code(int code) { printf("%d\n", code); }

static void log_success(const char *p, const char *m, const char *v) {
  printf("OK %s %s %s\n", p, m, v);
}

int main(void) {
  char line[4096];
  while (fgets(line, sizeof line, stdin)) {
    char *nl = strpbrk(line, "\r\n");
    if (nl)
      *nl = 0;

    if (line[0] == 0)
      continue;

    char method[64], path[2048], version[64];
    int a, b, c, d = 0;
    int n = sscanf(line, "%63s%n %n%2047s%n %n%63s", method, &a, &b, path, &c,
                   &d, version);

    int has_bad_space = ((b - a) + (d - c)) != 2;

    if (n != 3 || has_bad_space || !is_method(method) ||
        !is_valid_path(path, method) || !is_version(version)) {
      log_code(BAD_REQUEST);
      continue;
    }

    if (!is_method_implemented(method))
      log_code(NOT_IMPLEMENTED);

    // TODO: implement the http version validation

    log_success(method, path, version);
  }

  return 0;
}
