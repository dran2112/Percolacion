#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define P     16             // 1/2^P, P=16
#define Z     27000          // iteraciones
#define N     5            // lado de la red simulada


void llenar(int *red, int n, float prob);
void imprimir(int *red, int n);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red,int n);


int main(){
	int i, j, *red, *red_ini, n, z;
	float prob, denominador;
	n=N;
  	z=Z;
	red = (int *)malloc(n*n*sizeof(int));
  red_ini = (int *)malloc(n*n*sizeof(int)); 
/*
  for(i=0;i<z;i++)
    {*/
  prob=0.5;
  denominador=2.0;
 
  srand(time(NULL));
/*
      for(j=0;j<P;j++) //no menos de 13 iteraciones (P>13)
        {*/
  llenar(red,n,prob);

  for(i=0;i<n*n;i++) red_ini[i] = red[i]; 
  hoshen(red,n);      
  //      denominador=2.0*denominador;
/*
          if (percola(red,n)) 
             prob+=(-1.0/denominador); 
          else prob+=(1.0/denominador);

		//ir guardando en cada paso prob/N que percolan 
       }
    }
*/

	imprimir(red_ini,n);
	printf("\n \n");
  imprimir(red, n);
	free(red);
	return 0;
}

void llenar(int *red, int n, float prob){
	int i;
	float res;
	for(i=0; i<n*n; i++) {
		red[i] = 0;
		res = (float)rand()/(float)RAND_MAX;
		if(res<prob){
			red[i] = 1;
		}
	}
}

void imprimir(int *red, int n){
	int i, j;
	for(i=0; i<n; i++){ 
		for(j=0; j<n; j++){ 
			printf("%i ", red[i*n+j]); //Si le pongo \t deja un espacio
		}
	printf("\n");
	}
}

int   actualizar(int *red,int *clase,int s,int frag){
	if(!s){
		*red = frag;
		clase[frag] = frag;
		return frag+1;
	}
	else if(s){
		*red = s;
		return frag;
	}
}

int hoshen(int *red,int n){
  /*
    Esta funcion implementa en algoritmo de Hoshen-Kopelman.
    Recibe el puntero que apunta a la red y asigna etiquetas 
    a cada fragmento de red. 
  */
  int i,j,k,s1,s2,frag;
  int *clase;

  frag=0;
  
  clase=(int *)malloc(n*n*sizeof(int)); //vector de etiquetas

  for(k=0;k<n*n;k++) *(clase+k)=frag; //esto es: clase[k]=frag
  
  // primer elemento de la red

  s1=0; 
  frag=2; //1era etiqueta
  if (*red) frag=actualizar(red,clase,s1,frag);
  
  // primera fila de la red

  for(i=1;i<n;i++) //si el anterior es 1 y este tambien, van juntos
    {
      if (*(red+i)) //if red[i]=1 
         {
           s1=*(red+i-1);  //s1 es el anterior al que miramos
           frag=actualizar(red+i,clase,s1,frag);
         }
    }
  

  // el resto de las filas 

  for(i=n;i<n*n;i=i+n) //si el de arriba es 1 y este tambien, van juntos
    {

      // primer elemento de cada fila
      if (*(red+i)) 
         {
           s1=*(red+i-1); //el de arriba
           frag=actualizar(red+i,clase,s1,frag);
         }
      if (*(red+i)) 
         {
           s1=*(red+i-n); //el de arriba
           frag=actualizar(red+i,clase,s1,frag);
         }

      for(j=1;j<n;j++){
				if (*(red+i+j))
	  		{
	    			s1=*(red+i+j-1); 
            s2=*(red+i+j-n);

	    	if (s1*s2>0)
	      {
					etiqueta_falsa(red+i+j,clase,s1,s2);
	     	}
	    	else 
	      { if (s1!=0) frag=actualizar(red+i+j,clase,s1,frag);
	        else       frag=actualizar(red+i+j,clase,s2,frag);
	      }
	  		}
   		}

    }


  corregir_etiqueta(red,clase,n);

  free(clase);

  return 0;
}


void etiqueta_falsa(int *red, int *clase, int s1, int s2){  // pone etiquetas falsas en vector auxiliar "clase"

		if(s2 >= s1){
			 clase[s2] = -s1;
			 *red = s1;		
		}
		else if (s2 <= s1){
			 clase[s1] = -s2;	
			 *red = s2;	
		}
}

void corregir_etiqueta(int *red, int *clase, int n){  // corrije etiquetas en la red.

	int i,s;
	for(i=0;i<n*n;i++){
		s = red[i]; 
		while(clase[s]<0) s = -clase[s];
		red[i] = s;
	
	}

/*	int s0, s1, s2, i, j;
	for(i=1; i<n; i++){ 
		for(j=1; j<n; j++){ 

			if( red[i*n+j] && (red[i*n+j-n] || red[i*n+j-1]) ){
				s1 = red[i*n+j-n];  s2 = red[i*n+j-1]; s0 = red[i*n+j];
				while(clase[s1]<0)  s1 = -clase[s1];
				while(clase[s2]<0)  s2 = -clase[s2];
			
				if(s1<=s2 && s1>0){
					clase[s2] = -s1;
					red[i*n+j] = s1;
					red[i*n+j-1] = s1;
					red[i*n+j-n] = s1;
				}
				else if(s2<=s1 && s2>0){
					clase[s1] = -s2;
					red[i*n+j] = s2;
					red[i*n+j-1] = s2;
					red[i*n+j-n] = s2;
				}		
				else if(s0 < s1){
					clase[s1] = -s0;
					red[i*n+j-n] = s0;
				} 
			}
		}
	}*/
}






