#include <stdio.h>
#include <math.h>
float detrm(float[100][100], float);
void cofact(float[100][100], float);
void trans(float[100][100], float[100][100], float);
void reg(float[100][100], float[100][100], float, int);
//void inv(float[100][100]);

int main()
{
	//Setup required vars
	int i, rows, cols, rows_trans = 0;
	float inv, sum, obs_x, vars_x, obs_y = 0;
	float x[100][100], transpose[100][100], y[100];
	float xx[100][100], xy[100];

	//Enter x variables
	printf("Enter the number of observations and variables of XXXX\n");
	scanf("%f%f", &obs_x, &vars_x);
	printf("Enter the variables\n");

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

		
		//	Calculate the inverse of the matrix.  Under construction.
		float d;
		d = detrm(xx, vars_x);
		printf( "The determinant is = %f", d );
 
		if (d == 0)
			printf( "\nMatrix is not invertible\n" );
		else
			cofact(xx , vars_x);				


		//Calculate vectors of regression coefficients
		reg(inv, xy, obs_x, rows_trans);
	}
	
	return 0;
}




/*******************************************************************************
Function to find the determinant of the matrix
*******************************************************************************/
 
float detrm(float xx[100][100], float vars_x)
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

						det = det + s *( xx[0][c] * detrm(b, x1));
						s = -1 * s;
				}
		}
 
	return(det);
}
 
/*******************************************************************************
Function to find the cofactor of the matrix
*******************************************************************************/
void cofact(float num[100][100], float f)
{
	float b[100][100], fac[100][100];
	int p, q, m, n, k, j;
	float f1 = f - 1;
	float f2 = f - 2;

	for (q = 0; q < f; q++ )
		{
			for ( p = 0;p < f;p++ )
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

					fac[q][p] = pow(-1, q + p) * detrm(b, f1);
			}
		}

	trans( num, fac, f );
}
 
/*******************************************************************************
Function to find the transpose and inverse of the matrix
*******************************************************************************/
 
void trans(float num[100][100], float fac[100][100], float r)

{
	int k, j;
	float b[100][100], inv[100][100], d;

	for (k = 0; k < r; k++)
		{
			for (j = 0; j < r; j++)
				{
					b[k][j] = fac[j][k];
				}
		}

	d = detrm(num, r);
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
}


/*******************************************************************************
Function to find the regression coefficients
*******************************************************************************/
void reg(float inv[100][100], float xy[100][100], float obs_x, int rows_trans)

{
	float beta[100];
	int i = 0;


	for (rows_trans = 0; rows_trans < obs_x; rows_trans++)
	{
		for (i = 0; i < vars_x; i++)
		{
			sum = sum + inv[rows_trans][i]*y[i];
		} 
	beta[rows_trans] = sum;
	sum = 0;
	}

	//Finally print out coefficients of the regression
	printf("Regression Coefficients:\n");
	for (rows_trans = 0; rows_trans < vars_x; rows_trans++)
	{
		printf("%f ", beta[rows_trans]);
		printf("\n");
	}
}
