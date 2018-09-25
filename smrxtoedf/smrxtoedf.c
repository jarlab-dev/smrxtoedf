// smrxtoedf.c : file conversion utility from spike2 to eds data format

#include <stdio.h>
#include <string.h>
#include <smrx2edf.h>
// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	//input argument handler
	if (argc != 3)
	{
		printf("usage: smrx2edf <intput filename> <output_filename> \n");
		return 1;
	}

	char *fin_name = argv[1];
	char *fout_name = argv[2];
	
	FILE *file;
	if (!(file = fopen(fin_name, "r")))
	{
		printf("File %s doesn't exist.\n", fin_name);
		fclose(file);
		return 1;
	}
	fclose(file);

	int convert = smrx2edf(fin_name, fout_name);
	return convert;
}

	