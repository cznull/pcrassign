// pcrassign.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

struct action_t{
    std::vector<int> team;
    double point;
    std::string note;
};

struct assign_t {
    int t1, t2, t3;
    double point;
};

inline bool pointcmpa(const assign_t& a1, const assign_t& a2) {
    return a1.point > a2.point;
}

inline bool pointcmp(const action_t& a1, const action_t& a2) {
    return a1.point > a2.point;
}

int conflictcount3(std::vector<int> t1, std::vector<int> t2, std::vector<int> t3, std::vector<int>& box, std::vector<int>& buffer) {
    int count = 0;
    for (int i = 0; i < t1.size(); i++) {
        int c = t1[i];
        if (box[c]) {
            box[c] = 0;
            buffer.push_back(c);
        }
        else {
            count++;
        }
    }
    for (int i = 0; i < t2.size(); i++) {
        int c = t2[i];
        if (box[c]) {
            box[c] = 0;
            buffer.push_back(c);
        }
        else {
            count++;
        }
    }
    for (int i = 0; i < t3.size(); i++) {
        int c = t3[i];
        if (box[c]) {
            box[c] = 0;
            buffer.push_back(c);
        }
        else {
            count++;
        }
    }
    for (int i = 0; i < buffer.size(); i++) {
        box[buffer[i]] = 1;
    }
    buffer.clear();
    return count;
}

int conflictcount2(std::vector<int> t1, std::vector<int> t2, std::vector<int>& box, std::vector<int>& buffer) {
    int count = 0;
    for (int i = 0; i < t1.size(); i++) {
        int c = t1[i];
        if (box[c]) {
            box[c] = 0;
            buffer.push_back(c);
        }
        else {
            count++;
        }
    }
    for (int i = 0; i < t2.size(); i++) {
        int c = t2[i];
        if (box[c]) {
            box[c] = 0;
            buffer.push_back(c);
        }
        else {
            count++;
        }
    }
    for (int i = 0; i < buffer.size(); i++) {
        box[buffer[i]] = 1;
    }
    buffer.clear();
    return count;
}

int conflictcount1(std::vector<int> t1, std::vector<int>& box, std::vector<int>& buffer) {
    int count = 0;
    for (int i = 0; i < t1.size(); i++) {
        int c = t1[i];
        if (box[c]) {
            box[c] = 0;
            buffer.push_back(c);
        }
        else {
            count++;
        }
    }
    for (int i = 0; i < buffer.size(); i++) {
        box[buffer[i]] = 1;
    }
    buffer.clear();
    return count;
}

int checkconflict(std::vector<int> t0, std::vector<int> t1, std::vector<int> t2, std::vector<int>& box, std::vector<int>& buffer) {
    if (conflictcount3(t0, t1, t2, box,buffer) > 3) {
        return -1;
    }
    if (conflictcount2(t0, t1, box, buffer) > 2) {
        return -1;
    }
    if (conflictcount2(t1, t2, box, buffer) > 2) {
        return -1;
    }
    if (conflictcount2(t2, t0, box, buffer) > 2) {
        return -1;
    }
    if (conflictcount1(t0, box, buffer) > 1) {
        return -1;
    }
    if (conflictcount1(t1, box, buffer) > 1) {
        return -1;
    }
    if (conflictcount1(t2, box, buffer) > 1) {
        return -1;
    }
    return 0;
}

int cmp(const char* s1, const char* s2, int nof2) {
    for (int i = 0; i < nof2; i++) {
        if (!s1[i] || s1[i] != s2[i]) {
            return 1;
        }
    }
    return 0;
}

int find(const char* text, int textlength, const char* tag, int taglength) {
    int i;
    for (i = 0; i <= textlength - taglength; i++) {
        if (!cmp(text + i, tag, taglength)) {
            return i;
        }
    }
    return -1;
}

int getline(const char* s, int length, std::vector<std::string>& line, const char* flag, int flaglength) {
    int cur, next;
    cur = 0;
    next = find(s + cur, length - cur, flag, flaglength);
    while (next >= 0) {
        if (next > 0) {
            //int i;
            //for (i = 0; i < next&&s[cur+i]=='/'; i++);
            //if (next > i) {
            line.push_back(std::string(s + cur, next));
            //}
        }
        cur += next + flaglength;
        next = find(s + cur, length - cur, flag, flaglength);
    }
    if (length > cur) {
        line.push_back(std::string(s + cur, length - cur));
    }
    return 0;
}

