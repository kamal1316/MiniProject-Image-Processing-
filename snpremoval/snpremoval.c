// https://www.geeksforgeeks.org/salt-and-pepper-noise-removal-using-c/
#include <stdio.h> 
#include<stdlib.h>

#define min(x,y) x<y?x:y
#define max(x,y) x>y?x:y

int comparator(const void *x,const void *y)
{
	return (*(int*)x-*(int*)y);
}

int main(int argc,char *argv[]) 
{ 
	FILE *fp,*op; 
	fp=fopen(argv[1],"r");
	op=fopen(argv[2],"w"); 

	if(fp==NULL || op==NULL)
	{
		printf("The file could  not be opened!!\n");
		exit(0);
	}

	int i,j,k,temp=0,x,ii,jj,y; 
	int width,height,max_value; 
	char c,magic[4],str[5000];
	int *p;

	printf("Which size window do you want to use? ");
	scanf("%d",&x);

	fgets(magic,3,fp);                   // reading a magic number , will stop reading when 3 bytes are read or EOF or \n is read. 
	fscanf(fp,"\n");					 // reading a newline character;	
	
	while(c!='\n') c=fgetc(fp);			//skipping the commented line
	
	fscanf(fp,"\n%d %d",&width,&height); //reading width and height of image file 
	fscanf(fp,"\n%d",&max_value); 		 //reading maximum value that can be present contents of file

	p=(int*)malloc(height*width*sizeof(int));
	int w=0,h=0; 
	
	for(i=0;i<height;i++) 					// loop for reading image file and storing in array.
	{   
		for(j=0;j<width;j++)
		{
			fscanf(fp,"%d\n",&temp);
			*(p+i*width+j)=temp;
		}		
		if(feof(fp)) break;
	}
	
	fprintf(op,"%s\n",magic);
	fprintf(op,"%d %d\n",width,height);
	fprintf(op,"%d\n",max_value);

	x=x/2;

	// code snippet for snp removal using median
	//int arr[50];
	// for(i=0;i<height;i++) 
	// {   
	// 	for(j=0;j<width;j++)
	// 	{
	// 		temp=0;
	// 		y=0;
	// 		int mxi=max(0,i-x),mni=min(height,i+x+1),mxj=max(0,j-x),mnj=min(width,j+x+1);		// constraints to window
	// 		for(ii=mxi;ii<mni;ii++)
	// 		{
	// 			for(jj=mxj;jj<mnj;jj++)
	// 			{
	// 				arr[y++]=*(p+ii*width+jj);
	// 			}	
	// 		}
	// 		qsort(arr,y,sizeof(int),comparator);
	// 		temp=arr[y/2-1];
	// 		fprintf(op,"%d\n",temp);
	// 	}	
	// }

	// code snippet for snp removal using average.
	for(i=0;i<height;i++) 
	{   
		for(j=0;j<width;j++)
		{
			temp=0;
			y=0;
			int mxi=max(0,i-x),mni=min(height,i+x+1),mxj=max(0,j-x),mnj=min(width,j+x+1);		// constraints to window
			for(ii=mxi;ii<mni;ii++)
			{
				for(jj=mxj;jj<mnj;jj++)
				{
					temp+=*(p+ii*width+jj);
					y++;
				}	
			}
			temp=temp/y;
			fprintf(op,"%d\n",temp);
		}	
	}

	fclose(op);
	fclose(fp);
	
	return 0; 
} 
