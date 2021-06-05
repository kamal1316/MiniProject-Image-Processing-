#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define min(x,y) x<y?x:y
#define max(x,y) x>y?x:y

int mni, mxi, mnj, mxj;
int base_index, upper_index, med_height;

int* ReadFile(FILE *ip, int*, int*, char*, int*);
int* segment(int*, int, int);
void Remove_base_line(int*, int, int, int, int);
int sep_upper_region(int*, int*, int, int, int, int);
int find_head_line(int*, int, int, int, int);
void sep_char(int*, int*, int, int, int, int, int);
int* HPP(int*, int, int);

void conn_comp_label(int*, int*, int, int, int, int);
void setlabel( int*, int*, int, int, int, int, int);

int cmp(const void *x, const void *y)
{
	return ( *(int*)x - *(int*)y );
}

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
	int *img, *seg_img;
	char magic[4];

	img = ReadFile(ip, &height, &width, magic, &max_value);

	seg_img = segment(img, height, width);


	fprintf(op, "%s\n", magic);
	fprintf(op, "%d %d\n", width, height);
	fprintf(op, "%d\n", max_value);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			fprintf(op, "%d\n", *(seg_img + i*width + j));
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

int* segment(int *img, int height, int width)
{
	if(img == NULL)
	{
		printf("Empty image file!!\n");
		exit(0);
	}

	int i, j, in_row;
	int *seg_img, *hpp, *visited;
	
	seg_img = (int*)malloc(sizeof(int) * height * width);
	visited = (int*)malloc(sizeof(int) * height * width);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			*(seg_img + i*width + j) = *(img + i*width + j);
			*(visited + i*width + j) = 0;
		}
	}

	hpp = HPP(img, height, width);
	in_row = -1;
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
				int head_index = sep_upper_region(seg_img, hpp, height, width, in_row, i);

				Remove_base_line(seg_img, height, width, head_index, i);

				med_height -= 5;
				sep_char(seg_img, visited, height, width, upper_index, base_index, 5);

				in_row = -1;
			}
		}
	}
	return seg_img;
}

int sep_upper_region(int *seg_img, int *hpp, int height, int width, int in_row, int fn_row)
{
	int head_line, i, j;
	head_line = find_head_line(hpp, height, width, in_row, fn_row);
	for(i = head_line - 2; i <= head_line; i++)
	{
		for(j = 0; j < width; j++)
			*(seg_img + i*width + j) = 0;
	}
	return head_line;
}

void Remove_base_line(int *img, int height, int width, int in_row, int fn_row)
{
	int *label, i, j;
	label = (int*)malloc(sizeof(int) * height * width);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
			*(label + i*width + j) = 0;
	}

	conn_comp_label(img, label, height, width, in_row, fn_row);
}

void sep_char(int *img, int *visited, int height, int width, int in_row, int fn_row, int line_width)
{
	int i, j, ti, tj;

	for(j = 0; j < width; j++)
	{
		ti = fn_row;
		tj = j;
		
		while(ti > in_row + line_width)
		{
			if(*(img + ti*width + tj))
			{	
				ti++;
				if(tj - 1 >= 0 && !*(visited + ti * width + tj - 1))
					tj--;
				else if(tj + 1 >= 0 && !*(visited + ti * width + tj + 1))
					tj++;
				else
					break;
			}
			else
				ti--;
			*(visited + ti*width + tj) = 1;
		}
		if(fn_row - ti + 1 >= med_height)
		{
			for(i = in_row-1; i <= ti+1; i++)
			{
				*(img + i*width + tj) = 0;
				*(visited + i*width + tj) = 1;
			}
		}
	}
}

void conn_comp_label(int *img, int *label, int height, int width, int in_row, int fn_row)
{
	int i, j, t1, t2, l = 0;
	int mni, mnj, mxi, mxj;
	int *minx = NULL, *maxx = NULL;	
	int *m_height;

	mni = INT_MAX;
	mnj = INT_MAX;
	mxi = INT_MIN;
	mxj = INT_MIN;

	for(i = in_row; i <= fn_row; i++)
	{   
		for(j = 0; j < width; j++)
		{
			t1 = *(img + i * width + j);
			t2 = *(label + i * width + j);
			if(t2==0 && t1==1)
			{
				l++;
				setlabel(label, img, height, width, i, j, l);
			}
		}		
	}

	minx = (int*)malloc(sizeof(int) * l);
	maxx = (int*)malloc(sizeof(int) * l);
	m_height = (int*) malloc(sizeof(int) * l);

	for(i = 0; i < l; i++)
	{
		minx[i] = INT_MAX;
		maxx[i] = INT_MIN; 
		m_height[i] = 0;
	}

	for(i  = in_row; i <= fn_row; i++)
	{
		for(j = 0; j < width; j++)
		{
			t2 = *(label + i * width + j);
			if(t2 != 0)
			{
				minx[t2-1] = min(minx[t2-1], i);
				maxx[t2-1] = max(maxx[t2-1], i+1);
			}
		}
	}

	for(i = 0; i < l; i++)
		m_height[i] = maxx[i] - minx[i] + 1;

	qsort(minx, l, sizeof(int), cmp);
	qsort(maxx, l, sizeof(int), cmp);
	qsort(m_height, l, sizeof(int), cmp);

	med_height = m_height[l/2];

	base_index = maxx[l/2];

	upper_index = minx[l/2];

	for(j = 0; j < width; j++)
		*(img + base_index*width + j) = 0;

	free(minx); 
	free(maxx);
}

void setlabel(int *label, int *img, int height, int width, int i, int j, int l)
{
	mni = min(mni, i);
	mnj = min(mnj, j);
	mxi = max(mxi, i);
	mxj = max(mxj, j);

	*(label + i * width + j) = l;
	
	int temp, ii, jj, t1, t2;
	int mxi = max(0, i - 1), mni = min(height, i + 2);
	int mxj = max(0, j - 1), mnj = min(width, j + 2);
	
	for(ii = mxi; ii < mni; ii++)
	{
		for(jj = mxj; jj < mnj; jj++)
		{
			t1 = *(img + ii * width + jj);
			t2 = *(label + ii * width + jj);
			if(t1 == 1 && t2 == 0 ) setlabel(label, img, height, width, ii, jj, l);
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