#include<stdio.h>
#include<stdlib.h>

int* ReadFile(FILE *ip, int*, int*, char*, int*);
int* Final(int *img, int height, int width);
int* HPP(int *img, int height, int width);
int* VPP(int *img, int widht, int in_row, int fn_row);
int find_max_line(int *hpp, int height, int width, int in_row, int fn_row);
void copy_ith_line(int *src, int *dest, int height, int width, int line_no, int sizeof_head_line);
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


	int *final_img = Final(img, height, width);


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

int* Final(int *img, int height, int width)
{
	if(img == NULL)
	{
		printf("Empty array!!\n");
		exit(0);
	}

	int *final_img, *hpp;
	int  i, in_row = -1, line_no = -1;
	int sizeof_head_line, *vpp;

	final_img = (int*)malloc(sizeof(int) * height * width);

	hpp = HPP(img, height, width);

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
				line_no = find_max_line(hpp, height, width, in_row, i);
				sizeof_head_line = find_sizeof_head_line(img, vpp, width, line_no, i);
				copy_ith_line(img, final_img, height, width, line_no, sizeof_head_line);
				in_row = -1;
			}
		}
	}

	return final_img;
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

int find_max_line(int *hpp, int height, int width, int in_row, int fn_row)
{
	if(hpp == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}
	int i, mx = 0, imx = -1;
	for(i = in_row; i <= fn_row; i++)
	{
		if(hpp[i] > mx)
		{
			imx = i;
			mx = hpp[i];
		}
	}
	return imx;
}

void copy_ith_line(int *src, int *dest, int height, int width, int line_no, int sizeof_head_line)
{
	if(src == NULL || dest == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}
	int i, j;
	for(i = line_no - sizeof_head_line/2; i <= line_no + sizeof_head_line/2; i++)
	{
		if(i >= 0 && i < height)
		{
			for(j = 0; j < width; j++)
				*(dest + i*width + j) = *(src + i*width + j);
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