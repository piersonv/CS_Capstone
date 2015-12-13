#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define MAXTOKS 100

int parser (char ** parsed_line, string input_string)
{
  int nchars = input_string.size();
  int word_size = 0;
  int word_start = 0;
  int word_count = 0;
 
  for (int char_count = 0; char_count < nchars; char_count++)
  {    
    if (isspace(input_string[char_count]) || (input_string[char_count] == '\0' && char_count < nchars))
    {
      int white_space_count = 0;
      while (isspace(input_string[char_count]))
      {
        white_space_count++;
        char_count++;

        if (input_string[char_count] == '\0')
        {
          char_count++;
        }
      }

      parsed_line[word_count] = new char [word_size +1];

      int word_char = 0;
      
      while (word_start < char_count)
      {
        parsed_line[word_count][word_char] = input_string[word_start];
        word_start++;
        word_char++;
      }
      parsed_line[word_count][word_size] = '\0';
      word_size = 0;
      word_count++;
    }

    //cout << parsed_line[word_count] << endl;

    word_size++;
  
    if (char_count > nchars)
    {
      return 1;
    }     
  }

  parsed_line[word_count] = new char [1];
  parsed_line[word_count][0] = '\0';

  if (nchars <= 1 || word_count == 0)
  {
    return 1;
  }

  return 0;
};
		       
  

  
