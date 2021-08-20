# OpenMP
> Overview...

- Open multi-processing

	- OpenMP is an API for shared memory parallel applications


- Shared memory model can be:

	- UMA: uniform memory access

	- NUMA: non-uniform memory access

- OpenMP programs accomplish parallelism exclusively through the use of threads

	- A thread of execution is the smallest unit of processing that
	 can be scheduled by an operating system. The idea of a subroutine
	that can be scheduled to run autonomously might help explain what
	a thread is.

	- Typically, the number of threads match the number of machine
	processors/cores. However, the actual use of threads is up to the
	application.

- OpenMP is an explicit (not automatic) programming model, offering
the programmer full control over parallelization.

- Parallelization can be as simple as taking a serial program and
inserting compiler directives...

	- Or as complex as inserting subroutines to set multiple levels of
	parallelism, locks and even nested locks.


- OpenMP compiler directives are used for various purposes: 

	- Spawning a parallel region

	- Dividing blocks of code among threads

	- Distributing loop iterations between threads Serializing sections
	of code

	- Synchronization of work among threads


- Compiler directives have the following syntax:

  ```sentinel       directive-name      [clause, ...]```

- OpenMP provides several environment variables for controlling the
execution of parallel code at run-time.

	- These environment variables can be used to control such things
	as: 
	
		- Setting the number of threads

		- Specifying how loop interations are divided

		- Binding threads to processors
		
		- Enabling/disabling nested parallelism; setting the maximum
		levels of nested parallelism Enabling/disabling dynamic threads

		- Setting thread stack size

		- Setting thread wait policy

	- Setting OpenMP environment variables is done the same way you
		set any other environment variables, and depends upon which
		shell you use. For example:
		
		```$ export OMP_NUM_THREADS=8```

## C / C++ Directives Format


| #pragma omp | directive-name | [clause, ...] | newline |
|:-----:|:-----:|:-----:|:-----:|
| Required for all OpenMP C/C++ directives | A valid OpenMP directive.<br>Must appear after the pragma and before any clauses. | Optional. Clauses can be in any order,<br>and repeated as necessary unless <br>otherwise restricted. | Required. Precedes the structured block <br>which is enclosed by this directive. |

- Example:

	```#pragma omp parallel default(shared) private(beta,pi)```

- General rules:
	- Case sensitive

	- Directives follow conventions of the C/C++ standards for
	compiler directives

	- Only one directive-name may be specified per directive

	- Each directive applies to at most one succeeding statement,
	which must be a structured block.

	- Long directive lines can be "continued" on succeeding lines by
	escaping the newline character with a backslash ("\\") at the end
	of a directive line.




<hr>

## References

- https://www.brunoribas.com.br/pspd/2021-1/OpenMP.pdf

- https://sol.sbc.org.br/livros/index.php/sbc/catalog/download/30/115/279-1?inline=1

- http://www.inf.puc-rio.br/~noemi/pcp-16/aula4/openmp.pdf

- https://sol.sbc.org.br/livros/index.php/sbc/catalog/download/44/190/407-1?inline=1

- https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf