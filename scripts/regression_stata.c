//This program runs a multilinear regression with variables specified by the
//user.  This program should be treated as a work in progress.
//Comments or questions should be directed to Damian C. Clarke:
// damian.clarke@economics.ox.ax.uk or damian.c.clarke@gmail.com
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "stplugin.h"

typedef struct Beta {
	double beta;
	double se;
	struct Beta* next;
} coefficients;

//Store values of beta in a struct to send to the principal program
coefficients* create_coefficients(double coef, double stder) {
	coefficients* newcoef = malloc(sizeof(coefficients));
	if (NULL != newcoef) {
		newcoef->beta = coef;
		newcoef->se = stder;
		newcoef->next = NULL;
	}
	return newcoef;
}

void delete_coefficients(coefficients* oldcoefs) {
	if (NULL != oldcoefs) {
		delete_coefficients(oldcoefs->next);
	}
	free(oldcoefs);
}

coefficients* add_coefs(coefficients* coeflist, double coef, double stder) {
	coefficients* newcoef = create_coefficients(coef, stder);
	if (NULL != newcoef){
		newcoef->next = coeflist;
	}
	return newcoef;
}

//Function for making matrix (double array using malloc)
double** MakeMatrix(int obs, int vars) {  
double** theArray;  
Matrix = (double**) malloc(obs*sizeof(double*));  
for (int i = 0; i < obs; i++)  
   Matrix[i] = (double*) malloc(vars*sizeof(double));  
   return Matrix;  
}  


double detrm(double[100][100], double, double[100], int, double, double[100][100], double[100]);
coefficients* cofact(double[100][100], double, double[100], int, double, double[100][100], double[100]);
coefficients* trans(double[100][100], double[100][100], double, double[100], int, double, double[100][100], double[100]);
coefficients* reg(double, double[100], int, double[100][100], double, double[100][100], double[100]);
void stderror(double, double[100], double, double[100][100], double[100], double[100][100]);

STDLL stata_call(int argc, char *argv[])
{
	//Setup required vars
	int rows, cols, rows_trans = 0;
	double inv, sum = 0;
	double x[100][100], transpose[100][100], y[100];
	double xx[100][100], xy[100];

	//Enter x variables
//	printf("Enter the number of observations and variables of X\n");
//	scanf("%lf%lf", &obs_x, &vars_x);
//	printf("Enter the variables\n");
	double vars_x = SF_nvars() - 1;
	//printf("vars_x: %f\n", vars_x);
	double obs_x = SF_nobs();
	double obs_y = SF_nobs();	
	ST_double z;
	int i = 0 ;

	printf("Vars_x: %lf\n", vars_x);
	printf("obs_x: %lf\n", obs_y);
	printf("obs_y: %lf\n", obs_y);

	
	for (cols = 1; cols < SF_nvars(); cols++)
	{	
		for (rows = 1; rows <= SF_nobs(); rows++)
		{
			SF_vdata(cols, rows, &z);
			x[cols-1][rows-1] = z;
			//SF_vstore(SF_nvars(), rows, x[cols][rows]);
			SF_mat_store(argv[0], rows, cols, x[cols-1][rows-1]);
		}
	}


	//Enter y variable 
	//Check whether x and y have same number of obs
	if ( obs_x != obs_y )
		printf("Matrices with entered orders can't be multiplied with each other.\n");
	else
	{
		//printf("Enter the variable\n");

		for (rows = 1; rows <= SF_nobs(); rows++)
		{
			SF_vdata(SF_nvars(), rows, &z);
			y[rows-1] = z;
			SF_mat_store(argv[1], rows, 1, y[rows-1]);
		}

		//First transpose the matrix X
		for(cols = 0; cols < obs_x; cols++)
		{
	    	for(rows = 0; rows < obs_x; rows++)
      		{
				transpose[cols][rows] = x[rows][cols];
			}
		} 

		//Now create the matrix X'X 
		for(rows_trans = 0; rows_trans < vars_x; rows_trans++)
		{
			for(cols = 0; cols < vars_x; cols++)
			{
				for (i = 0; i < obs_x; i++)
				{
					sum = sum + transpose[i][rows_trans]*x[cols][i];
				}
	
				xx[rows_trans][cols] = sum;
				sum = 0;
			}
		}


		//Create matrix X'y
		for (rows_trans = 0; rows_trans < obs_x; rows_trans++)
			{
				for (i = 0; i < obs_y; i++)
				{
					sum = sum + transpose[i][rows_trans]*y[i];
				} 
			xy[rows_trans] = sum;
			sum = 0;
		}
		
		// Calculate the inverse of the matrix.
		// Calculate vectors of regression coefficients
		// Finally print out coefficients of the regression
		double d;
		coefficients* aa;
		d = detrm(xx, vars_x,  xy, rows_trans, obs_x, x, y);
 
		if (d == 0)
			printf( "\nMatrix is not invertible\n" );
		else
			{
			aa = cofact(xx , vars_x, xy, rows_trans, obs_x, x, y);
			coefficients* list;
			int num = 1;
			for (list = aa; NULL != list; list = list->next) {
				printf("xbeta: %lf \n", list->beta);
				SF_mat_store(argv[2], num, 1, list->beta);
				printf("xse: %lf \n", list->se);
				SF_mat_store(argv[2], num, 2, list->se);
				num++;
			}
			delete_coefficients(aa);
		}				
	}
	return 0;
}




