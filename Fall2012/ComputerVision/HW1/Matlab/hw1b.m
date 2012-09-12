%%Computer Vision Fall 2012
%%Homework1b
%%Brian Fehrman
%%
%%Takes an arbitrary matrix and finds the dominant eigenvector and eigenvalue
%%by using the power iteration method

function [domEigenvector, domEigenvalue] = powerIteration(A)
	
	%Set the ending tolerance
	tol = 0.001;
	change = inf;
	[numRows, numColumns] = size(A);
	v1 = rand(numColumns);
	
	while (change > tol)
		v2 = (A*v1) ./ norm(A*v1);
		change = norm(v2 - v1);
		v1 = v2;
	end
	
	domEigenvector = v1;
	domEigenvalue = norm(v1);

		
	