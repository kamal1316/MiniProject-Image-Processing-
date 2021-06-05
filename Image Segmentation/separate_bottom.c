#include<stdio.h>
#include<stdlib.h>

int* ReadFile(FILE *ip, int*, int*, char*, int*);
void Remove_bottom_line(int *img, int *Remove_bottom_line_img, int *hpp, int height, int width);
int* HPP(int *img, int height, int width);
int find_bottom_line(int *img, int height, int width, int in_row, int fn_row, int count_call);
void copy_lines(int *src, int *dest, int height, int width, int in_row, int fn_row);
void Diff(int *img1, int *img2, int height, int width, int in_row, int fn_row);


int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("\nWrong format!!\n");
		exit(0);
	}

	FILE *op, *ip;
	ip = fopen(argv[1], "r");
	op = fopen(argv[2], "w");
	if(ip == NULL || op == NULL)
	{
		printf("\nFile could not be opened!!\n");
		exit(0);
	}

	int i, j;
	int height, width, max_value;
	char magic[4];

	int *img, *hpp, *final_img;
	img = ReadFile(ip, &height, &width, magic, &max_value);

	hpp = HPP(img, height, width);
	final_img = (int*)malloc(sizeof(int) * height * width);
	Remove_bottom_line(img, final_img, hpp, height, width);


	fprintf(op, "%s\n", magic);
	fprintf(op, "%d %d\n", width, height);
	fprintf(op, "%d\n", max_value);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			fprintf(op, "%d\n", *(final_img + i*width + j));
		}
	}


	fclose(ip);
	ip = NULL;
	fclose(op);
	op = NULL;

	return 0;
}

int* ReadFile(FILE *ip, int *height, int *width, char *magic, int *max_value)
{
	if(ip == NULL)
	{
		printf("File pointer = NULL\n");
		exit(0);
	}
	int i, j; 
	int temp;
	int *img; 
	char c;

	fgets(magic, 3, ip);	
	fscanf(ip, "\n");		
	fscanf(ip, "%c", &c);			
	if(c == '#')
		while(c != '\n') 
			c = fgetc(ip);
	else
		fseek(ip, -1, SEEK_CUR);

	fscanf(ip, "\n%d %d", width, height);
	fscanf(ip, "\n%d", max_value);

	img = (int*)malloc(sizeof(int) * *height * *width);

	for(i = 0; i < *height; i++)
	{
		for(j = 0; j < *width; j++)
		{
			fscanf(ip, "%d\n", &temp);
			*(img + i * *width + j) = temp;
		}
		if(feof(ip))
			break;
	}
	return img;
}

int* HPP(int *img, int height, int width)
{
	if(img == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}

	int *HPParr, i, j, count;
	HPParr = (int*)malloc(sizeof(int) * height);
	for(i = 0; i < height; i++)
	{
		count = 0;
		for(j = 0; j < width; j++)
			count += *(img + i*width + j);
		HPParr[i] = count;
	}
	return HPParr;
}

void Remove_bottom_line(int *img, int *Remove_bottom_line_img, int *hpp, int height, int width)
{
	if(img == NULL)
	{
		printf("Empty array!!\n");
		exit(0);
	}

	int  i, in_row = -1, bottom_line = -1;

	for(i = 0; i < height; i++)
	{
		if(hpp[i] > 0)
		{
			if(in_row == -1)
				in_row = i;
		}
		else
		{
			if(in_row != -1)
			{	
				bottom_line = find_bottom_line(img, height, width, in_row, i, 0);
				copy_lines(img, Remove_bottom_line_img, height, width, bottom_line, bottom_line+1);
				Diff(img, Remove_bottom_line_img, height, width, in_row, i);
				in_row = -1;
			}
		}
	}
}

int find_bottom_line(int *img, int height, int width, int in_row, int fn_row, int count_call)
{
	int i, j, count = 0;
	long long int avg = 0;

	for(j = 0; j < width; j++)
	{
		for(i = fn_row; i >= in_row; i--)
		{
			if(*(img + i*width + j))
			{
				count++;
				avg += i;
				break;
			}
		}
	}
	if(count)
		avg = avg / count;

	if(count_call < 3)
		avg = find_bottom_line(img, height, width, avg, fn_row, count_call + 1);

	return avg;
}

void copy_lines(int *src, int *dest, int height, int width, int in_row, int fn_row)
{
	if(src == NULL || dest == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}
	int i, j;
	for(i = in_row; i <= fn_row; i++)
	{
		if(i >= 0 && i < height)
		{
			for(j = 0; j < width; j++)
				*(dest + i*width + j) = *(src + i*width + j);
		}
	}
}

void Diff(int *img1, int *img2, int height, int width, int in_row, int fn_row)
{
	if(img1 == NULL || img2 == NULL)
	{
		printf("Empty image\n");
		exit(0);
	}
	int i, j;

	for(i = in_row; i <= fn_row; i++)
	{
		for(j = 0; j < width; j++)
		{
			if(*(img1 + i * width + j))
				*(img2 + i * width + j) = *(img1 + i * width + j) - *(img2 + i * width + j); 
		}

	}
}