/*******************************************************************************
Function to find the determinant of the matrix
*******************************************************************************/
 
double detrm(double xx[100][100], double vars_x, double xy[100], int rows_trans, double obs_x, double x[100][100], double y[100])
{
	double s = 1, det = 0, b[100][100];
	int k, j, m, n, c;
	double x1 = vars_x - 1;
	double x2 = vars_x - 2;

	if (vars_x == 1)
		{
			return (xx[0][0]);
		}
	else
		{
			det = 0;

			for (c = 0; c < vars_x; c++)
				{
					m = 0;
					n = 0;

					for (k = 0; k < vars_x; k++)
						{
							for (j = 0; j < vars_x ; j++)
								{
									b[k][j] = 0;

									if (k != 0 && j != c)
										{
											b[m][n] = xx[k][j];
 
											if (n < x2)
												n++;
											else
												{
													n = 0;
													m++;
												}
										}
								}
						}

						det = det + s *( xx[0][c] * detrm(b, x1, xy, rows_trans, obs_x, x, y));
						s = -1 * s;
				}
		}
 
	return(det);
}
 
/*******************************************************************************
Function to find the cofactor of the matrix
*******************************************************************************/
coefficients* cofact(double num[100][100], double f, double xy[100], int rows_trans, double obs_x, double x[100][100], double y[100])
{
	double b[100][100], fac[100][100];
	int p, q, m, n, k, j;
	double f1 = f - 1;
	double f2 = f - 2;

	for (q = 0; q < f; q++)
		{
			for (p = 0; p < f; p++)
				{
					m = 0;
					n = 0;

					for (k = 0; k < f; k++)
						{
							for (j = 0; j < f; j++)
								{
									b[k][j] = 0;
 
									if (k != q && j != p)
										{
											b[m][n] = num[k][j];
 
											if (n < f2)
												n++;
											else
												{
													n = 0;
													m++;
												}
										}
								}
						}

					fac[q][p] = pow(-1, q + p) * detrm(b, f1, xy, rows_trans, obs_x, x, y);
			}
		}
	coefficients* aa;
	aa = trans(num, fac, f, xy, rows_trans, obs_x, x, y);
	return(aa);	

}
 
/*******************************************************************************
Function to find the transpose and inverse of the matrix
*******************************************************************************/
coefficients* trans(double num[100][100], double fac[100][100], double r, double xy[100], int rows_trans, double obs_x, double x[100][100], double y[100])

{
	int k, j;
	double b[100][100], d, inv[100][100];

	for (k = 0; k < r; k++)
		{
			for (j = 0; j < r; j++)
				{
					b[k][j] = fac[j][k];
				}
		}

	d = detrm(num, r, xy, rows_trans, obs_x, x, y);
	inv[k][j] = 0;

	for (k = 0; k < r; k++)
		{
			for (j = 0; j < r; j++)
				{
					inv[k][j] = b[k][j] / d;
				}
		}
		coefficients* aa;
		aa = reg(r, xy, rows_trans, inv, obs_x, x, y);

		return(aa);		

}


/*******************************************************************************
Function to calculate regression coefficient and standard errors
*******************************************************************************/

coefficients* reg(double r, double xy[100], int rows_trans, double inv[100][100], double obs_x, double x[100][100], double y[100])
{
	
	int k;
	double sum = 0;	
	double beta[100];
	
	//Calculate vectors of regression coefficients
	for (rows_trans = 0; rows_trans < r; rows_trans++)
	{
		for (k = 0; k < r; k++)
		{
			sum = sum + inv[rows_trans][k]*xy[k];
		} 
	beta[rows_trans] = sum;
	sum = 0;
	}

	//Store regression coefficients as Structs



	//STANDARD ERRORS
	int i, j = 0;
	double xbi, ubar, sigma = 0;
	double xb[100], ui[100], se[100];

	//(1) Calculate x_i'B and ui
	for(i = 0; i < obs_x; i++)
	{
		xbi = 0;
		for (j = 0; j < r; j++)
		{	
			xbi = xbi + beta[j]*x[j][i];
		}
		xb[i] = xbi;
		ui[i] = y[i] - xb[i];
	}

	for(i = 0; i < obs_x; i++)
	{
		sigma = sigma + pow(ui[i], 2);
	}
	sigma = sigma / (obs_x-r);

	//(4) Calculate std. errors
	for (j = 0; j < r; j++)
	{	
		se[j] = sigma * inv[j][j];
		se[j] = pow(se[j], 0.5);
	}		


	coefficients* regres_results = create_coefficients(beta[0], se[0]);
	for(k = 1; k < r; k++)
	{
		regres_results = add_coefs(regres_results, beta[k], se[k]);
	}

	coefficients* list;
	for (list = regres_results; NULL != list; list = list->next) {
		printf("beta: %lf \n", list->beta);
		printf("se: %lf \n", list->se);
	}

//	delete_coefficients(regres_results);
	return(regres_results);
}
