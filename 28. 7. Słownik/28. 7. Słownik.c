#include "dictionary.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
int main()
{
	char * filename, *dsa, *dso;
	int tryb;
	filename = (char*)malloc(41);
	if (filename == NULL)
	{
		printf("Failed to allocate memory");
		return 4;
	}
	printf("Podaj nazwe pliku");
	fgets(filename, 41, stdin);
	strtok(filename, "\n");
	int err_code;
	printf("Podaj tryb pracy 0-Wczyt slownika z p. bin. 1-Wczyth slow z p. i utwo slownika.");
	
	scanf("%i", &tryb);
	
	struct dictionary_t* d;
	
	if(tryb == 0)
	{ 
		d = load_dictionary_b(filename, &err_code);
		if(err_code == 4)
		{
			printf("Failed to allocate memory");
			free(filename);
			return 4;
		}
		if (err_code == 2)
		{
			printf("File not found");
			free(filename);
			return 2;
		}
		if (err_code == 3)
		{
			printf("File corrupted");
			free(filename);
			return 3;
		}
	
	}
	else if (tryb == 1)
	{
		FILE *f = fopen(filename, "rt");
		if (f == NULL)
		{
			free(filename);
			printf("File not found");
			return 2;
		}
		char * buf;
		buf = (char*)malloc(41);
		if (buf == NULL)
		{
			free(filename);
			fclose(f);
			printf("Failed to allocate memory");
			return 4;
		}
		d = create_dictionary(10);
		if (d == NULL)
		{
			free(filename);
			free(buf);
			printf("Failed to allocate memory");
			fclose(f);
			return  4;
		}
		int test;
		//
		while (!feof(f))
		{
			test = fscanf(f, "%s", buf);
			if (!test || feof(f))
				break;
			strtok(buf, ".");
			strtok(buf, ",");
			strtok(buf, "?");
			strtok(buf, "!");
			//if(!strcmp(filename, "dolor"))
			//{
			//  ei++;
			//  }
			test = dictionary_add_word(d, buf);

			if (test == 2)
			{
				free(filename);
				free(buf);
				printf("Failed to allocate memory");
				fclose(f);
				destroy_dictionary(&d);
				return 4;
			}
		}
		free(buf);
		fclose(f);
		//smieciarz
		//destroy_dictionary(&dic);
		//free(filename);
	}
	else
	{
		printf("Incorrect input");
		free(filename);
		return 1;
	}


	dsa = (char*)malloc(41);
	if (dsa == NULL)
	{
		destroy_dictionary(&d);
		free(filename);
		printf("Failed to allocate memory");
		return 4;
	}
	dso = (char*)malloc(41);
	if(dso == NULL)
	{
		free(dsa);
		destroy_dictionary(&d);
		free(filename);
		printf("Failed to allocate memory");
		return 4;
	}
	if (strstr(filename, ".txt"))
	{
		strncpy(dsa, filename, 40);
		strncpy(dsa + strlen(filename) - 4, "_alpha", 7);
		strncpy(dsa + strlen(dsa), ".txt", 40 - 4 - strlen(filename));

		strncpy(dso, filename, strlen(filename));
		strncpy(dso + strlen(filename) - 4, "_occ", 4);
		strncpy(dso + +strlen(dso), ".txt", 40 - 4 - strlen(filename));
	}
	else if(strstr(filename, ".bin"))
	{
		strncpy(dsa                       , filename, 40);
		strncpy(dsa + strlen(filename) - 4, "_alpha", 7);
		strncpy(dsa + strlen(dsa)         , ".bin"	, 40 - 4 - strlen(filename));
		
		strncpy(dso                       , filename, strlen(filename));
		strncpy(dso + strlen(filename) - 4, "_occ", 4);
		strncpy(dso + +strlen(dso),         ".bin", 40 - 4 - strlen(filename));
	}
	dictionary_sort_occurence(d);
	err_code = save_dictionary_b(d, dso);
	if (err_code != 0)
	{
		printf("Couldn't create file\n");
	}
	else 
		printf("File saved\n");
	
	dictionary_sort_alphabetically(d);
	err_code = save_dictionary_b(d, dsa);
	if (err_code != 0)
	{
		printf("Couldn't create file\n");
	}
	else
		printf("File saved\n");
	free(dsa);
	free(dso);
	free(filename);
	destroy_dictionary(&d);
	
	return 0;
}

