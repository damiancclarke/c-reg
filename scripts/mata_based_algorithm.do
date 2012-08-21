clear all
set obs 1000
gen y=rnormal()
gen x1=rnormal() + y*2
gen x2=rnormal()+ y*3
gen x3=rnormal()*3

mata:

Y = st_data(.,"y")
X = st_data(.,"x*")
X = (1::1000):/(1::1000), X

invsym((X'X))*(X'Y)


// X'X
N = rows(X)
P = cols(X)

// Diagonal
XX_DIAG = 0
for (j=1; j<=P; ++j)
{
	SUM = X[1,j]*X[1,j]
	for (i=2; i<=N; ++i) SUM = SUM + X[i,j]*X[i,j]
	XX_DIAG = XX_DIAG\ SUM
}

X[1,2]
XX_DIAG
diagonal((X'X))
end
/*
reg y x1 x2 x3
