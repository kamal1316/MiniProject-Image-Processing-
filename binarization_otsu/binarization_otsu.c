/*Program to convert grayscale image to binary image using Otsu's algorithm.

theory referred from wikipedia.
*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define max(x,y) x>y?x:y
typedef long long int lli;

lli arr[256];											// array to store the histogram.

int otsu(lli k)											//function to calculate threshold using otsu's technique.
{														//k= total no. of pixels in image
	int i,thres=-1;
	long double w0=0,w1=0,u0=0,u1=0,u10;								//w = class prob*k     u = mean
	long double temp=0.0,mx=0.0;										//mx hold the maximum value of sigma b till now.
	for(i=0;i<256;i++) 									//precomputing w1 and u1.
	{
		w1+=arr[i];
		u1+=(arr[i]*i);
	}
	u10=u1;
	for(i=0;i<256;i++)
	{
		w0+=arr[i];
		w1-=arr[i];
		u0+=(i*arr[i]);
		u1-=(i*arr[i]);
		if(w0!=0 && w0!=1)
		{
			long double x=(u0*w1-u1*w0);
			long double y=k*k*w0*w1;
			temp=(x*x)/y;			     // dividing at last so as to not lose precision.
			if(temp>=mx) 						//thres corresponds to that intensity value which maximizes the mx
			{
				mx=temp;
				thres=i;						
			}
		}
	}
	printf("\n%Ld\n",k);
	return thres;
}

int main(int argc,char *argv[])
{
	FILE *fp,*op;
	fp=fopen(argv[1],"r");
	op=fopen(argv[2],"wb");
	if(fp==NULL || op==NULL)
	{
		printf("The file could not be opened!!Aborting...");
		exit(0);
	}

	int i,j; 
	int width,height,max_value,*p,temp; 
	char c,magic[4];
	int thres;	

	for(i=0;i<256;i++) arr[i]=0;

	fgets(magic,3,fp);                   // reading a magic number , will stop reading when 3 bytes are read or EOF or \n is read. 
	fscanf(fp,"\n");					 // reading a newline character;	
	
	fscanf(fp,"%c",&c);
	if(c=='#')  while(c!='\n') c=fgetc(fp);			//skipping the commented line
	else fseek(fp,-1,SEEK_CUR);
	
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
			arr[temp]++;
		}		
		if(feof(fp)) break;
	}

	thres=otsu(height*width);					//thres will hold threshold value calculated using otsu's technique.

	max_value=1;								//binary max value is 1
	strcpy(magic,"P1");							//Its magic number is P1.
	fprintf(op,"%s\n",magic);
	fprintf(op,"%d %d\n",width,height);
	//fprintf(op,"%d\n",max_value);             //not necessary to print max_value for binary image

	for(i=0;i<height;i++)
	{   
		for(j=0;j<width;j++)
		{
			if(*(p+i*width+j)<=thres) temp=1;			//dividing all the pixels into two classes using
			else temp=0;								//threshold value.(this is the basic idea of binarization).
			fprintf(op,"%d\n",temp);
		}		
	}

	printf("The threshold value calculated using otsu's technique is: %d\n",thres);
	fclose(fp); 
	fclose(op);
	return 0;
}	