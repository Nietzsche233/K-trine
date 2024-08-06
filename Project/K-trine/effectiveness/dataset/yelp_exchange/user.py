######################
# Author: ????? ???? #
######################
#
#TODO:
#Author->id
#
#output file 1:../yelp_index.txt (if it is not exist)
#n
#exchangeid user_id
#... ...
#... ...
#
#output file 2:../yelp_edges.txt (if it is not exist)
#n      m
#inid   outid
#...    ...
#...    ...
#
#output file 3:../yelp_*.txt
#n
#id     value
#...    ...
#...    ...
#
######################################################

file_tag = (
    'user_id',
    'name',
    'review_count',
    'yelping_since',
    'friends',
    'useful',
    'funny',
    'cool',
    'fans',
    'elite',
    'average_stars',
    'compliment_hot',
    'compliment_more',
    'compliment_profile',
    'compliment_cute',
    'compliment_list',
    'compliment_note',
    'compliment_plain',
    'compliment_cool',
    'compliment_funny',
    'compliment_writer',
    'compliment_photos',
    '__'
)
import os
import sys
import json
org_path = "yelp_academic_dataset_user.json"
index_path = "yelp_index.txt"
edges_path = "yelp_edges.txt"

if __name__ == "__main__":
    job_tag = sys.argv[1]
    if job_tag not in file_tag :
        print("Error: ", job_tag, " is not exist!!!\n")
    index_exist = os.path.exists(index_path)
    edges_exist = os.path.exists(edges_path)
    org_data = {}
    user_id_list = []
    with open (org_path, 'r') as f :
        for i, line in enumerate(f):
            try:
                data = json.loads(line)
            except json.JSONDecodeError:
                print(f"Error decoding JSON on line {i+1}")
            user_id_list.append(data['user_id'])
            if edges_exist == False and job_tag != '__' :
                org_data[data['user_id']] = {'friends' : data['friends'], job_tag : data[job_tag]}
            elif edges_exist == False :
                org_data[data['user_id']] = {job_tag : data[job_tag]}
            elif job_tag != '__' :
                org_data[data['user_id']] = {job_tag : data[job_tag]}
            else :
                exit()        
            
    user_id_index = {}
    if index_exist :
        with open (index_path, 'r') as f :
            nn = -1
            for line in f :
                x = line.strip().split()
                y = list(x)
                if nn == -1 :
                    nn = int(y[0])
                    continue
                i = int(y[0])
                uid = (y[1])
                user_id_index[uid] = i
    else :
        with open (index_path, 'w') as f :
            n = len(user_id_list)
            f.write(f"{n}\n")
            for i, uid in enumerate(user_id_list) :
                user_id_index[uid] = i
                f.write(f"{i}\t{uid}\n")
    if edges_exist == False :
        edges = set()
        edge_list = []
        for uid in user_id_list :
            u = user_id_index[uid]
            friend_list = org_data[uid]['friends'].split(', ')
            # print(friend_list)
            for vid in friend_list :
                if vid not in user_id_index.keys() :
                    print(vid)
                    continue
                v = user_id_index[vid]
                uu, vv = u, v 
                if u > v :
                    uu, vv = v, u
                if (uu, vv) in edges :
                    continue
                edge_list.append((uu, vv))
                edges.add((uu, vv))
        n = len(user_id_list)
        m = len(edge_list)
        with open (edges_path, "w") as f :
            f.write(f"{n}\t{m}\n")
            for e in edge_list :
                f.write(f"{e[0]}\t{e[1]}\n")
    
    if job_tag != '__' :
        job_path = "yelp_"+job_tag+".txt"
        with open (job_path, "w") as f :
            n = len(user_id_list)
            f.write(f"{n}\n")
            for uid in user_id_list :
                i = user_id_index[uid]
                con = org_data[uid][job_tag]
                f.write(f"{i}\t{con}\n")
            



