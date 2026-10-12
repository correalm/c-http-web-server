#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int is_method(const char *s) {
  static const char *m[] = {"GET",  "POST",    "PUT",   "DELETE",
                            "HEAD", "OPTIONS", "PATCH", NULL};
  for (int i = 0; m[i]; i++)
    if (!strcmp(s, m[i]))
      return 1;
  return 0;
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

    if (n != 3 || has_bad_space || !is_method(method) || path[0] != '/' ||
        !is_version(version)) {
      puts("INVALID");
      continue;
    }

    printf("METHOD=%s PATH=%s VERSION=%s\n", method, path, version);
  }
  return 0;
}
