#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

int main( int argc, char *argv[] )
{
	// Define
	size_t i, j, k, l;
	unsigned int count;
	gsl_matrix *A;
	gsl_vector *v1;
	gsl_vector *v2;
	gsl_vector *v3;
	double lambda1;
	double lambda2;

	// Seed rand( )
	srand( time( NULL ) );

	// Set Dimensions
	k = 3;
	l = 3;

	// Allocate
	A = gsl_matrix_alloc( k, l );
	v1 = gsl_vector_alloc( l );
	v2 = gsl_vector_alloc( l );
	v3 = gsl_vector_alloc( l );

	// Populate
	for( i = 0; i < k; i++ )
		for( j = 0; j < l; j++ )
			gsl_matrix_set( A, i, j, rand( ) % 30 );
	for( i = 0; i < l; i++ )
		gsl_vector_set( v1, i, rand( ) % 30 );

	printf( "Here is A:\n" );
	for( i = 0; i < k; i++ )
	{
		for( j = 0; j < l; j++ )
			printf( "%2.0f ", gsl_matrix_get( A, i, j  ) );
		printf( "\n" );
	}

	printf( "\nHere comes the loop:\n" );


	// lambda1 = ||v1||
	lambda1 = gsl_blas_dnrm2( v1 );

	// v1 = v1 / lambda1 )
	if( gsl_vector_scale( v1, 1.0 / lambda1 ) )
		fprintf( stderr, "ERROR" );

	// Go
	lambda2 = 1;
	for( count = 0; lambda2 >= 0.000000000000001; count++ )
	{
		// v2 = A * v1
		if( gsl_blas_dgemv(CblasNoTrans, 1.0, A, v1, 0.0, v2 ) )
			fprintf( stderr, "ERROR" );

		// lambda1 = ||v2||
		lambda1 = gsl_blas_dnrm2( v2 );

		// v2 = v2 / lambda1 )
		//if( gsl_vector_memcpy( v1, v2 ) )
		//	fprintf( stderr, "ERROR" );
		if( gsl_vector_scale( v2, 1.0 / lambda1 ) )
			fprintf( stderr, "ERROR" );

		if( gsl_vector_memcpy( v3, v2 ) )
			fprintf( stderr, "ERROR" );

		if( gsl_vector_sub( v3, v1 ) )
			fprintf( stderr, "ERROR" );

		if( gsl_vector_memcpy( v1, v2 ) )
			fprintf( stderr, "ERROR" );

		lambda2 = gsl_blas_dnrm2( v3 );

		// Output
		printf( "%.15f", lambda2 );
		printf( " %.15f\n", lambda1 );
	}

	printf( "------------------------------------\n" );
	printf( "Iterations: %d\n", count );
	printf( "Eigenvalue: %.15f\n", lambda1 );
	printf( "Eigenvector:\n" );
	gsl_vector_fprintf( stdout, v1, "%.15f" );

	// Free
	gsl_matrix_free( A );
	gsl_vector_free( v1 );
	gsl_vector_free( v2 );
	gsl_vector_free( v3 );

	// Done
	exit( EXIT_SUCCESS );
}
