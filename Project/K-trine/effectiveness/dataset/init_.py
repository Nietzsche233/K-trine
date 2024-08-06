# give edgs.txt n and m
# new file: 
# n
# id cnt
# ...
import os
import sys
import time
import random
import matplotlib.pyplot as plt
import heapq
def read(epath, ipath, opath) :
    n = 0
    edges = []
    has_edge = set()
    with open(epath, "r") as f:
        for line in f :
            x = line.strip().split()
            y = list(x)
            u, v = int(y[0]), int(y[1])
            if (u > v) :
                u, v = v, u
            if n <= v :
                n = v + 1
            if (u, v) not in has_edge :
                edges.append((u, v))
                has_edge.add((u, v))
    with open(epath, "w") as f:
        f.write(f"{n}\t{len(edges)}\n")
        for line in edges:
            f.write(f"{line[0]}\t{line[1]}\n")
    cnt = []
    while len(cnt) < n :
        cnt.append(0)
    with open(ipath, "r") as f:
        for line in f:
            x = line.strip().split()
            node = int(x[0])
            # print(node)
            # while n <= node :
            #     n += 1
            #     cnt.append(0)
            cnt[node] += 1
    print("....")
    with open(opath, "w") as f:
        f.write(f"{n}\n")
        for i in range(n) :
            f.write(f"{i}\t{cnt[i]}\n")
if __name__ == "__main__":
    edginput_dir, insinput_dir, out_dir = sys.argv[1], sys.argv[2], sys.argv[3]
    try :
        read(edginput_dir, insinput_dir, out_dir)
    except :
        if len(sys.argv) == 5 :
            log_dir = sys.argv[4]
            with open(log_dir,"w") as f:
                f.write("ERROR!!!")
        else :
            print("ERROR!!!!n")