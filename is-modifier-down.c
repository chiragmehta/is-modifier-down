#include <CoreGraphics/CoreGraphics.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

static const CGEventFlags kKnownFlags =
    kCGEventFlagMaskCommand |
    kCGEventFlagMaskAlternate |
    kCGEventFlagMaskControl |
    kCGEventFlagMaskShift |
    kCGEventFlagMaskSecondaryFn;

static const char *kVersion = "1.0.0";

static void usage(const char *prog) {
  fprintf(stderr, "usage: %s [modifiers...]\n", prog);
  fprintf(stderr, "  no args: print normalized modifier mask\n");
  fprintf(stderr, "  --mask: exit with normalized modifier mask, no stdout\n");
  fprintf(stderr, "  -h, --help: show this help\n");
  fprintf(stderr, "  -V, --version: print version\n");
  fprintf(stderr, "  mask bits:\n");
  fprintf(stderr, "    cmd=1 opt=2 ctrl=4 shift=8 fn=16\n");
  fprintf(stderr, "  prefixes: + = exclusive-on, ! = must be off\n");
  fprintf(stderr, "  mods: cmd opt ctrl shift fn\n");
}

static bool parse_modifier(const char *arg, CGEventFlags *flag, bool *exclusive, bool *negated) {
  *flag = 0;
  *exclusive = false;
  *negated = false;

  if (!arg || !*arg) return false;

  while (*arg == '!' || *arg == '+') {
    if (*arg == '!') {
      if (*exclusive || *negated) return false;
      *negated = true;
    } else if (*arg == '+') {
      if (*exclusive || *negated) return false;
      *exclusive = true;
    }
    arg++;
  }

  if (!*arg) return false;

  if (strcasecmp(arg, "cmd") == 0 || strcasecmp(arg, "command") == 0) {
    *flag = kCGEventFlagMaskCommand;
    return true;
  }
  if (strcasecmp(arg, "opt") == 0 || strcasecmp(arg, "option") == 0 || strcasecmp(arg, "alt") == 0) {
    *flag = kCGEventFlagMaskAlternate;
    return true;
  }
  if (strcasecmp(arg, "ctrl") == 0 || strcasecmp(arg, "control") == 0) {
    *flag = kCGEventFlagMaskControl;
    return true;
  }
  if (strcasecmp(arg, "shift") == 0) {
    *flag = kCGEventFlagMaskShift;
    return true;
  }
  if (strcasecmp(arg, "fn") == 0 || strcasecmp(arg, "function") == 0 || strcasecmp(arg, "function-key") == 0 || strcasecmp(arg, "globe") == 0) {
    *flag = kCGEventFlagMaskSecondaryFn;
    return true;
  }
  return false;
}

static unsigned int current_modifier_code(CGEventFlags current) {
  unsigned int code = 0;
  if (current & kCGEventFlagMaskCommand) code |= 1u;
  if (current & kCGEventFlagMaskAlternate) code |= 2u;
  if (current & kCGEventFlagMaskControl) code |= 4u;
  if (current & kCGEventFlagMaskShift) code |= 8u;
  if (current & kCGEventFlagMaskSecondaryFn) code |= 16u;
  return code;
}

int main(int argc, char **argv) {
  CGEventFlags requiredOn = 0;
  CGEventFlags requiredOff = 0;
  CGEventFlags allowedOn = 0;
  bool exclusiveMode = false;
  CGEventFlags current = CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState);
  unsigned int mask = current_modifier_code(current);

  if (argc == 1) {
    printf("%u\n", mask);
    return 0;
  }

  if (argc == 2 && strcmp(argv[1], "--mask") == 0) {
    return (int)mask;
  }

  if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    usage(argv[0]);
    return 0;
  }

  if (argc == 2 && (strcmp(argv[1], "-V") == 0 || strcmp(argv[1], "--version") == 0)) {
    printf("%s\n", kVersion);
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    CGEventFlags flag = 0;
    bool exclusive = false;
    bool negated = false;
    if (!parse_modifier(argv[i], &flag, &exclusive, &negated)) {
      usage(argv[0]);
      return 2;
    }

    if (negated) {
      requiredOff |= flag;
      continue;
    }

    if (exclusive) {
      exclusiveMode = true;
      requiredOn |= flag;
      allowedOn |= flag;
      continue;
    }

    if (exclusiveMode) {
      allowedOn |= flag;
    } else {
      requiredOn |= flag;
    }
  }

  if ((current & requiredOn) != requiredOn) return 1;
  if (current & requiredOff) return 1;

  if (exclusiveMode) {
    if ((current & kKnownFlags & ~allowedOn) != 0) return 1;
  }

  return 0;
}
