//program to label the connected pixels(8-connectivity)
#include <stdio.h> 
#include<stdlib.h>
#include<limits.h>

#define min(x,y) x<y?x:y
#define max(x,y) x>y?x:y

int width, height;												 //image dimensions
int *p = NULL, *label = NULL;
int *minx = NULL, *miny = NULL, *maxx = NULL, *maxy = NULL;		//array to hold the rectangular corrdinates for each label
int mni, mxi, mnj, mxj;
void conn_comp_label();
void setlabel(int i, int j, int l);
void alloc(int **p, int size, int value);

int main(int argc, char *argv[]) 
{ 
	FILE *fp;
	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		printf("The file could not be opened!!Aborting...");
		exit(0);
	}

	int i, j; 
	int max_value, temp; 
	char c, magic[4];

	fgets(magic, 3, fp);                   			// reading a magic number , will stop reading when 3 bytes are read or EOF or \n is read. 
	fscanf(fp, "\n");					 			// reading a newline character;	
	
	fscanf(fp, "%c", &c);
	if(c == '#')  while(c != '\n') c = fgetc(fp);			//skipping the commented line
	else fseek(fp, -1, SEEK_CUR);
	
	fscanf(fp, "\n%d %d", &width, &height); 			//reading width and height of label file 
	//fscanf(fp, "\n%d", &max_value); 		 			//reading maximum value that can be present contents of file
	
	p = (int*) malloc(height * width * sizeof(int));
	label = (int*) malloc(height * width * sizeof(int));

	for(i = 0; i < height; i++) 							// loop for reading label file and storing in array.
	{   
		for(j = 0; j < width; j++)
		{
			fscanf(fp, "%d\n", &temp);
			*(p + i * width + j) = temp;
			*(label + i * width + j) = 0;
		}		
		if(feof(fp)) break;
	}

	conn_comp_label(p);
	
	printf("\n");
	for(i = 0; i < height; i++)
	{   
		for(j = 0; j < width; j++)
		{
			temp = *(label + i * width + j);
			// printf("%d ", temp);
		}		
		// printf("\n");
	}	

	fclose(fp);
	return 0; 
} 

void conn_comp_label()
{
	int i, j, t1, t2, l = 0;
	
	mni = INT_MAX;
	mnj = INT_MAX;
	mxi = INT_MIN;
	mxj = INT_MIN;

	for(i = 0; i < height; i++)
	{   
		for(j = 0; j < width; j++)
		{
			t1 = *(p + i * width + j);
			t2 = *(label + i * width + j);
			if(t2==0 && t1==1)
			{
				//if(i < mni || j < mnj || i > mxi || j > mxj)	l++;
				l++;
				setlabel(i, j, l);
			}
		}		
	}

	printf("\nThe Number of components in image file is %d\n", l);

	alloc(&minx, l + 1, INT_MAX);
	alloc(&miny, l + 1, INT_MAX);
	alloc(&maxx, l + 1, INT_MIN);
	alloc(&maxy, l + 1, INT_MIN);

	for(i  = 0; i < height; i++)
	{
		for(j = 0;j < width; j++)
		{
			t2 = *(label + i * width + j);
			if(t2 != 0)
			{
				minx[t2] = min(minx[t2], i);
				miny[t2] = min(miny[t2], j);
				maxx[t2] = max(maxx[t2], i+1);
				maxy[t2] = max(maxy[t2], j+1);
			}
		}
	}

	// printf("\n");
	// for(i = 1; i <= l; i++) printf("%d ", minx[i]);
	// printf("\n");
	// for(i = 1; i <= l; i++) printf("%d ", miny[i]);
	// printf("\n");
	// for(i = 1; i <= l; i++) printf("%d ", maxx[i]);
	// printf("\n");
	// for(i = 1; i <= l; i++) printf("%d ", maxy[i]);
	// printf("\n");

	// printf("\nCoordinates for labelling: \n");
	int avw = 0, avh = 0;
	for(i = 1; i <= l; i++) 
	{
		// printf("\nlabel: %d\nTop Left: (%d,%d)\nBottomRight: (%d,%d)\n", i, minx[i], miny[i], maxx[i], maxy[i]);
		avw += (maxy[i] - miny[i]);
		avh += (maxx[i] - minx[i]);
	}
	avw = avw/l;
	avh = avh/l;
	printf("Average Width: %d\nAverage height: %d", avw, avh);
}

void setlabel(int i, int j, int l)
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
			t1 = *(p + ii * width + jj);
			t2 = *(label + ii * width + jj);
			if(t1 == 1 && t2 == 0 ) setlabel(ii, jj, l);
		}	
	}
}

void alloc(int **p, int size, int value)
{
	*p = malloc(sizeof(int)*size);
	for(int i = 1; i <= size; i++) *(*p+i) = value;
}
