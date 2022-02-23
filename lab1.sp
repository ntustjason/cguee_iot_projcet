lab 1 HWare

.option post 

r1 vi vo 1k
r2 vo 0 1k
c1 vo 0 10u

** DC
vi vi 0 1
.tran 1p 100p


** AC
*vi vi 0 sin 1 2 10
*.tran 1p 1



.end