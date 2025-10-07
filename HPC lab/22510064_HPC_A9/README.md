TECHNICAL REPORT
PARALLEL PROCESSING USING EXPLORATORY AND SPECULATIVE EXECUTION TECHNIQUES

ABSTRACT

This report presents an implementation and performance analysis of two parallel processing techniques: exploratory parallelism using the N-Queens problem and speculative execution using conditional branch prediction. The study demonstrates performance improvements with speedups of 3.25× for N-Queens and 2.11× for speculative execution using OpenMP parallel programming.

1. INTRODUCTION

This report presents an implementation and performance analysis of two parallel processing techniques: exploratory parallelism using the N-Queens problem and speculative execution using conditional branch prediction. Both techniques represent different approaches to achieving parallelization in computational problems.

Exploratory parallelism involves dividing a search space among multiple threads to explore different solution paths simultaneously. The N-Queens problem serves as an ideal candidate for this technique as it requires exploring multiple board configurations to find valid solutions.

Speculative execution, on the other hand, involves executing multiple code paths in parallel before the actual branch condition is determined. This technique is particularly useful in scenarios where branch prediction can improve performance despite potential computational waste.

2. PROBLEM DESCRIPTIONS

2.1 N-Queens Problem (Exploratory Parallelism)

The N-Queens problem requires placing N chess queens on an N×N chessboard such that no two queens attack each other. Queens can attack horizontally, vertically, and diagonally. For N=12, there are exactly 14,200 distinct solutions.

The problem exhibits natural parallelism as different initial queen placements can be explored independently by separate threads. Each thread explores a portion of the solution space, making it an ideal candidate for exploratory parallelism.

2.2 Speculative Execution Problem

The speculative execution problem involves conditional statements where the outcome of a condition cannot be predicted with certainty. The implementation uses an if-else construct where both branches are executed speculatively in parallel, with one result being discarded based on the actual condition outcome.

This technique trades computational resources for reduced execution time, accepting wasted computation in exchange for potential performance gains through parallel execution.

3. ALGORITHM DESIGN

3.1 N-Queens Algorithm Design

Algorithm: Parallel N-Queens
Input: Board size N, Number of threads T
Output: Total number of solutions

Step 1: Initialize shared solution counter
Step 2: Divide first row positions among T threads
Step 3: For each thread:
        a. Get assigned starting positions
        b. For each starting position:
           - Place queen in assigned column
           - Recursively solve remaining positions
           - Use backtracking for invalid configurations
        c. Add local count to shared counter
Step 4: Return total solution count

3.2 Speculative Execution Algorithm Design

Algorithm: Speculative If-Else Execution
Input: Array of size N, Number of threads T
Output: Processed array

Step 1: Divide array into T segments
Step 2: For each segment in parallel:
        a. Create two threads for each element:
           - Thread A: Execute if-branch operations
           - Thread B: Execute else-branch operations
        b. Evaluate actual condition
        c. Select appropriate result, discard other
        d. Store result in output array
Step 3: Combine results from all segments

4. IMPLEMENTATION DETAILS

4.1 N-Queens Implementation

The N-Queens implementation uses OpenMP for parallelization with the following key components:

Code Snippet:
#pragma omp parallel for reduction(+:solutions)
for (int i = 0; i < n; i++) {
    // Each thread handles different starting positions
    solutions += solve_nqueens_recursive(board, 0, i, n);
}

Key implementation features:
- Recursive backtracking algorithm
- Thread-safe solution counting using reduction
- Dynamic work distribution among threads
- Conflict detection for queen placement validation

4.2 Speculative Execution Implementation

The speculative execution implementation creates parallel threads for both conditional branches:

Code Snippet:
#pragma omp parallel sections
{
    #pragma omp section
    {
        // Execute if-branch speculatively
        if_result = compute_if_branch(data);
    }
    #pragma omp section
    {
        // Execute else-branch speculatively
        else_result = compute_else_branch(data);
    }
}
// Select result based on actual condition
final_result = (condition) ? if_result : else_result;

Key implementation features:
- Parallel section execution for both branches
- Condition evaluation after speculative execution
- Result selection mechanism
- Waste computation tracking

5. RESULTS

5.1 Experimental Setup

Testing was conducted with the following configuration:
- N-Queens problem size: N=12 (14,200 solutions)
- Speculative execution array size: 10,000,000 elements
- Thread counts tested: 1, 2, 4, 8

5.2 Raw Experimental Data

N-Queens Results (from nqueens_results.csv):
nqueens,N=12,threads=1,solutions=14200,time=0.143000
nqueens,N=12,threads=2,solutions=14200,time=0.068000
nqueens,N=12,threads=4,solutions=14200,time=0.044000
nqueens,N=12,threads=8,solutions=14200,time=0.050000

