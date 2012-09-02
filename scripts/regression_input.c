//This program runs a multilinear regression with variables specified by the
//user.  This program should be treated as a work in progress.
//Comments or questions should be directed to Damian C. Clarke:
// damian.clarke@economics.ox.ax.uk or damian.c.clarke@gmail.com
#include <stdio.h>
#include <math.h>
float detrm(float[100][100], float, float[100], int, float, float[100][100], float[100]);
void cofact(float[100][100], float, float[100], int, float, float[100][100], float[100]);
void trans(float[100][100], float[100][100], float, float[100], int, float, float[100][100], float[100]);
float reg(float, float[100], int, float[100][100], float, float[100][100], float[100]);
void stderror(float, float[100], float, float[100][100], float[100], float[100][100]);

int main(int argc, char *argv[])
{
	//Setup required vars
	int i, rows, cols, rows_trans = 0;
	float inv, sum, obs_x, vars_x, obs_y = 0;
	float x[100][100], transpose[100][100], y[100];
	float xx[100][100], xy[100];

	//Enter x variables
//	printf("Enter the number of observations and variables of X\n");
//	scanf("%f%f", &obs_x, &vars_x);
//	printf("Enter the variables\n");
	float vars_x = argv[1]
	
	for (cols = 0 ; cols < vars_x ; cols++)
		for (rows = 0 ; rows < obs_x ; rows++)
			scanf("%f", &x[cols][rows]);

	printf("print X:\n");
	for (rows = 0; rows < obs_x; rows++)
		{
		for (cols = 0; cols < vars_x; cols++)
			{
				printf("%f ", x[cols][rows]);
	 		}
			printf("\n");
		}


	//Enter y variable 
	printf("Enter the number of observations of y\n");
	scanf("%f", &obs_y);

	//Check whether x and y have same number of obs
	if ( obs_x != obs_y )
		printf("Matrices with entered orders can't be multiplied with each other.\n");
	else
	{
		printf("Enter the variable\n");

		for ( rows = 0 ; rows < obs_y ; rows++ )
			scanf("%f", &y[rows]);


		//First transpose the matrix X
		for(cols = 0; cols < obs_x; cols++)
		{
	    	for(rows = 0; rows < obs_x; rows++)
      		{
				transpose[cols][rows] = x[rows][cols];
			}
		} 

		//Print transposed matrix X (check)
		printf("print transpose:\n");
		for (rows_trans = 0; rows_trans < vars_x; rows_trans++)
		{
			for (cols = 0; cols < obs_x; cols++)
			{
				printf("%f ", transpose[cols][rows_trans]);
	 		}
			printf("\n ");
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

		//Test printing out X'X and X'y (check)
		printf("Product of X'X:\n");
		for (rows_trans = 0; rows_trans < vars_x; rows_trans++)
		{
			for (cols = 0; cols < vars_x; cols++)
			{
				printf("%f ", xx[rows_trans][cols]);
	 		}
			printf("\n");
		}

		printf("Product of X'y:\n");
		for (rows_trans = 0; rows_trans < vars_x; rows_trans++)
		{
			printf("%f ", xy[rows_trans]);
			printf("\n");
		}

		
		// Calculate the inverse of the matrix.
		// Calculate vectors of regression coefficients
		// Finally print out coefficients of the regression
		float d;
		d = detrm(xx, vars_x,  xy, rows_trans, obs_x, x, y);
		printf( "The determinant is = %f", d );
 
		if (d == 0)
			printf( "\nMatrix is not invertible\n" );
		else
			cofact(xx , vars_x, xy, rows_trans, obs_x, x, y);				

	}

	return 0;
}




/*******************************************************************************
Function to find the determinant of the matrix
*******************************************************************************/
 
float detrm(float xx[100][100], float vars_x, float xy[100], int rows_trans, float obs_x, float x[100][100], float y[100])
{
	float s = 1, det = 0, b[100][100];
	int k, j, m, n, c;
	float x1 = vars_x - 1;
	float x2 = vars_x - 2;

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
void cofact(float num[100][100], float f, float xy[100], int rows_trans, float obs_x, float x[100][100], float y[100])
{
	float b[100][100], fac[100][100];
	int p, q, m, n, k, j;
	float f1 = f - 1;
	float f2 = f - 2;

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

	trans(num, fac, f, xy, rows_trans, obs_x, x, y);
}
 
/*******************************************************************************
Function to find the transpose and inverse of the matrix
*******************************************************************************/
void trans(float num[100][100], float fac[100][100], float r, float xy[100], int rows_trans, float obs_x, float x[100][100], float y[100])

{
	int k, j;
	float b[100][100], d, inv[100][100];

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

	printf( "\nThe inverse of the matrix is:\n");

	for (k = 0; k < r; k++)
		{
			for (j = 0; j < r; j++)
				{
					printf("\t%f", inv[k][j]);
				}

			printf( "\n" );
		}

		reg(r, xy, rows_trans, inv, obs_x, x, y);

}


/*******************************************************************************
Function to multiply inv(X'X) with X'y
*******************************************************************************/
float reg(float r, float xy[100], int rows_trans, float inv[100][100], float obs_x, float x[100][100], float y[100])
{
	
	int k;
	float sum = 0;	
	float beta[100];
	
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

	//Finally print out coefficients of the regression
	printf("Regression Coefficients:\n");
	for (rows_trans = 0; rows_trans < r; rows_trans++)
	{
		printf("%f ", beta[rows_trans]);
		printf("\n");
	}
	stderror(r, beta, obs_x, x, y, inv);
	return beta[rows_trans];
}


/*******************************************************************************
Function to calculate sigma squared (for standard errors).  Very parallelizable.
*******************************************************************************/
//u=y-XB // float xy[100]?, float inv[100][100],
void stderror(float r, float beta[100], float obs_x, float x[100][100], float y[100], float inv[100][100])
{
	int i, j = 0;
	float xbi, ubar, sigma = 0;
	float xb[100], ui[100], se[100];

	//(1) Calculate x_i'B and ui
	for(i = 0; i < obs_x; i++)
	{
		xbi = 0;
		for (j = 0; j < r; j++)
		{	
			xbi = xbi + beta[j]*x[j][i];
		}
		xb[i] = xbi;
		printf("X'B: %f \n", xb[i]);
		ui[i] = y[i] - xb[i];
		printf("ui: %f \n", ui[i]);
	}


	//(2) Calculate u_bar
	for(i = 0; i < obs_x; i++)
	{
		ubar = ubar + ui[i];
	}
	ubar = ubar / obs_x;
	printf("ubar: %f \n", ubar);

	//(3) Calculate var(u)
	for(i = 0; i < obs_x; i++)
	{
		sigma = sigma + pow((ui[i]-ubar), 2);
	}
	sigma = sigma / obs_x;

	//(4) Calculate std. errors
	for (j = 0; j < r; j++)
	{	
		se[j] = sigma * inv[j][j];
	}		
	printf("sigma: %f \n", sigma);

	//(5) Print std. errors
	printf("Standard Errors:\n");
	for (j = 0; j < r; j++)
	{
		printf("%f ", se[j]);
		printf("\n");
	}
}

