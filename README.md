# quick-similarity-kernel
Implementation of an algorithm to quickly calculate string similarity Gram Kernel matrices.

## Background
Given a set of strings $S$ and an alphabet $A$, define the kernel function
$$
K_m(s_i, s_j) = \sum_{w:|w|=m} count(s_i, w)*count(s_j, w)
$$
Where $w$ is a word made from $A$ of length $m$ and $count(s_i, w)$ is the
number of occurences of the substring $w$ in $s_i$ with overlaps.

Then the Gram Kernel Matrix id defined as

$G^m_{ij} = K_m(s_i, s_j)$

This gets computationally very expensive as $m$ gets largers, since the search space $\{w \in A:|w|=m\}$ has $A^m$ elements. A very naive algorithm can take a huge amount of compute resoures, e.g.
```python
for i1, s1 in enumerate(S):
    for i2, s2 in enumerate(S):
        for w in length_m_words:
            kernel[i1, i2] += count(s1, w)*count(s2, w)
```

## Algortihmic details
This repository hosts an alogirthm for computing the exact matrix $G^m_{ij}$ in a faster set of steps. It is currently implemented in C++ with plans to expose a Python wrapper on top of it.

It makes use of the fact that for larger $m$, the probability that, for a given $w$, $count(s_i, w) > 0$ is $len(s_i)/|A|^m$, which gets smaller as $m$ increases. 
Secodly, it's clear that if $count(s, w) = 0$, then for any $a \in A$ we also have $count(s, w+a) = 0$. Thirdly $G$ is symmetric, so computing only the upper trainglur form is sufficient to compute all of $G$. 

Using this, the algorithm performs _depth first search_ for each $s_i \in S$ starting from each individual character $a \in A$ and building words $w$ up to length $m$. If at any point $count(s_i, w) = 0$, it skips over all words that are prefixed with $w$. Any non zero count $c_i = count(s_i, w)$ where $|w| = m$ is recorded into a map as $\{w: (i, c)\}$, and all tuples $(w, c_i)$ are fetched that start with the character $a$. It then iterates over all $j < i$, finds $c_j = count(s_j, w)$ in the recorded map and adds $c*c_j$ to the $(i, j)$ entry of a matrix $K$. (This finally step is optimised further, since all $count(s_j, w)$ have already been recorded in the map, as $j<i$, the iteration over $j$ can be replaced by an iteration over the map entries for the given $w$).  

Finally $G  = K + K^T - Diag(K)$. 

## Other implementations
A less complicated algorithm makes use of matrix multiplication instead. Since $G$ is symmetric positive semi definite it can be written as $G = XX^T$, where $X_{ij} = count(s_i, w_j)$. A similar depth first search can be used here to find each entry making the algorithm also very quick.

The problem with this approach, is that as a matrix $X$ can be enormous, of size $(|S|, |A|^m)$ so very quickly the memory requirements become an obstacle, even though $G$ itself is only of size $(|S|, |S|)$. Sparse matrices can be used instead, however there will still be a point where performing matrix multiplication isn't possible anymore due to the size of $|A|^m$ as it overtakes $2^{64}$

This issue is what has led to the algortihm in this repository. 

### Dependencies:
Requires the header files for [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) to be present in the include path.