Speculative Execution Results (from spec_results.csv):
speculation,SIZE=10000000,threads=1,t_seq=0.262000,t_spec=0.583000,spec_ops=20000000,wasted_ops=10000000,wasted_pct=50.000000
speculation,SIZE=10000000,threads=2,t_seq=0.274000,t_spec=0.287000,spec_ops=20000000,wasted_ops=10000000,wasted_pct=50.000000
speculation,SIZE=10000000,threads=4,t_seq=0.302000,t_spec=0.176000,spec_ops=20000000,wasted_ops=10000000,wasted_pct=50.000000
speculation,SIZE=10000000,threads=8,t_seq=0.281000,t_spec=0.133000,spec_ops=20000000,wasted_ops=10000000,wasted_pct=50.000000

5.3 Performance Analysis

Table 1: Summary Results
Problem                        Sequential Time (ms)  Parallel Time (ms)  Speedup  Wasted Computation (%)
N-Queens (Exploratory)                143                    44          3.25×                    ~0%
Branch Execution (Speculative)       262                   133          1.97×                   ~50%

Table 2: Detailed N-Queens Performance
Threads  Sequential Time (ms)  Parallel Time (ms)  Speedup  Efficiency
1                     143                    -        -           -
2                     143                   68     2.10×      105.0%
4                     143                   44     3.25×       81.3%
8                     143                   50     2.86×       35.8%

Table 3: Detailed Speculative Execution Performance
Threads  Sequential Time (ms)  Parallel Time (ms)  Speedup  Wasted %
1                     262                    -        -      50.0%
2                     274                  287     0.95×      50.0%
4                     302                  176     1.72×      50.0%
8                     281                  133     2.11×      50.0%

5.4 Code Output Snippets

The implementation generates the following output format:

N-Queens Data:
   threads    time
0        1  0.143
1        2  0.068
2        4  0.044
3        8  0.050

Speculative Execution Data:
   threads   t_seq   t_spec  wasted_pct
0        1  0.262    0.583        50.0
1        2  0.274    0.287        50.0
2        4  0.302    0.176        50.0
3        8  0.281    0.133        50.0

6. OBSERVATIONS AND CONCLUSIONS

6.1 Key Observations

1. N-Queens Performance: Demonstrates excellent scalability up to 4 threads with peak speedup of 3.25×. The super-linear efficiency at 2 threads (105%) suggests improved cache behavior and optimal work distribution.

2. Speculative Execution Overhead: Single-threaded speculative execution (583ms) performs significantly worse than sequential execution (262ms) due to unnecessary computational overhead.

3. Scaling Characteristics: N-Queens shows superior parallel efficiency due to independent problem decomposition, while speculative execution is fundamentally limited by fixed 50% computational waste.

4. Thread Scaling Limits: Both algorithms show performance degradation beyond 4 threads, with N-Queens experiencing slight slowdown at 8 threads (50ms vs 44ms at 4 threads).

6.2 Performance Trade-offs

N-Queens (Exploratory Parallelism):
Advantages: Near-zero computational waste, excellent scalability, natural problem decomposition
Disadvantages: Load balancing challenges at higher thread counts, diminishing returns beyond optimal thread count

Speculative Execution:
Advantages: Consistent performance improvement with thread scaling, predictable overhead characteristics
Disadvantages: Fixed 50% computational waste, poor single-threaded performance, limited maximum speedup

6.3 Conclusions

Exploratory parallelism, as demonstrated by the N-Queens problem, provides superior performance gains when problems can be naturally decomposed into independent subproblems. The technique achieves significant speedups with minimal computational waste.

Speculative execution offers moderate performance improvements but at the cost of guaranteed computational overhead. The technique becomes beneficial only when sufficient parallel resources are available to overcome the overhead penalty.

The experimental results demonstrate that:
- N-Queens achieves 3.25× speedup with near-zero waste
- Speculative execution achieves 2.11× speedup with 50% computational waste
- Both techniques show optimal performance at 4 threads for this system configuration

The choice between techniques depends on problem characteristics and resource availability. Exploratory parallelism is preferred for decomposable search problems, while speculative execution suits scenarios with unpredictable branching patterns and abundant computational resources.

7. FILES AND USAGE

Source Files:
- nqueens_openmp.c - N-Queens parallel implementation
- speculative_ifelse_win.c - Speculative execution implementation
- plot_results.py - Data analysis and visualization script

Data Files:
- nqueens_results.csv - N-Queens experimental results
- spec_results.csv - Speculative execution experimental results

Generated Outputs:
- nq_time.png - N-Queens time vs threads plot
- nq_speedup.png - N-Queens speedup analysis
- spec_time.png - Speculative execution time comparison
- spec_speedup.png - Speculative execution speedup analysis
- spec_wasted.png - Wasted computation visualization

Usage Commands:
Compile and run N-Queens:
gcc -fopenmp nqueens_openmp.c -o nqueens.exe
./nqueens.exe

Compile and run Speculative execution:
gcc -fopenmp speculative_ifelse_win.c -o speculative.exe
./speculative.exe

Generate analysis and plots:
python plot_results.py