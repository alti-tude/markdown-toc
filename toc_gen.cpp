#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include<assert.h>

std::string GetDir(std::string& path){
    std::stringstream s;
    int prev = 0;
    for(int i=0;i<path.size();i++){
        if(path[i]=='/'){
            s<<path.substr(prev, i-prev+1);
            prev = i+1;
        }
    }
    return s.str();
}

std::vector<std::string> GetSections(std::string& file){
    std::ifstream stream(file);
    assert(stream.is_open());
    std::string line;

    enum {
        CODE, TEXT
    };

    int mode = TEXT;
    std::vector<std::string> sections;

    while(getline(stream, line)){
        if(line.find("```") != std::string::npos && line.find("```") == 0){
            if(mode == TEXT) mode = CODE;
            else mode = TEXT;
        }
        else if(mode == TEXT){
            if(line[0]=='#'){
                sections.push_back(line);
            }
        }
    }

    stream.close();
    return sections;
}  

struct StrippedStrings{
    int numHash;
    std::string str;
};

StrippedStrings StripHash(std::string& str){
    std::string stripped;
    int count = 0;
    while(str[count++] == '#');
    unsigned int size = str.size()-1;
    while(str[size--] == ' ');

    stripped = str.substr(count, size-count+2);
    return {count-1, stripped};
}

std::string CreateAnchor(std::string& line){
    std::stringstream anchor;

    for(int i=0;i<line.size();i++){
        if(line[i] == '_') continue;

        if(line[i] == ' ')
            line[i] = '-';
        else if(line[i]-'a'<0 && line[i]-'A'>=0)
            line[i] = line[i] - 'A' + 'a';
        anchor << line[i]; 
    }
    return anchor.str();
}

std::vector<std::string> GenTOC(std::vector<std::string> sections){
    std::vector<int> numbering;
    std::vector<std::string> toc;

    for(int i=0;i<sections.size();i++){
        StrippedStrings ss = StripHash(sections[i]);
        
        while(ss.numHash > numbering.size()) 
            numbering.push_back(0);
        while(numbering.size() > ss.numHash) 
            numbering.pop_back();
        
        numbering[ss.numHash-1]++;

        std::stringstream line;
        for(int i=0;i<numbering.size()-1;i++) 
            line << "\t";
        line << "- [";

        for(int i=0;i<numbering.size();i++) 
            line << numbering[i] << ".";
        
        line << " " << ss.str <<"]";
        line << "(#" << CreateAnchor(ss.str) << ")";
        toc.push_back(line.str());
    }

    return toc;
}
int main(){
    std::string file;
    std::cin >> file;

    std::string dir = GetDir(file);
    std::ofstream op(dir+"withTOC.md");
    assert(op.is_open());

    std::vector<std::string> sections = GetSections(file);
    std::vector<std::string> toc = GenTOC(sections);
    for(int i=0;i<toc.size();i++) op << toc[i] << std::endl;

    std::ifstream ip(file);
    assert(ip.is_open());
    
    std::string line;
    while(getline(ip, line))
        op << line << std::endl;
    
    ip.close();
    op.close();
    
    return 0;
}