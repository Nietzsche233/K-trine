######################
# Author: ????? ???? #
######################
#
#TODO:
#review first 10 lines
#
######################################################

import json
fine_path = "yelp_academic_dataset_user.json"
output_path = "yelp_user_rev.txt"
with open (fine_path, 'r') as f :
    json_line = []
    for i, line in enumerate(f):
        try:
            data = json.loads(line)
            json_line.append(json.dumps(data))
            if i == 9:
                break
        except json.JSONDecodeError:
            print(f"Error decoding JSON on line {i+1}")

# 将前20行的JSON数据保存到txt文件中
with open(output_path, 'w') as output_file:
    for line in json_line:
        output_file.write(line + '\n')

