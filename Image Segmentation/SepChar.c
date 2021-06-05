#include<stdio.h>
#include<stdlib.h>

int* ReadFile(FILE *ip, int*, int*, char*, int*);
void Sep_char(int *img, int *Sep_char_img, int *hpp, int height, int width);
int* HPP(int *img, int height, int width);
int* VPP(int *img, int widht, int in_row, int fn_row);
int find_head_line(int *hpp, int height, int width, int in_row, int fn_row);
void copy_ith_line(int *src, int *dest, int height, int width, int in_row, int fn_row);
void Diff(int *img1, int *img2, int height, int width, int in_row, int fn_row);
void separate(int *Sep_char_img, int *vpp, int width, int in_row, int fn_row, int sizeof_head_line);
int find_sizeof_head_line(int *img, int *vpp, int width, int head_line, int fn_row);

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

	int *Sep_char_img, *hpp;
	Sep_char_img = (int*)malloc(sizeof(int) * height * width);
	hpp = HPP(img, height, width);

	Sep_char(img, Sep_char_img, hpp, height, width);

	fprintf(op, "%s\n", magic);
	fprintf(op, "%d %d\n", width, height);
	fprintf(op, "%d\n", max_value);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			fprintf(op, "%d\n", *(Sep_char_img + i*width + j));
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

int* VPP(int *img, int width, int in_row, int fn_row)
{
	int *VPParr, i, j, count;
	VPParr = (int*)malloc(sizeof(int) * width);
	for(j = 0; j < width; j++)
	{
		count = 0;
		for(i = in_row; i <= fn_row; i++)
			count += *(img + i*width + j);
		VPParr[j] = count;
	}
	return VPParr;
}

void Sep_char(int *img, int *Sep_char_img, int *hpp, int height, int width)
{
	if(img == NULL)
	{
		printf("Empty array!!\n");
		exit(0);
	}

	int  i, in_row = -1, j, head_line, sizeof_head_line;
	int *vpp;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
			*(Sep_char_img + i*width + j) = *(img + i*width + j);
	}

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
				vpp = VPP(img, width, in_row, i);
				head_line = find_head_line(hpp, height, width, in_row, i);
				sizeof_head_line = find_sizeof_head_line(img, vpp, width, head_line, i);
				separate(Sep_char_img, vpp, width, in_row, i, sizeof_head_line);
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
			*(img2 + i * width + j) = *(img1 + i * width + j) - *(img2 + i * width + j); 

	}
}

void separate(int *Sep_char_img, int *vpp, int width, int in_row, int fn_row, int sizeof_head_line)
{
	int i, j;
	for(j = 0; j < width; j++)
	{
		if(vpp[j] <= sizeof_head_line)
		{
			for(i = in_row; i <= fn_row; i++)
			{
				*(Sep_char_img + i*width + j) = 0;
			}
		}	
	}
}

int find_sizeof_head_line(int *img, int *vpp, int width, int head_line, int fn_row)
{
	int i, j, avg = 0, count = 0;
	
	for(j = 0; j < width; j++)
	{
		if(vpp[j] > 0)
		{
			for(i = head_line; i <= fn_row; i++)
			{
				if(!*(img + i*width + j))
					break;
				avg++;		
			}
			count++;
		}	
	}

	avg /= count;
	return avg;
}