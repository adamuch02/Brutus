#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <stdint.h>

const float TARGET_FREQUENCY[] = 
{
	8.2,	//A
	1.5,	//B
	2.8, 	//C
	4.3,	//D
	13.0,	//E
	2.2,	//F
	2.0,	//G
	6.1,	//H
	7.0,	//I
	0.15,	//J
	0.77,	//K
	4.0,	//L
	2.4,	//M
	6.7,	//N
	7.5,	//O
	1.9,	//P
	0.095,	//Q
	6.0,	//R
	6.3,	//S
	9.1,	//T
	2.8,	//U
	0.98,	//V
	2.4,	//W
	0.15,	//X
	2.0,	//Y
	0.074	//Z
};

FILE* open_file(int argc, char** argv);

uint_fast8_t check_character(wint_t character);
uint_fast8_t find_key(FILE* encrypted_file);
uint_fast8_t find_lowest(float* scores);

uint_fast64_t count_letters(FILE* encrypted_file, 
	uint_fast64_t* absolute_frequency);

void convert_frequency(float* relative_frequency, uint_fast64_t total,
		uint_fast64_t* absolute_frequency);
void decrypt(FILE* encrypted_file, uint_fast8_t key, char* filepath_new);
void error(uint_fast8_t error_code);
void get_scores(float* scores8, float* relative_frequency);
void increment_character(wint_t* character, uint_fast8_t key);
