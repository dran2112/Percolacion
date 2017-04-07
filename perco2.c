#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define P 16             // 1/2^P, P=16
#define Z 27000          // iteraciones
#define N 5             // lado de la red simulada


void llenar(int *red, int n, float prob);
void imprimir(int *red, int n);
int numeros(float p); // meter en llenar
void  actualizar(int *red, int *clase, int frag, int n);
void  etiqueta_falsa(int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
void hoshen(int *red,int n);
//int   percola(int *red,int n);


int main(){
	int n, z, *red, *red_ini;
	float prob, denominador;
	n=N;
   z=Z;
	prob = 0.5;
	red = (int *)malloc(n*n*sizeof(int));
	red_ini = (int *)malloc(n*n*sizeof(int));   // red con 1 y 0
   srand(time(NULL));
   llenar(red,n,prob);
	red_ini = red;
	imprimir(red_ini, n);
   hoshen(red,n);
	printf("\n \n");

	imprimir(red, n);
	
 /* for(i=0;i<z;i++)
    {
      prob=0.5;
      denominador=2.0;
 
      srand(time(NULL));

      for(j=0;j<P;j++) //no menos de 13 iteraciones (P>13)
        {
          llenar(red,n,prob);
      
          hoshen(red,n);
        
          denominador=2.0*denominador;

          if (percola(red,n)) 
             prob+=(-1.0/denominador); 
          else prob+=(1.0/denominador);
		//ir guardando en cada paso prob/N que percolan 
        }
    }*/

  free(red);
	return 0;
}

void hoshen(int *red, int n){
	int *clase;

	clase = (int *)malloc(30*n*sizeof(int));

	clase[0] = 0; clase[1] = 1;

	actualizar(red, clase, 2, n);
	free(clase);
}

void actualizar(int *red, int *clase, int frag, int n){  // actualiza cada nodo de la red según los nodos vecinos.
	int i, j, nuevo, s;
	nuevo = frag;

	for(j=0;j<n;j++){
				if(!red[0*n+j]) red[0*n+j]= red[0*n+j]; // CASO 1 no pasa nada
				else if(red[0*n+j]){  // CASO 2 actualiza etiqueta.
					red[0*n+j]=nuevo; 		
					clase[nuevo] = nuevo;  // agrego una etiqueta al vector auxiliar "clase[]"
					nuevo++;
				} 
				if(  red[0*n+j] && red[i*n+j-1]){  // 		      0
					s = red[i*n+j-1];												// CASO 3 ---> X  1
					red[i*n+j] = s;		//										Actualizao etiqueta
				}
	}

	for(i=1;i<n;i++){
				if(!red[i*n+0]) red[i*n+0]= red[i*n+0]; // CASO 1 no pasa nada
				else if(red[i*n+0]){  // CASO 2 actualiza etiqueta.
					red[i*n+0]=nuevo; 		
					clase[nuevo] = nuevo;  // agrego una etiqueta al vector auxiliar "clase[]"
					nuevo++;
				} 
				if(  red[i*n+0] && red[i*n+0-n]){  // 		      0
					s = red[i*n+0-n];												// CASO 3 ---> X  1
					red[i*n+0] = s;		//										Actualizao etiqueta
				}
	}

	for (i=1; i<n; i++){
		for (j=1; j<n; j++){
				
				if(!red[i*n+j]) red[i*n+j]= red[i*n+j]; // CASO 1 no pasa nada
				else if(red[i*n+j] && !red[i*n+j-1] && !red[i*n+j-n]){  // CASO 2 actualiza etiqueta.
					red[i*n+j]=nuevo; 		
					clase[nuevo] = nuevo;  // agrego una etiqueta al vector auxiliar "clase[]"
					nuevo++;
				} 
				else if(  red[i*n+j] && red[i*n+j-1] && !red[i*n+j-n] ){  // 		      0
					s = red[i*n+j-1];												// CASO 3 ---> X  1
					red[i*n+j] = s;		//										Actualizao etiqueta
				}
				else if(red[i*n+j] && red[i*n+j-n] && !red[i*n+j-1]){//					X
					s = red[i*n+j-n];					//   						CASO 3 --->	0  1 
					red[i*n+j] = s;					//							Actualizo etiqueta
				}
				else if( red[i*n+j] && red[i*n+j-n] && red[i*n+j-1] ) etiqueta_falsa(clase, red[i*n+j-n], red[i*n+j-1]); // CASO 4 Conflicto de etiquetas.	
		}
	}
	corregir_etiqueta(red, clase, n);
//	corregir_etiqueta(red, clase, n);	
}

void etiqueta_falsa(int *clase, int s1, int s2){  // pone etiquetas falsas en vector auxiliar "clase"

		if(s2 > s1) clase[s2] = -s1;
		else if (s2 < s1) clase[s1] = -s2;	
}

void corregir_etiqueta(int *red, int *clase, int n){  // corrije etiquetas en la red.
	int s0, s1, s2, i, j;
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
	}
}


int numeros(float p){
	int n;
	n = 0;
	float res = (float)rand()/(float)RAND_MAX;
	if(res<p){
		n=1;
	}
	return n;
}
 
void llenar(int *red, int n, float prob){
	int i;
	for(i=0; i<n*n; i++) {
			red[i] = numeros(prob);
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
