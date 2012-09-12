%%Computer Vision Fall 2012
%%Homework1b
%%Brian Fehrman
%%
%%Takes an arbitrary matrix and finds the dominant eigenvector and eigenvalue
%%by using the power iteration method

function [domEigenvector, domEigenvalue, numIterations] = powerIteration(A)
	
	%Set the ending tolerance
	tol = 1e-9; 
	%Initialize change to infinity
	change = inf;
	
	%Get the number of rows and columns of the matrix 
	[numRows, numColumns] = size(A);
	
	%Generate a random vector of the correct
	%dimension for the first guess
	v1 = rand(numColumns,1);
	
	numIterations = 0;
	
	%Compute new vectors until convergence is reached
	while (change > tol)
		numIterations = numIterations + 1;
		v2 = (A*v1) ./ norm(A*v1);
		change = norm(v2 - v1);
		v1 = v2;
	end
	
	%Dominant eigenvector is just the latest v1 computed
	domEigenvector = v1;
	
	%Dominant eigenvalue is the given matrix A times the latest v1
	domEigenvalue = norm(A*v1);

		
	