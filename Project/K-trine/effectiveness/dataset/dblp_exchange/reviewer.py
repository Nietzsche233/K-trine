######################
# Author: ????? ???? #
######################
#
#TODO:
#review first 100 lines
#
######################################################

import json
fine_path = "dblp.xml"
output_path = "dblp_rev.txt"
xml_line = []
with open (fine_path, 'r', encoding="ISO-8859-1") as f :
    for i, line in enumerate(f):
        try:
            xml_line.append(line)
            if i == 19999:
                break
        except json.JSONDecodeError:
            print(f"Error xml on line {i+1}")

# 将前20000行的JSON数据保存到txt文件中
with open(output_path, 'w', encoding="ISO-8859-1") as output_file:
    for line in xml_line:
        output_file.write(line + '\n')

