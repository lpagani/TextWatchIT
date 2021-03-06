#include "num2words-it.h"
#include "string.h"

static const char* const ONES[] = {
  "in punto",
  "uno",
  "due",
  "tre",
  "quattro",
  "cinque",
  "sei",
  "sette",
  "otto",
  "nove"
};

static const char* const HOURS[] = {
  "",
  "una",
  "due",
  "tre",
  "quattro",
  "cinque",
  "sei",
  "sette",
  "otto",
  "nove",
  "dieci",
  "undici",
  "dodici"
};


static const char* const TEENS[] ={
  "",
  "undici",
  "dodici",
  "tredici",
  "quattor dici",
  "quindici",
  "sedici",
  "dicias sette",
  "diciotto",
  "dician nove"
};

static const char* const TENS[] = {
  "",
  "dieci",
  "venti",
  "trenta",
  "quaranta",
  "cinquanta",
  "sessanta",
  "settanta",
  "ottanta",
  "novanta"
};

static size_t append_hour(char* words, int num) {
  strcat(words, HOURS[num]);
  return strlen(HOURS[num]);
}

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;
  
  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    int tens_len = strlen(TENS[tens_val]);
    if ((ones_val == 1) || (ones_val == 8)){ // special manipulation for truncation
      tens_len --;
      strncat(words, TENS[tens_val], tens_len);      
    }
    else {
      strcat(words, TENS[tens_val]);
    }
    
    len += tens_len;
    int ones_len = strlen(ONES[ones_val]);
    if ((ones_val > 0) && ((tens_len + ones_len) > 9)){ //short numbers fit in one line
      strcat(words, " ");
      len += 1;
    }
   //app_log(APP_LOG_LEVEL_INFO, "num2words-it", 81, "num: %d tens: %d ones %d, tens_len %d ones_len %d, str ::%s::", num, tens_val, ones_val, tens_len, ones_len, words);
  
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}


void time_to_words(int hours, int minutes, char* words, size_t length) {

  size_t remaining = length;
  memset(words, 0, length);

  if (hours == 0 || hours == 12) {
    remaining -= append_string(words, remaining, TEENS[2]);
  } else {
    remaining -= append_hour(words, hours % 12);
  }

  remaining -= append_string(words, remaining, " ");
  remaining -= append_number(words, minutes);
  remaining -= append_string(words, remaining, " ");
}

void time_to_3words(int hours, int minutes, char *line1, char *line2, char *line3, size_t length)
{
	char value[length];
	time_to_words(hours, minutes, value, length);
	
	memset(line1, 0, length);
	memset(line2, 0, length);
	memset(line3, 0, length);
	
	char *start = value;
	char *pch = strstr (start, " ");
	while (pch != NULL) {
		if (line1[0] == 0) {
			memcpy(line1, start, pch-start);
		}  else if (line2[0] == 0) {
			memcpy(line2, start, pch-start);
		} else if (line3[0] == 0) {
			memcpy(line3, start, pch-start);
		}
		start += pch-start+1;
		pch = strstr(start, " ");
	}
  
  // add third line in between if only two lines are shown
  if (line3[0] == 0){
    memcpy(line3, line2, strlen(line2)+1);
    memcpy(line2, "e", 1);
    line2[1] = 0;
  }
}
