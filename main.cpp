#include <iostream>
using namespace std;
#include <pthread.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include <bits/stdc++.h>

double ** matrix1;
double ** matrix2;
double ** product;
static int totalthreads = 1;
static int eachthreadrows = 0;
int rows=0,rows2=0,columns=0,columns2=0;
int leftoutrows = 0;

struct prodrows {
   int i; 
};

void *multiplier(void *param) {
 struct prodrows * pr = (prodrows *)param;
 int size = pr->i;
 int leftoutindex = 0;
 if(leftoutrows!=0){
   if(size + eachthreadrows + 1 == rows){
     leftoutindex = leftoutrows;
  }
 }

 if(eachthreadrows > 1 && size != 0){
    size = size + eachthreadrows -1;
 }
 for(int k = size; k < size + eachthreadrows + leftoutindex; k++){
  for(int m = 0; m < columns2 ; m++){
   for(int n = 0; n < columns; n++){
      product[k][m] += matrix1[k][n] * matrix2[n][m];
     }
  }
}
   pthread_exit(0);
}

void printmatrix(double ** mat, int rows, int cols){
for(int i=0;i<rows;i++){
 for(int j=0; j<cols ; j++){
  cout << setw(10)<<mat[i][j];
 }
cout<<endl;
 }
 cout<<endl;
}

void printmatrixfileformat(double ** mat, int rows, int cols){
cout<< rows <<" "<< cols << endl;
 for(int i=0 ;i < rows; i++){
   cout<<"# Row "<< i << endl;
    for(int j=0 ;j < cols; j++){
     cout << mat[i][j] << endl;
       }
  } 
 cout<<endl;
}

int main(int argv, char * argc[]){
clock_t start, end;
 
        int i=0,j=0;  
        rows=0,rows2=0;
        columns=0,columns2=0;
        char value[50];
        totalthreads = atoi(argc[6]);        
        ifstream fin(argc[2]); 
     bool flag = true;
if(flag){       
		fin >> rows;
                fin >> columns;  
                fin.getline(value,50);
                fin.getline(value,50);
                matrix1 = new double * [rows];
                for(int l=0;l<rows;l++){
			matrix1[l] = new double[columns]; 
		}
		while (fin.getline(value,50)) {
                        if(value[0] != '#'){ 
			 matrix1[i][j] = atof(value);
			 j++;
                    } 
                         if(value[0] == '#'){
                      		j=0;
                                i++;
                       }  
		}
   cout << "Matrix A:"<<endl;
   printmatrix(matrix1,rows,columns);
}

i=0, j=0;
ifstream fin2(argc[4]);       
		fin2 >> rows2;
                fin2 >> columns2;  
                fin2.getline(value,50);
                fin2.getline(value,50);
                matrix2 = new double * [rows2];
                for(int l=0;l<rows2;l++){
			matrix2[l] = new double[columns2]; 
		}
		while (fin2.getline(value,50)) {
                        if(value[0] != '#'){ 
			 matrix2[i][j] = atof(value);
			 j++;
                    } 
                         if(value[0] == '#'){
                      		j=0;
                                i++;
                       }  
		}
    cout << "Matrix B:"<<endl;	
    printmatrix(matrix2,rows2,columns2);

if (totalthreads > rows || totalthreads == 0){
   cout<<"Matrices can not be Multiplied because number of threads are out of range!!"<<endl; 
}

else if(columns == rows2){   
        
 leftoutrows = rows % totalthreads;
  product = new double * [rows];
    for(int l=0;l<rows;l++){
	product[l] = new double[columns2]; 
}
  i=0,j=0;
  int count = 0;
  eachthreadrows = rows/totalthreads;
start = clock();
 for(i = 0; i < totalthreads ; i++) {
         struct prodrows *pr = (struct prodrows *) malloc(sizeof(struct prodrows));
         pr->i = i; 
         pthread_t id;
         pthread_attr_t attr; 
         pthread_attr_init(&attr);
         pthread_create(&id,&attr,multiplier,pr);
         pthread_join(id, NULL);
         count++;
   }
  
    // Recording the end clock tick. 
    end = clock(); 

        cout << "Matrix Product A*B: "<<endl;
	printmatrix(product,rows,columns2);
        printmatrixfileformat(product,rows,columns2);
        
}
  else {
  	cout<<"Matrices can not be Multiplied because their dimensions are not compatible!!"<<endl;
} 
  
    // Calculating total time taken by the program. 
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    cout << "Time taken by program is : " << fixed  
         << time_taken << setprecision(5); 
    cout << " sec " << endl;

return 0;
}
