#include<stdio.h>
#include<stdlib.h>

int* dilate_operation(int *img, int height, int width);

int main(int argc, char *argv[])
{
	if(argc <= 2)
	{
		printf("\nWrong format!!\n");
		exit(0);
	}

	FILE *op, *ip;
	ip = fopen(argv[1], "r");
	op = fopen(argv[2], "w");
	if(op == NULL || ip == NULL)
	{
		printf("\nFile could not be opened!!\n");
		exit(0);
	}

	int i, j; 
	int height, width, max_value = 1, temp;
	int *img, *dilate_oper; 
	char c, magic[2];

	fgets(magic, 3, ip);	
	fscanf(ip, "\n");		

	fscanf(ip, "%c", &c);			
	if(c == '#')								//skipping the commented line
		while(c != '\n') 
			c = fgetc(ip);
	else
		fseek(ip, -1, SEEK_CUR);

	fscanf(ip, "\n%d %d", &width, &height);
	fscanf(ip, "\n%d", &max_value);

	img = (int*)malloc(sizeof(int) * height * width);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			fscanf(ip, "%d\n", &temp);
			*(img + i * width + j) = temp;
		}
		if(feof(ip))
			break;
	}

	dilate_oper = dilate_operation(img, height, width);

	fprintf(op, "%s\n", magic);
	fprintf(op, "%d %d\n", width, height);
	fprintf(op, "%d\n", max_value);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			fprintf(op, "%d\n", *(dilate_oper + i*width + j));
		}
	}

	fclose(ip);
	ip = NULL;
	fclose(op);
	op = NULL;

	return 0;
}

int* dilate_operation(int *img, int height, int width)
{
	int *dilate_oper;
	int i, j, ki, kj, sh, sw, f;

	printf("\nEnter the dimensions of structuring element:\n");
	printf("Height: ");
	scanf("%d", &sh);
	printf("Width: ");
	scanf("%d", &sw);

	dilate_oper = (int*)malloc(sizeof(int) * height * width);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			f = 0;
			for(ki = i - sh/2; ki <= i + sh/2; ki++)
			{
				for(kj = j - sw/2; kj <= j + sw/2; kj++)
				{
					if(ki >= 0 && ki < height && kj >= 0 && kj < width)
					{
						if(*(img + ki*width + kj))						
							f = 1;
					}
				}
			}
			*(dilate_oper + i*width + j) = f;

		}
	}

	return dilate_oper;
}