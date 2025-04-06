# FINM327_Project

## Phase1 Discussion
**1. Explain the key differences between pointers and references in C++. When would you choose to use a pointer over a reference, and vice versa, in the context of implementing numerical algorithms?**

Pointers can be reassigned, point to `nullptr`, and are mainly used for dynamic memory and arrays. References are like constant aliases — can’t be null and can’t be changed to refer to something else after initialization. In this project, since I needed to pass arrays (like matrix and vector) and use direct memory access, pointers made more sense (also as suggested in the homework description). In practice, I feel pointers are just more flexible in this homework since space is also importabt.


**2. How does the row-major and column-major storage order of matrices affect memory access patterns and cache locality during matrix-vector and matrix-matrix multiplication? Provide specific examples from your implementations and benchmarking results.**

Row-major matrix has values naturally stored in the sequence of matrix(vector<vector>) generation. So, iterating row by row gives good cache performance. For example, in `multiply_mv_row_major`, memory access is linear, which fits well with cache. But in `multiply_mv_col_major`, accessing by column in row-major layout means jumping in memory and leads to more cache misses.  
In matrix-matrix multiplication, the naive version accesses B column-wise, which again causes poor cache behavior. But the transposed version, `multiply_mm_transposed_b`, accesses both A and B_T row by row, which improves locality. My benchmarks showed the transposed version was faster and more stable in most cases, especially for larger matrices.


**3. Describe how CPU caches work (L1, L2, L3) and explain the concepts of temporal and spatial locality. How did you try to exploit these concepts in your optimizations?**

Caches store recent memory values to avoid fetching from main memory repeatedly. L1 is fastest and smallest, L2 and L3 are slower but bigger. CPUs load data in blocks (cache lines). Spatial locality means accessing data close together in memory; temporal locality means accessing the same data again soon.  
In my code, I tried to walk through memory linearly (especially in row-major loops) to get better spatial locality. Also reused values like `a_ik` and `v_j` in inner loops, which helps with temporal locality. These are simple tricks but help performance a lot.


**4. What is memory alignment, and why is it important for performance? Did you observe a significant performance difference between aligned and unaligned memory in your experiments? Explain your findings.**

Memory alignment is making sure data starts at memory addresses that match the CPU’s word or cache line size (like 64 bytes). I used `_aligned_malloc` to align all arrays. Performance was quite significantly better for aligned memory, especially in smaller to medium sizes. i think the timing was more stable as well — lower standard deviation. It’s not a game changer alone, but when combined with other optimizations, it helps make everything more efficient and predictable.

**5. Discuss the role of compiler optimizations (like inlining) in achieving high performance. How did the optimization level affect the performance of your baseline and optimized implementations? What are the potential drawbacks of aggressive optimization?**

I tested both `-O0` and `-O3`. With `-O3`, runtimes dropped a lot. The compiler automatically inlines functions, unrolls loops, vectorizes code, etc. I didn’t manually use `inline` because I didn’t have helper functions. (from hw1, we know that inline writings could be more efficient) Still, performance improved a lot just by switching to `-O3`.  
But sometimes the compiler may make changes that hurt manual optimizations. For example, `multiply_mm_transposed_b` performed worse than `mm_naive` under `-O3` in large sizes — **maybe** due to loop reordering or assumptions made by the optimizer. Also, debugging under `-O3` is harder because variables get optimized away.


**6. Based on your profiling experience, what were the main performance bottlenecks in your initial implementations? How did your profiling results guide your optimization efforts?**

The initial implementation of the transposed matrix multiplication wasn’t performing as expected. From the profiling and benchmarking results, I noticed that even though it should be more cache-friendly, it underperformed compared to the naive version, especially under compiler optimizations. After reviewing the implementation, I realized the issue was that it wasn’t structured in a fully parallel way to mm_naive. The loop order and memory access patterns needed to be carefully matched. Once I fixed that and made the access in the same row-major aligned structure (but using the transposed matrix), the performance improved noticeably, especially in larger matrix sizes. Profiling helped identify the hotspot in the inner loop where the performance loss was happening.


**7. Reflect on the teamwork aspect of this assignment. How did dividing the initial implementation tasks and then collaborating on analysis and optimization work? What were the challenges and benefits of this approach?**

I did the whole assignment on my own. That meant I had to handle implementation, benchmarking, optimization, and writing. It took more time, but I got to learn every part thoroughly. 

