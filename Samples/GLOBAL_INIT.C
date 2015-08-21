/*********************************************************

     Samples\global_init.c
     Z-World, 2000

     This sample demonstrates the use of #GLOBAL_INIT
     sections to run initialization code such as
     initializing static variables.
     
     Because static variables are placed in flash if
     they are initialized upon declaration, e.g. int x=0;,
     #GLOBAL_INIT sections are the correct method in
     Dynamic C to pre-initialize static variables whose
     values will be modified later in the program.

**********************************************************/
#class auto

void foo();
int y;

void main(){
	// static is the default storage type, but if the default changes
	//  we still need this to be static, hence the explicit static
	static int x;  

	// #GLOBAL_INIT sections must appear after variable
	//  declarations and before executable code
	#GLOBAL_INIT
	{
		x = 1234;
	}

	// note that the execution cursor stops here after 
	//  compilation with F5
	printf("x = %d\n\n",x);
	printf("y = %d\n\n",y);
	x = 0;
	foo();
	printf("x = %d\n\n",x);
	printf("y = %d\n\n",y);	
}


void foo()
{
	// #GLOBAL_INIT sections must appear after variable
	//  declarations and before executable code
	#GLOBAL_INIT
	{
		y = 4321;
	}
	y = 0;
}