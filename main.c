#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include "fact.h"

#define DESCC 3
#define FLAGC 2
#define VERSC 2
#define BUFSIZE 64

#define EXIT_HELP 2

#define FGRED "\x1B[31m"
#define FGNORMAL "\x1B[0m"
#define FTUNDERLINE "\x1B[4m"

#define OPENMODE "w+" 


static FILE* ostream = 0; 
static char* pathname = 0;
static unsigned long long results[BUFSIZE];

static char* flags[FLAGC][DESCC] = {
	{ "-h", "--help", "Show help page", },
	{ "-o", "--output", "Path to output file, caution: this will\n\t\t\toverwrite file contents!" },
};


int main(int argc, char** argv) {
	int parse_status = parse(argc, argv);
	ostream = stdout; 

	switch (parse_status) {
		case EXIT_HELP: 
			help();
			goto exit_success;
			break; 
		case EXIT_FAILURE: 
			goto exit_failure; 
			break;
	}
	
	int buflen = 0; 
	if (optind < argc) { 
		int i = 0;
		while (optind < argc && i != BUFSIZE) {
			if (strnumeric(argv[optind]) == 0) {
				results[i++] = factorial(atoll(argv[optind]));
				++buflen;
			}
			++optind;
		}
	} 	
	if (!buflen) {
		arg_error();
	} else {
		if (pathname != NULL) {
			ostream = fopen(pathname, OPENMODE);
			if (ostream == NULL) {
				file_open_error(pathname);
				goto exit_failure; 
			}
		} 
		for (int i = 0; i < buflen; ++i) {
			fprintf(ostream, "%llu\n", results[i]);
		}
		fclose(ostream);
	}

	exit_success:
	exit(EXIT_SUCCESS);

	exit_failure:
	exit(EXIT_FAILURE);
}

int parse(const int argc, char* argv[]) {
	int STATUS = 0;
	int c;
	
	static char* short_options = "ho:";
	static struct option long_options[] = {
		{ "help",   no_argument,       0, 'h' },
		{ "output", required_argument, 0, 'o' },
		{ 0, 	    0, 		           0,  0  }
	};

	
	while(1) {
		int option_index = 0; 

		c = getopt_long(argc, argv, short_options, long_options, &option_index);

		if (c == -1) {
			break; 
		}

		switch (c) {
			case 'h': 
				STATUS = EXIT_HELP;
				goto exit; 
			case 'o':
				pathname = optarg; 
				break;
			case '?':
				STATUS = EXIT_FAILURE; 
				goto exit;
			default: 
				printf("?? getopt returned character code 0x%x ??\n", c);
				STATUS = EXIT_FAILURE;
				goto exit; 
		}
	}
	exit:
	return STATUS;
}

unsigned long long factorial(unsigned long long number) {
	unsigned long long res = 1; 
	for (; number > 1; number--) {
		res *= number;
	}
	return res;
}

void help() {
	printf("fact - Get factorial of an unsigned(positive) integer and\n");
	printf("       print it on standard output or write in file with -o option\n");
	putchar('\n');
	printf("Usage: fact [OPTION...] [INTEGER]\n");
	putchar('\n');

	for (int i = 0; i < FLAGC; ++i) {
		printf("\t%s %s \t%s\n", flags[i][0], flags[i][1], flags[i][2]);
	}
}

int strnumeric(const char *str) {
	for (int i = 0; str[i] != '\0'; ++i) {
		if (!isdigit(str[i])) {
			return -1; 
		}
	}
	return 0;
}

void arg_error() {
	printf("%serror: %s", FGRED, FGNORMAL);
	printf("expected unsigned integer (see: %sfact --help%s)\n", FTUNDERLINE, FGNORMAL);
}

void file_open_error(const char* optarg) {
	printf("%serror: %s", FGRED, FGNORMAL);
	printf("%s can't open such file or directory\n", optarg); 
}
