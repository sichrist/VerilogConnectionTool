module top
(

	a,
	b,
	c,
	d,
	e

	);


input a;
input b;
input c;
input d;
output e;
output f;



testmodule_a test_top_1(


	.in1( a );
	.in2( b );
	.out( e );
	);

testmodule_b test_top_2(


	.in1( c );
	.in2( d );
	.out( f );
	);


endmodule






