#include<stdio.h>
#include<stdlib.h>

int* ReadFile(FILE *ip, int*, int*, char*, int*);
void Remove_head_line(int *img, int *Remove_head_line_img, int *hpp, int height, int width);
int* HPP(int *img, int height, int width);
int find_head_line(int *hpp, int height, int width, int in_row, int fn_row);
void copy_ith_line(int *src, int *dest, int height, int width, int in_row, int fn_row);
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

	int *img;
	img = ReadFile(ip, &height, &width, magic, &max_value);

	int *Remove_head_line_img, *hpp;
	Remove_head_line_img = (int*)malloc(sizeof(int) * height * width);
	hpp = HPP(img, height, width);

	Remove_head_line(img, Remove_head_line_img, hpp, height, width);

	fprintf(op, "%s\n", magic);
	fprintf(op, "%d %d\n", width, height);
	fprintf(op, "%d\n", max_value);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			fprintf(op, "%d\n", *(Remove_head_line_img + i*width + j));
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

void Remove_head_line(int *img, int *Remove_head_line_img, int *hpp, int height, int width)
{
	if(img == NULL)
	{
		printf("Empty array!!\n");
		exit(0);
	}

	int  i, in_row = -1, head_line = -1;

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
				head_line = find_head_line(hpp, height, width, in_row, i);
				copy_ith_line(img, Remove_head_line_img, height, width, head_line-2, head_line-1);
				Diff(img, Remove_head_line_img, height, width, in_row, i);
				in_row = -1;
			}
		}
	}
}

int find_head_line(int *hpp, int height, int width, int in_row, int fn_row)
{
	if(hpp == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}
	int i, imx = -1;
	long long int avg = 0; 

	for(i = in_row; i <= fn_row; i++)
		avg += hpp[i];
	avg = avg / (fn_row-in_row+1);

	for(i = in_row; i <= fn_row; i++)
	{
		if(hpp[i] >= avg)
		{
			imx = i;
			break;
		}
	}
	return imx;
}

void copy_ith_line(int *src, int *dest, int height, int width, int in_row, int fn_row)
{
	if(src == NULL || dest == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}
	int i, j;
	for(i = in_row; i <= fn_row; i++)
	{
		for(j = 0; j < width; j++)
			*(dest + i*width + j) = *(src + i*width + j);
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