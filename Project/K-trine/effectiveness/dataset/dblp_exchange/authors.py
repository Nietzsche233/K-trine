######################
# Author: ????? ???? #
######################
#
#TODO:case-study
#Author->id
#
#output file 1:../dblp_cnt.txt
#n
#paper_cnt      Author_name
#...            ...
#...            ...
#
#output file 2:../dblp_edges.txt
#n      m
#inid   outid
#...    ...
#...    ...
#Authors have edge iff they co-publish at least 20 paper
#
#refer to cainiaojiaocheng
#https://blog.csdn.net/wisemanchen/article/details/109140755
######################################################

import codecs
import copy
from xml.sax import handler, make_parser
from xml.sax.handler import feature_external_ges
paper_tag = ('article'
             ,'inproceedings'
            #  ,'proceedings'
            #  ,'book'
            #  ,'incollection'
            #  ,'phdthesis'
            #  ,'mastersthesis'
            #  ,'www'
)

a_name_list = []
a_id_map = {}
edge_map = {}
art_auth_list = []
class mHandler(handler.ContentHandler):
    def __init__(self):
        self.is_in_article = 0
        self.is_author = 0
        self.is_year = 0
        self.is_booktitle = 0
        self.now_aid_list = []
        self.contents = []  # 存放content内容
        self.name = ""
        self.year = ""
        self.booktitle = ""

    def startDocument(self):
        print ('Document Start')
        
    def endDocument(self):
        print ('Document End')
        
    def startElement(self, name, attr):
        if name == 'inproceedings' :
            self.is_in_article = 1
        if self.is_in_article and name == 'author' :
            self.is_author = 1
        if self.is_in_article and name == 'year' :
            self.is_year = 1
        if self.is_in_article and name == 'booktitle' :
            self.is_booktitle = 1

    def characters(self, chrs) : 
        if chrs != '\n' :
            if self.is_in_article and self.is_author :
                self.contents.append(chrs)
                self.name = ''.join(self.contents)
            elif self.is_in_article and self.is_year :
                self.year = chrs.strip()
            elif self.is_in_article and self.is_booktitle :
                self.contents.append(chrs)
                self.booktitle = ''.join(self.contents)

    def endElement(self, name):
        self.contents.clear()
        if self.is_in_article and self.is_author :
            if self.name not in a_id_map.keys() :
                a_id_map[self.name] = [len(a_name_list), 1]
                a_name_list.append(self.name)
            else :
                a_id_map[self.name][1] += 1
            self.now_aid_list.append(a_id_map[self.name][0])
            self.is_author = 0
            
        if name == 'inproceedings':
            if self.booktitle == "SIGMOD Conference" and len(self.now_aid_list) <= 6 :
                art_auth_list.append(copy.deepcopy(self.now_aid_list))
                length = len(self.now_aid_list)
                for i in range(length) :
                    for j in range(i) :
                        u, v = self.now_aid_list[i], self.now_aid_list[j]
                        if (u > v) :
                            u, v = v, u
                        if u == v :
                            continue
                        if (u, v) not in edge_map.keys() :
                            edge_map[(u, v)] = 1
                        else :
                            edge_map[(u, v)] += 1
            self.now_aid_list = []
            self.is_in_article = 0
        if name == 'year' :
            self.is_year = 0
        if name == 'booktitle' :
            self.is_booktitle = 0
        
    

def parserDblpXml(source):
    handler = mHandler()
    parser = make_parser()
    parser.setContentHandler(handler)
    parser.parse(source)
    

if __name__ == '__main__':
    source = codecs.open('dblp.xml','r', encoding="ISO-8859-1")
    parserDblpXml(source)
    source.close()
    cnt_dir = "dblp_cnt.txt"
    edgs_dir = "dblp_edges_SIGMODlim6.txt"
    art_dir = "dblp_coau_SIGMODlim6.txt"
    with open (cnt_dir, 'w', encoding="ISO-8859-1") as f :
        f.write(f"{len(a_name_list)}\n")
        for line in a_name_list :
            f.write(f"{a_id_map[line][1]}\t{line}\n")
    used_edgs = []
    for (key, value) in edge_map.items() :
        if value >= 1 :
            used_edgs.append(key)
    with open (edgs_dir, 'w') as f :
        f.write(f"{len(a_name_list)}\t{len(used_edgs)}\n")
        for line in used_edgs :
            f.write(f"{line[0]}\t{line[1]}\n")

    with open (art_dir, 'w') as f :
        for line in art_auth_list :
            for auth in line :
                f.write(f"{auth}\t")
            f.write(f"\n")
