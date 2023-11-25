import array
import numpy as np
from scipy.sparse import csr_array

# the count(s, w) returns the number of times 'w' is
# a substring of 's' _with_ overlaps
def count(s, w):
    count = 0
    index = 0
    while True:
        index = s.find(w, index) + 1
        if index > 0:
            count+=1
        else:
            return count
        
# returns true if there is at least one match
def has_substring(s, w):
    return s.find(w, 0) > -1

# This function computes the sparse matrices needed for M
# given the list of characters and a desired m-value
def calc_kernel_matmul(inputs, chars, m):
    rows = array.array('l')
    cols = array.array('l')
    data = array.array('i')

    for i1, s in enumerate(inputs):
      for i2, w in enumerate(chars):
        output = gen_counts(s, w, m, chars)
        for pos, c in output:
          col_pos = calc_column_pos(pos+[i2], len(chars))
          rows.append(i1)
          cols.append(col_pos) # handles the jumping forward
          data.append(c)

    rows = np.frombuffer(rows, dtype=np.int64)
    cols = np.frombuffer(cols, dtype=np.int64)
    data = np.frombuffer(data, dtype=np.int32)

    m = csr_array((data, (rows, cols)), shape=(len(inputs), len(chars)**m))
    k = m @ m.transpose()
    return k.toarray()

# gen_counts performs depth-first search
# building words from the set of characters
def gen_counts(s, w, m, chars):
    # output is list of tuples
    # elem 1 is the col positions
    # elem 2 is the data/count
    output = []
    if len(w) == m:
      c = count(s, w)
      if c > 0:
        return [([], c)]
      return []

    for i, c in enumerate(chars):
      if has_substring(s, w+c):
        ret = gen_counts(s, w+c, m, chars)
        output += [(elem1+[i], elem2) for (elem1, elem2) in ret]

    return output

# calculates the column postion in the matrix
# given the character indexes during DFS
def calc_column_pos(positions, length):
    pos = 0
    for i, p in enumerate(positions):
      pos += p*(length**i)
    return pos