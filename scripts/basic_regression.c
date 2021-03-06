#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	/*if (argc <= 2) {
		printf("ERROR: You need at least one independent variable.\n");
		return 1;
	}*/

	int y[] = {23, 21, 18, 15, 6, 12, 7, 3, 0};
	int x[][] = {
					430, 350, 330, 220, 120, 100, 90, 80, 10,
				 	1, 0, 1, 1, 0, 1, 0, 0, 0
				};
	double xx = 0; 
	double xy = 0;
	double sum = 0;
	double beta = 0;
	char *vars[] ={"x1", "x2"};

	//get the size of the x and y vector
	int count_y = sizeof(y) / sizeof(int);
	int count_x = sizeof(x[1]) / sizeof(int);
	int count_vars = sizeof(vars) / sizeof(int);

	//create the vectors x'x and x'y
	int i = 0;
//	int j = 0;
//	for(j = 0; j < count_vars; j++) {
		for(i = 0; i < count_x; i++) {
			sum = sum + x[1][i]*x[1][i];
		}
		xx = sum;	
		printf("The product of this vector is %f:\n ", xx);

	sum = 0;

	for(i = 0; i < count_y; i++) {
		sum = sum + x[1][i]*y[i];
	}
	xy = sum;	
	printf("The product of this vector is %f:\n ", xy);

	//invert x'x and multiply with xy to calculate the beta in the regression
	beta = 1/xx;
	beta = beta*xy;
	printf("beta is %e:\n ", beta);

	printf("counts: %d, %d, %d\n", count_y, count_x, count_vars);
	return 0;
}
