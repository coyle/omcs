#CSE 6220 Collectives -- MPI

In this lab, you will implement several collectives discussed in the lessons and analyze their performance.  This will give you an opportunity to solidify your understanding of the relevant algorithms and see how latency and bandwidth considerations impact overall performance.  You will also be familiarizing yourself with the MPI (Message Passing Interface) standard, which you will use in subsequent assignments. Recall that MPI provides a portable interface for parallel tasks running on different nodes of a cluster or supercomputer to send messages to each other.

##Programming

Your first task is to implement the following collectives

*  Reduce  
*  Scatter 
*  Gather  
*  Allgather
*  Broadcast : Tree-Based Implementation
*  Broadcast : Scatter-Allgather Implementation

Begin by obtaining the starter code from the github repository.

<pre><code>
	git clone .....
</code></pre>

should do the trick.  This will create clone of the repository on your local machine.  You should then modify the files and submit them to Udacity as described below.

Optionally, you may choose use a git hosting service for your code.  As always, please **do not make your repository public** on Github or any other git hosting service.  Anyone will be able to see it.  If you feel the need to use such a hosting service for your project, please keep your repository private.  Your GT account allows you to create [free private repositories](https:github.gatech.edu).

In the given files, you are to implement an interface with function of the form GT_\*, which
mirrors the MPI library.  Thus, GT_Bcast should do the same thing as MPI_Bcast.  In your implementation, 
**you may only use point-to-point MPI operations.**  These are  `MPI_Send`, `MPI_Recv`, `MPI_Isend`, and `MPI_Irecv`.  Simply calling `MPI_Bcast` within your `GT_Bcast` implementation, for instance, is not allowed.

Several good sources of documentation for MPI are available online.  There are several different versions and implementations.  These differences, however, will not be significant for our purposes.

*  [OpenMPI](http://www.open-mpi.org/doc/)
*  [MPI Forum](http://www.mpi-forum.org/docs/)
*  [LLNL](https://computing.llnl.gov/tutorials/mpi/)
*  [MPI Tutorial](http://mpitutorial.com/)

MPI is a very general tool, and to fully implement all of its features is beyond the scope of this lab.  Therefore, you are permitted to make simplifying assumptions such as 

*  the input buffers are valid memory addresses, not MPI_IN_PLACE for example.
*  the root is zero
*  the datatype is MPI_INT

In most cases, these assumptions are documented through the `assert` statements in the starter code.

## Submitting Your Code
When you have finished and tested your implementations, please submit them to the [Udacity site](https://www.udacity.com/course/viewer#!/c-ud281/l-4583467010/m-4623058914), which will make a quick test for correctness.  After the deadline, the TAs will pull the code and perform some timing runs to confirm that your implementation is efficient.  It is recommended that you used the strategies discussed in the lecture videos, tree-based algorithms and bucketing in particular, though you are encouraged to explore other ideas as well.

# Analysis

## Parameter Fitting
In our basic model of distributed memory, we say that the time for some communication is given by $$T = \alpha + \beta n,$$ where the constant term \\(\alpha\\) represents the **latency** and the linear coefficient \\(\beta\\) represents the **inverse bandwidth** (see the lesson for details).  Given some data on communication times, your task is to fit \\(\alpha\\) and \\(\beta\\) parameters.

The file comm-time.tsv contains some measurements on the time taken to perform some point-to-point communications on the Peguin-on-Demand cluster.  Your task is to find values for the parameters \\(\alpha\\) and \\(\beta\\), so that the results curve passes close to the data on a logscale plot like the one shown below.

![Points and Curve on Logscale]()

If you have access to gnuplot, you may modify the alpha and beta parameters at the file fit.gn to help you visualize the accuracy of your fit.  Note that a least-squares solution is not appropriate here.

Once you have a good fit, submit your \\(\alpha\\) and \\(\beta\\) values to the [Udacity site]().

## Identify the Algorithm

The files bcast-alg1.tsv and bcast-alg2.tsv show timing results from two separate implementations of GT_Bcast.  One uses a tree-based algorithm, the other the scatter/allgather approach.  From the plot below, can you tell which is which?

![](s3link)

Write up your explanation and submit it to the [Udacity site]().








