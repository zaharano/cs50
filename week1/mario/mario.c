#include <stdio.h>
#include <cs50.h>

void block(int n);
void space(int n);

int main(void)
{
  int height;
  do
  {
    height = get_int("Height: ");
  }
  while (height < 1 || height > 8);

  for (int i = 1; i <= height; i++)
  {
    space(height - i);
    block(i);
    space(2);
    block(i);
    printf("\n");
  }
}

// print a specified number of bricks
void block(int n)
{
  for (int i = 0; i < n; i++)
  {
    printf("#");
  }
}

// print a specified number of spaces
void space(int n)
{
  for (int i = 0; i < n; i++)
  {
    printf(" ");
  }
}