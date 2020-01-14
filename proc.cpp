/*
  Gets the processes from /proc and hierarchizes them using a general tree
  din -> attr/current, attr/display, cmdline, comm, gid_map , sessionid, uid_map, cwd
      -> **status**, **limits**, **stat** (din astea doar esentialul)

*/

#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
namespace proc {

    class ProcessScraper {
    private:
        const std::string path = "/proc";


        std::map<std::string, std::vector<int>> parentDict;// un PPid -> vector de Pid asociati lui

        std::string getParentID (int pid) {
            std::string ppid; // PPid se afla in /proc/pid/status la randul ce incepe cu PPid -> linia 7

            // status file
            std::string statPath = "";
            statPath += "/proc/" + std::to_string(pid) + "/status";

            std::ifstream fin(statPath);
            std::string line;
            for(unsigned i=0;i<7;i++)
                std::getline(fin,line);
            fin.close();
            ppid = line.substr(line.find("\t")+1);
            return ppid;
        }
    public:
        void scrapeProcesses() {
            for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
                std::string fileName = entry.path().filename();
                if (fileName[0] > '0' && fileName[0] <= '9') {
                    int processID = std::stoi(fileName);

                    parentDict[getParentID(processID)].push_back(processID);
                }
            }
        }

        void printProcesses() {
            // Procese grupate dupa PPid:
            // for(auto it = parentDict.begin();it != parentDict.end();it++)
            //  {
            //    std::cout << "\033[1;31m"<<(it->first)<<"\t->"<<"\033[0m";        // afisare doar pt verificare
            //        for(auto it2 : it->second)
            //            std::cout<<it2<<" ";
            //    std::cout<<"\n";
            //    }
            std::experimental::filesystem::remove_all("./bin/0/");          // stergem tot ce era inainte
            std::experimental::filesystem::create_directory("./bin/0/");    // creem radacina
            createPSTree("0","./bin/0/");                                   // recursiv toti fii, nepotii etc

        }

        void createPSTree(std::string ppid, std::string current_path) {

            if(parentDict.find(ppid) != parentDict.end())
                for (auto it: parentDict[ppid])
                {
                    std::string new_path = current_path  + std::to_string(it) + "/";
                    std::experimental::filesystem::create_directory(new_path);
                    createPSTree(std::to_string(it), new_path);
                }
            else
            {
                // nu il gaseste deci e frunza
                // daca e frunza trebuie sa scriem in el un fisier status 
            }

        }

        ProcessScraper() {
            scrapeProcesses();
        }
    };
}
