#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
  string text = get_string("Text: ");

  int letters = count_letters(text);
  int words = count_words(text);
  int sentences = count_sentences(text);

  if (words == 0)
  {
    return 1;
  }

  float lpw = (float) letters / (float) words;
  float spw = (float) sentences / (float) words;

  int index = round(0.0588 * (lpw * 100) - 0.296 * (spw * 100) - 15.8);

  if (index < 1)
  {
    printf("Before Grade 1\n");
    return 0;
  }
  else if (index > 15)
  {
    printf("Grade 16+\n");
    return 0;
  }
  else
  {
    printf("Grade %i\n", index);
    return 0;
  }
}

int count_letters(string text)
{
  int letter_count = 0;
  for (int i = 0; i < strlen(text); i++)
  {
    if (isalpha(text[i]))
    {
      letter_count++;
    }
  }
  return letter_count;
}

int count_words(string text)
{
  int word_count = 0;
  bool word_started = false;

  for (int i = 0; i < strlen(text); i++)
  {
    if (isalpha(text[i]))
    {
      word_started = true;
    }
    if ((text[i] == ' ' || text[i] == '.') && word_started)
    {
      word_count++;
      word_started = false;
    }
  }

  return word_count;
}

int count_sentences(string text)
{
  int sentence_count = 0;
  bool sentence_started = false;

  for (int i = 0; i < strlen(text); i++)
  {
    if (isalpha(text[i]))
    {
      sentence_started = true;
    }
    if ((text[i] == '.' || text[i] == '!' || text[i] == '?') && sentence_started)
    {
      sentence_count++;
      sentence_started = false;
    }
  }

  return sentence_count;
}