int load(const char *filename,std::vector<action_t> &actions, std::vector<int>& box, std::map<std::string, int> &namemap, std::vector<std::string> &names,int damageonly) {
    FILE* fi;
    char* ficon;
    int ficount;
    std::vector<std::string> line;
    if (!fopen_s(&fi, filename, "rb")) {
        fseek(fi, 0, SEEK_END);
        ficount = ftell(fi);
        fseek(fi, 0, SEEK_SET);
        ficon = (char*)malloc(ficount * sizeof(char));
        ficount = fread(ficon, 1, ficount, fi);
        fclose(fi);
        getline(ficon, ficount, line, "\r\n", 2);
        for (int i = 0; i < line.size() - 1; i++) {
            std::vector<std::string> line1;
            action_t action;
            getline(line[i].c_str(), line[i].length(), line1, " ", 1);
            for (int j = 0; j < 5; j++) {
                auto it = namemap.find(line1[j]);
                if (it == namemap.end()) {
                    namemap.insert(std::make_pair(line1[j], names.size()));
                    action.team.push_back(names.size());
                    names.push_back(line1[j]);
                }
                else {
                    action.team.push_back(it->second);
                }
            }
            double p, s;
            sscanf_s(line1[5].c_str(), "%lf", &p);
            sscanf_s(line1[6].c_str(), "%lf", &s);
            if (line1.size() > 7) {
                action.note = line1[7];
            }
            if (damageonly) {
                s = fmin(s, 1.0);
            }
            action.point = p * s;
            actions.push_back(action);
        }

        box.resize(names.size(), 1);

        std::vector<std::string> line1;
        getline(line[line.size() - 1].c_str(), line[line.size() - 1].length(), line1, " ", 1);
        for (int j = 0; j < line1.size(); j++) {
            auto it = namemap.find(line1[j]);
            if (it != namemap.end()) {
                box[it->second] = 0;
            }
        }
    }

    std::sort(actions.begin(), actions.end(), pointcmp);

    return 0;
}

int main(int argc,char** argv) {
    const char* filename = "d:/files/data/pcr/text.txt";
    int maxoutput = 10;
    int damageonly = 0;

    if (argc > 1) {
        filename = argv[1];
    }

    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "-n")) {
            i++;
            if (i < argc) {
                sscanf_s(argv[i], "%d", &maxoutput);
            }
        }
        if (!strcmp(argv[i], "-damageonly")) {
            damageonly = 1;
        }
    }

    std::vector<int> box;
    std::vector<int> buffer;
    std::map<std::string, int> namemap;
    std::vector<std::string> names; 
    std::vector<action_t> actions;
    load(filename, actions, box, namemap, names, damageonly);
    
    std::vector<assign_t> as;
    for (int i = 0; i < actions.size(); i++) {
        for (int j = i; j < actions.size(); j++) {
            for (int k = j; k < actions.size(); k++) {
                as.push_back({ i,j,k,actions[i].point + actions[j].point + actions[k].point });
            }
        }
    }

    std::sort(as.begin(), as.end(), pointcmpa);
    int outputcount = 0;
    for (int a = 0; a < as.size(); a++) {
        int i = as[a].t1;
        int j = as[a].t2;
        int k = as[a].t3;
        if (!checkconflict(actions[i].team, actions[j].team, actions[k].team, box, buffer)) {
            printf("%s,%s,%s,%s,%s,%.1f,%s\n", names[actions[i].team[0]].c_str(), names[actions[i].team[1]].c_str(), names[actions[i].team[2]].c_str(), names[actions[i].team[3]].c_str(), names[actions[i].team[4]].c_str(), actions[i].point, actions[i].note.c_str());
            printf("%s,%s,%s,%s,%s,%.1f,%s\n", names[actions[j].team[0]].c_str(), names[actions[j].team[1]].c_str(), names[actions[j].team[2]].c_str(), names[actions[j].team[3]].c_str(), names[actions[j].team[4]].c_str(), actions[j].point, actions[j].note.c_str());
            printf("%s,%s,%s,%s,%s,%.1f,%s\n", names[actions[k].team[0]].c_str(), names[actions[k].team[1]].c_str(), names[actions[k].team[2]].c_str(), names[actions[k].team[3]].c_str(), names[actions[k].team[4]].c_str(), actions[k].point, actions[k].note.c_str());
            printf("\n");

            outputcount++;
            if (outputcount >=maxoutput) {
                return 0;
            }
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
