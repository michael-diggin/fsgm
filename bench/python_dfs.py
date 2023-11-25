import numpy as np

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

# cache maps the substrings of length w
# the index i of a smiles entry and it's count(s, w) c
# {substring w : {index i: count c}}
cache = {}
def add_to_cache(ind, w, c):
    if cache.get(w, None):
      cache[w][ind] = c
      return
    cache[w] = {ind:c}
    return

def reset_cache():
    cache.clear()

# This function computes the Kernel matrix
# given the list of characters and a desired m-value
def calc_kernel_m_dfs(inputs, chars, m):
    reset_cache()
    kernel = np.zeros((len(inputs), len(inputs)))

    for i1, s in enumerate(inputs):
      for w in chars:
        output = gen_counts_with_words(s, w, m, chars, i1)
        for word, c1 in output:
          for i2, c2 in cache.get(word).items():
            kernel[i1, i2] += c1*c2

    return kernel + kernel.transpose() - np.diag(np.diag(kernel))


# gen_counts performs depth-first search
# building words from the set of characters
def gen_counts_with_words(s, w, m, chars, ind):
    # output is list of tuples
    # elem 1 is the word
    # elem 2 is the data/count
    output = []
    if len(w) == m:
      c = count(s, w)
      if c > 0:
        add_to_cache(ind, w, c)
        return [(w, c)]
      return []

    for c in chars:
      if has_substring(s, w+c):
        ret = gen_counts_with_words(s, w+c, m, chars, ind)
        output += ret

    return output

