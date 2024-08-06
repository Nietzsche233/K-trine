
if __name__ == '__main__':
    inp_dir = "ind_6-trussdblp_edges_SIGMODlim6.txt"
    cnt_dir = "dblp_cnt.txt"
    co_au_dir = "dblp_coau_SIGMODlim6.txt"
    case_node = "ind_6-trussdblp_node_SIGMODlim6.csv"
    org_node_con = []
    with open (cnt_dir, 'r', encoding="ISO-8859-1") as f :
        j = 0
        for line in f :
            if j == 0 :
                j = 1
                continue
            x = line.strip().split()
            paper_cnt = int(x[0])
            name = ""
            j = 0
            for y in x :
                if j == 0 :
                    j += 1
                    continue
                name += y
                name += " "
            org_node_con.append((paper_cnt,name))
                
    node_con = []
    co_paper = {}
    with open (inp_dir, 'r', encoding="ISO-8859-1") as f :
        for line in f :
            x = line.strip().split()
            id = int(x[0])
            co_paper[id] = 0
            lable = int(x[1])
            paper_cnt = org_node_con[id][0]
            name = org_node_con[id][1]
            node_con.append((id, lable, paper_cnt, name))
    with open (co_au_dir, 'r', encoding="ISO-8859-1") as f :
        for line in f :
            x = line.strip().split()
            flag = 0
            for y in x :
                if int(y) in co_paper.keys() :
                    flag += 1
            if flag >= 1 or flag >= len(x) :
                for y in x :
                    if int(y) in co_paper.keys() :
                        co_paper[int(y)] += 1
    
    with open (case_node, 'w', encoding="ISO-8859-1") as f :
        f.write(f"ID, Label, Cnt, Name\n")
        for line in node_con :
            f.write(f"{line[0]},{line[1]},{co_paper[line[0]]},{line[3]}\n")
            # f.write(f"{line[0]},{line[1]},{line[2]},{line[3]}\n")

