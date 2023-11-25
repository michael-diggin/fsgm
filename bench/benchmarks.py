from fsgm import compute_kernel_matrix
from python_dfs import calc_kernel_m_dfs
from python_matrix import calc_kernel_matmul
import time
import matplotlib.pyplot as plt
import numpy as np


def load_words():
    words = []
    with open("bench/words.txt") as file:
        for line in file:
            words += [line.rstrip('\n')]
    return words
    

if __name__ == "__main__":
    input_words = load_words()
    alphabet = set([c for w in input_words for c in w])
    alphabet = list(alphabet)
    
    include_py_dfs = True
    
    x = range(1, 11)
    fsgm_times = []
    py_dfs_times = []
    py_matmul_times = []
    
    for m in x:
        t0 = time.time()
        fsgm_k = compute_kernel_matrix(input_words, alphabet, m)
        t1 = time.time()
        fsgm_times.append(float('%.2f'%(t1-t0)))
    
        if m > 5:
            py_matmul_times.append(0.0)
        else:
            t2 = time.time()
            py_matmul_k = calc_kernel_matmul(input_words, alphabet, m)
            t3 = time.time()
            py_matmul_times.append(float('%.2f'%(t3-t2)))
    
        if include_py_dfs:
            t4 = time.time()
            py_dfs_k = calc_kernel_m_dfs(input_words, alphabet, m)
            t5 = time.time()
            py_dfs_times.append(float('%.2f'%(t5-t4)))
        
    
    width = 0.33  # the width of the bars
    multiplier = 0
    
    all_times = {
        "fsgm": fsgm_times,
        "py_matmul": py_matmul_times,
    }
    
    if include_py_dfs:
        all_times["py_dfs"] = py_dfs_times

    fig, ax = plt.subplots(figsize=(25, 10))

    for attribute, measurement in all_times.items():
        offset = width * multiplier
        rects = ax.bar(np.arange(len(x)) + offset, measurement, width, label=attribute)
        ax.bar_label(rects, padding=3)
        multiplier += 1

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Time (seconds)')
    ax.set_title('Kernel Compute time')
    ax.set_xticks(np.arange(len(x)) + width, x)
    ax.legend(loc='upper left', ncols=len(all_times.keys()))
    ax.set_ylim(0, 50)

    plt.savefig("bench/results.png")
    