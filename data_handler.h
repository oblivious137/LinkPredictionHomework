//
// Created by 吴先 on 2017/10/20.
//

#ifndef LINKPREDICTIONHOMEWORK_DATA_HANDLER_H
#define LINKPREDICTIONHOMEWORK_DATA_HANDLER_H

#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

// 数据定义详见ReadMe.txt
set<string> id_list;
map<string, string> titles;
map<string, string> abstracts;
map<string, vector<string> > authors;
map<string, vector<string> > known_citings;
map<string, vector<double> > docvecs;

// 默认的全局数据路径
map<string, string> create_file_path_config() {
    map<string, string> m;
    m["abstracts"] = "data/abstracts.txt";
    m["authors"] = "data/authors.txt";
    m["docvecs"] = "data/docvecs.txt";
    m["id_list"] = "data/id_list.txt";
    m["known_citings"] = "data/known_citings.txt";
    m["train_list"] = "data/train_list.txt";
    m["test_list"] = "data/test_list.txt";
    m["titles"] = "data/titles.txt";
    return m;
}
map<string, string> FILE_PATH_CONFIG = create_file_path_config();

// 字符串分割
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// 字符串去前后空格
string& strip(string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0,s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
}

// 通用的读取数据方法
int load_data(const string &filename, map<string, string> &data) {
    ifstream fin(filename);
    string id, buff;
    while (fin>>id) {
        getline(fin, buff);
        buff = strip(buff).substr(buff.find('"'), buff.rfind('"')-buff.find('"')+1);
        data[id] = buff;
    }
    return 0;
}

// 读入全部数据，并保存在全局变量中
int load_all_data(map<string, string> &config=FILE_PATH_CONFIG) {
    ifstream fin(config["id_list"]);
    string id;
    while (fin>>id) {
        id_list.insert(id);
    }

    map<string, string> buff;
    load_data(config["abstracts"], abstracts);
    load_data(config["titles"], titles);
    load_data(config["authors"], buff);
    for (map<string, string>::const_iterator mitr = buff.begin(); mitr != buff.end(); ++mitr) {
        authors[mitr->first] = split(mitr->second, '|');
    }
    load_data(config["known_citings"], buff);
    for (map<string, string>::const_iterator mitr = buff.begin(); mitr != buff.end(); ++mitr) {
        known_citings[mitr->first] = split(mitr->second, '|');
    }
    load_data(config["docvecs"], buff);
    for (map<string, string>::const_iterator mitr = buff.begin(); mitr != buff.end(); ++mitr) {
        vector<string> tmp = split(mitr->second, '|');
        docvecs[mitr->first].resize(tmp.size());
        for (int i = 0; i < tmp.size(); i++)
            docvecs[mitr->first][i] = strtod(tmp[i].c_str(), nullptr);
    }
    return 0;
}

#endif //LINKPREDICTIONHOMEWORK_DATA_HANDLER_H
