#include <stdio.h>
#include <math.h>
 
int main()
{
	//Setup required vars
	int i, rows, cols, rows_trans = 0;
	float inv, sum, obs_x, vars_x, obs_y = 0;
	float x[100][100], transpose[100][100], y[100];
	float xx[100][100], xy[100], beta[100];

	//Enter x variables
	printf("Enter the number of observations and variables of X\n");
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

		
		/*	Calculate the inverse of the matrix.  Under construction.
		*/
				



		

		//Calculate vectors of regression coefficients
		for (rows_trans = 0; rows_trans < obs_x; rows_trans++)
		{
			for (i = 0; i < vars_x; i++)
			{
				inv = transpose[i][rows_trans];
				if (inv == 0)
					inv = 0;
				else if (inv != 0)
					inv = 1/inv;
				sum = sum + inv*y[i];
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
	
	return 0;
}

