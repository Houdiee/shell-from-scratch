#include "../args.h"
#include "../error.h"
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

const int CD_MAX_ARGS_LEN = 2;

int cd_to_home_directory();

void change_working_directory(char **args) {
  char *command = args[0];

  if (args[1] == NULL) {
    if (cd_to_home_directory() != 0) {
      print_error(command, "Failed to get home directory");
    }
    return;
  }

  if (!args_is_expected_len(args, CD_MAX_ARGS_LEN)) {
    print_error(command, strerror(errno));
    return;
  };

  if (chdir(args[1]) == -1) {
    print_error(command, strerror(errno));
  }
}

int cd_to_home_directory() {
  struct passwd *pw = getpwuid(getuid());

  if (pw != NULL) {
    char *homedir = pw->pw_dir;
    char *cd_args[] = {"cd", homedir, NULL};
    change_working_directory(cd_args);
    return 0;
  } else {
    return 1;
  }
}
