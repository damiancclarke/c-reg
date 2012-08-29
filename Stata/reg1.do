clear all
set obs 1000
set seed 1001
gen y=rnormal()
gen x0=1
gen x1=rnormal()+1
gen x2=rnormal()+2
gen x3=rnormal()+3


mata:
	y = st_data(., "y")
	X = st_data(., "x0 x1 x2 x3")
	invsym(X'X)*X'y

	cores = 96
	R = rows(X)
	obs_per_core=R/cores
	
		
*	for (j=1;  j<=cores; ++j) {
	
	
*	}
end



reg y x0 x1 x2 x3



mata:
y = 1::10
x1 = 11::20
x2 = 21::30
x3 = 31::40

X = x1, x2, x3

XX = X'X
Xy = X'y

invXX=invsym(XX)
invXX'Xy
end









local cluster=8

count if y
local obs_cluster=ceil(r(N)/`cluster')
di `obs_cluster'


foreach var1 of varlist x1 x2 x3 {
	foreach var2 of varlist x1 x2 x3 {
	
		
	}
}
