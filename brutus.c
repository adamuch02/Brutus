#include "brutus.h"

int main(int argc, char** argv)
{
	FILE* encrypted_file;
	uint_fast8_t key;

	encrypted_file = open_file(argc, argv);
	key = find_key(encrypted_file);
	decrypt(encrypted_file, key, "decrypted.txt");

	return 0;
}

FILE* open_file(int argc, char** argv)
{
	FILE* encrypted_file;

	if(argc != 2)
	{
		error(1);
	}
	encrypted_file = fopen(argv[1], "r, css=UTF-8");
	if(encrypted_file == NULL)
	{
		error(2);
	}
	return encrypted_file;
}

uint_fast8_t check_character(wint_t character)
{
	if((character >= 65) && (character <= 90))
	{
		return character - 65;
	}
	else if((character >= 97) && (character <= 122))
	{
		return character - 97;
	}
	return 100;
}

uint_fast8_t find_key(FILE* encrypted_file)
{
	float 		relative_frequency[26] ={0,};
	float 		scores[26] = {0,};
	uint_fast8_t 	key;
	uint_fast64_t 	absolute_frequency[26] = {0,};
	uint_fast64_t 	total;

	total = count_letters(encrypted_file, absolute_frequency);
	convert_frequency(relative_frequency, total, absolute_frequency);
	get_scores(scores, relative_frequency);
	key = find_lowest(scores);

	return key;
}

uint_fast8_t find_lowest(float* scores)
{
	float 		lowest_score = scores[0];
	uint_fast8_t 	lowest_index = 0;

	for(register uint_fast8_t i = 1; i < 26; i++)
	{
		if(scores[i] < lowest_score)
		{
			lowest_score = scores[i];
			lowest_index = i;
		}
	}

	return lowest_index; 
}

uint_fast64_t count_letters(FILE* encrypted_file, 
		uint_fast64_t* absolute_frequency)
{
	uint_fast8_t 	index;
	uint_fast64_t 	total = 0;
	wint_t 		character;

	while((character = fgetwc(encrypted_file)) != WEOF)
	{
		if((index = check_character(character)) < 26)
		{
			absolute_frequency[index]++;
			total++;
		}
	}
	return total;
}

void convert_frequency(float* relative_frequency, uint_fast64_t total, 
		uint_fast64_t* absolute_frequency)
{
	for(register uint_fast8_t i = 0; i < 26; i++)
	{
		relative_frequency[i] = ((float)absolute_frequency[i] / (float)total) * 100;
	}
}

void decrypt(FILE* encrypted_file, uint_fast8_t key, char* filepath_new)
{
	FILE* 	decrypted_file;
	wint_t character;

	decrypted_file = fopen(filepath_new, "w, css=UTF-8");
	if(decrypted_file == NULL)
	{
		error(3);
	}
	fseek(encrypted_file, 0, SEEK_SET);
	while((character = fgetwc(encrypted_file)) != WEOF)
	{
		if(check_character(character) < 26)
		{
			increment_character(&character, key);
		}

		fputwc(character, decrypted_file);
	}
	fclose(decrypted_file);
}

void error(uint_fast8_t error_code)
{
	switch (error_code)
	{
		case 1:
			printf("Error: invalid arguments\n");
			exit(1);
		case 2:
			printf("Error: file not found\n");
			exit(2);
		default:
			printf("Error: something went wrong\n");
			exit(999);
	}
}

void get_scores(float* scores, float* relative_frequency)
{
	for(register int i = 0; i < 26; i++)
	{
		for(register int j = 0; j < 26; j++)
		{
			uint_fast8_t k = i + j;
			k = (k >= 26) ? k - 26 : k;

			if(relative_frequency[j] <= TARGET_FREQUENCY[k] && relative_frequency[j] > 0)
			{
				scores[i] += TARGET_FREQUENCY[k] / relative_frequency[j];
			}
			else if(relative_frequency[j] > TARGET_FREQUENCY[k])
			{
				scores[i] += relative_frequency[j] / TARGET_FREQUENCY[k];
			}
			else
			{
				scores[i] += TARGET_FREQUENCY[k] / 0.074;
			}
		}
	}
}

/*void increment_character(wint_t* character, uint_fast8_t key)
{
	*character += key;
	if(((*character >= 91) && (*character <= 96)) || (*character >= 123))
	{
		*character -= 26;
	}
}*/

void increment_character(wint_t* character, uint_fast8_t key)
{
	if(*character <= 90)
	{
		*character += key;
		if(*character >= 91)
		{
			*character -= 26;
		}
	}
	else
	{
		*character += key;
		if(*character >= 123)
		{
			*character -= 26;
		}
	}
}

