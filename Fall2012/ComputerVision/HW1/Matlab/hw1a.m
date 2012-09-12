%%Computer Vision Fall 2012
%%Homework1a
%%Brian Fehrman
%%Takes a matrix A and finds it eigenvectors and values
%%It then reconstructs A from the eigenvectors and values
%%If we take A times v where v is an eigenvector then we get
%%a new vector whos direction is the same as that of v's but is
%%scaled. However, we don't need to compute the Matrix-Vector
%%multiplication. The same result can be obtained by multiplying
%%the eigvector v with its associated eigenvalue. This is shown by
%%the program.

%Which eigenvector and eigenvalue pair to test
%Make sure that is is within the correct range...
eigenvectorUse = 1; 

%Generate an arbitrary Matrix A
A = [ 5 8 3 2; 6 1 9 4; 3 9 4 1; 7 3 1 4]

%Get the eigenvectors and eigenvalues of A
%v are the eigenvectors and D contains the
%eigenvalues along its diagonal
[v, D] = eig(A)

%Reconstruct A using the eigenvectors and values
v*D*inv(v)

%Output A multiplied by the first eigenvector
A*v(:,eigenvectorUse)

%Show that the eigenvector multiplied by its associated
%value gives the same result as A times the eigenvector
newVector = v(:,eigenvectorUse) * D(eigenvectorUse,eigenvectorUse)

%Show that eigenvector and the new vector have the same
%direction
v(:,eigenvectorUse)
newVector / norm(newVector)