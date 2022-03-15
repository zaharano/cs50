#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
  // check that there is only one argument
  if (argc != 2)
  {
    printf("Usage: ./substitution key\n");
    return 1;
  }

  // check that the key is 26 characters
  if (strlen(argv[1]) != 26)
  {
    printf("Key must be 26 non-repeating alpha characters\n");
    return 1;
  }

  // check for duplicates and non alpha characters in the key and
  // convert key to an array of case-insensitive numeric shifts 
  bool dupcheck[26];
  int key[26];
  for (int i = 0; i < 26; i++)
  {
    dupcheck[i] = false;
  }

  for (int i = 0; i < 26; i++)
  {
    if (!isalpha(argv[1][i]))
    {
      printf("Key must be 26 non-repeating alpha characters\n");
      return 1;
    }
    if (dupcheck[((int) toupper(argv[1][i]) - 65)])
    {
      printf("Key must be 26 non-repeating alpha characters\n");
      return 1;
    }
    dupcheck[(toupper(argv[1][i]) - 65)] = true;
    key[i] = ((toupper(argv[1][i]) - 65) - i);
  }

  // gather plaintext
  string plaintext = get_string("plaintext: ");

  printf("ciphertext: ");
  for (int i = 0; i < strlen(plaintext); i++)
  {
    // print non-alpha characters unchanged
    if (!isalpha(plaintext[i]))
    {
      printf("%c", plaintext[i]);
    }
    // apply the key
    else
    {
      printf("%c", plaintext[i] + key[((toupper(plaintext[i])) - 65)]);
    }
  }
  printf("\n");
  return 0;
}