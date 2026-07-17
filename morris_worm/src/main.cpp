// vulnerability
#include "vulnerability/gets.h"

typedef struct
{
  char username[8];
  int isAdmin;
} User;

void
read_username(User* user)
{
  printf("Enter username: ");
  gets(user->username);
}

int
main()
{
  User user = { .username = "", .isAdmin = 0 };

  read_username(&user);

  if (user.isAdmin) {
    printf("Admin Access Granted\n");
  } else {
    printf(" User\n");
  }
  return 0;
}
