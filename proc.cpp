/*
  Gets the processes from /proc and hierarchizes them using a general tree
  din -> attr/current, attr/display, cmdline, comm, gid_map , sessionid, uid_map, cwd
      -> **status**, **limits**, **stat** (din astea doar esentialul)

*/

#include "tree.cpp"
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
namespace proc {

    Tree ProcessTree;

    class ProcessScraper {
    private:
        const std::string path = "/proc";

        std::vector<int> procList;
        std::map<int, std::vector<int>> parentDict;// un PPid -> vector de Pid asociati lui

        int getParentID (int pid) {
            int ppid; // PPid se afla in /proc/pid/status la randul ce incepe cu PPid -> linia 7

            // status file
            std::string statPath = "";
            statPath += "/proc/" + std::to_string(pid) + "/status";

            std::ifstream fin(statPath);
            std::string line;
            for(unsigned i=0;i<7;i++)
                std::getline(fin,line);   
            fin.close();
            std::stringstream conv(line.substr(line.find("\t"))); //convert string to int
            conv>>ppid;
            // std::cout<<ppid<<"\n";
            // std::ifstream stat(statPath);

            return ppid;
        }
    public:
        void scrapeProcesses() {
            for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
                std::string fileName = entry.path().filename();
                if (fileName[0] >= '0' && fileName[0] <= '9') {
                    int processID = std::stoi(fileName);

                    procList.push_back(processID);
                    parentDict[getParentID(processID)].push_back(processID);
                }
            }
        }

        void printProcesses() {
            // Procese:
            // for (auto i: procList) {
            //     std::cout << i << std::endl;
            // }
            // Procese grupate dupa PPid:
             for(auto it = parentDict.begin();it != parentDict.end();it++)
              {  
                std::cout << "\033[1;31m"<<(it->first)<<"\t->"<<"\033[0m";
                    for(auto it2 : it->second)
                        std::cout<<it2<<" ";
                std::cout<<"\n";
                }
  
        }

        ProcessScraper() {
            scrapeProcesses();
        }
    };
}
