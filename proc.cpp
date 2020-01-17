#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <set>

namespace proc {

    struct Parameters{
	    std::string path = " ";
        std::string ppid = " ";
        std::string status = " ";
    };

    void* make_directory(void *argv){
        Parameters *aux = (Parameters*)argv;
        std::experimental::filesystem::path p(aux->path);

        while(!std::experimental::filesystem::exists(p.parent_path())){
            //std::cout<<"Blocat\n";
        };

        std::experimental::filesystem::create_directory(aux->path);
        return (void*)true;
    }

    void* make_directories(void *argv){
        Parameters *aux = (Parameters*)argv;

        std::experimental::filesystem::path p(aux->path);

        while(!std::experimental::filesystem::exists(p.parent_path())){
            ///std::cout<<"Blocat iar\n";
        };

        std::ofstream fout(aux->path);
        fout << "Statusul lui:\t" << aux->ppid << "\n\n\tStatus:\n" ;  // scriem in acel fisier

        fout << aux->status <<"\n";

        fout.close();
        return (void*)true;
    }

    class ProcessScraper {
    private:
        int processNo = 1, processIndex, fileIndex;
        pthread_t *processThreads, *filesThreads;
        Parameters *filesThreadsPaths;
        Parameters *processThreadsPaths;


    public:
        std::map<std::string, std::string> childStatus;
        std::map<std::string, std::vector<std::string>> parentDict;// un PPid -> vector de Pid asociati lui
        
        int getProcessNo(){return processNo;}

        void scrapeProcesses() {
            std::string path = "/dev/kproc";
            std::ifstream stat(path);
            std::string line;
            std::set<std::string> procesess;

            while(std::getline(stat, line)){
                std::string pid, ppid, processStatus;
                pid = line.substr(0, line.find(" "));

                line  = line.substr(line.find(" ") + 1);
                ppid = line.substr(0, line.find(" "));

                processStatus = line.substr(line.find(" ") + 1);;

                procesess.insert(ppid);
                procesess.insert(pid);

                parentDict[ppid].push_back(pid);
                childStatus[pid] = processStatus;

            }

            processNo = procesess.size();

        }

        ProcessScraper() {
            scrapeProcesses();

            processThreads = new pthread_t[processNo];
            processThreadsPaths = new struct Parameters[processNo];

            processIndex = processNo - 1;
            fileIndex = processNo - 1;
            filesThreads = new pthread_t[processNo];
            filesThreadsPaths = new struct Parameters[processNo];
        }



        int printProcesses() {
            //Procese grupate dupa PPid:
            for(auto it = parentDict.begin();it != parentDict.end();it++)
             {
               std::cout << "\033[1;31m"<<(it->first)<<"\t->"<<"\033[0m";        // afisare doar pt verificare
                   for(auto it2 : it->second)
                       std::cout<<it2<<" ";
               std::cout<<"\n";
               }
            std::experimental::filesystem::remove_all("./bin/0");          // stergem tot ce era inainte
            std::experimental::filesystem::create_directory("./bin/0");    // cream radacina
            createPSTree("0","./bin/0/");    // recursiv toti fii, nepotii etc

            for(unsigned i = processNo - 1; i > processIndex; i--){
                void *rez;

                if(pthread_join(processThreads[i], &rez)){
                    perror(NULL);
                    return errno;
                }
            }
            for(unsigned i = processNo - 1; i > fileIndex; i--){
                void *rez;

                if(pthread_join(filesThreads[i], &rez)){
                    perror(NULL);
                    return errno;
                }
            }
            return 0;
        }

        void createPSTree(std::string ppid, std::string current_path) {
            if(parentDict.find(ppid) != parentDict.end())
                for (auto it: parentDict[ppid]) {

                    std::string new_path = current_path  + it;
                    processThreadsPaths[processIndex].path = new_path;
                    processThreadsPaths[processIndex].ppid = ppid;

                    pthread_create(&processThreads[processIndex], NULL, make_directory, &processThreadsPaths[processIndex]);

                    ///std::experimental::filesystem::create_directory(new_path);

                    new_path = new_path + "/";
                     processIndex --;
                    createPSTree(it, new_path);

                }
            else {
                // nu il gaseste deci e frunza -> trebuie sa scriem in el un fisier status

                filesThreadsPaths[fileIndex].path = current_path + "status.txt";
                filesThreadsPaths[fileIndex].ppid = ppid;
                filesThreadsPaths[fileIndex].status = childStatus[ppid];
                pthread_create(&filesThreads[fileIndex], NULL, make_directories, &filesThreadsPaths[fileIndex]);
                fileIndex --;

                // std::ofstream fout(current_path + "status.txt");
                // fout << "Statusul lui:\t" << ppid << "\n\n\tStatus:\n" ;  // scriem in acel fisier

                // fout << childStatus[ppid] <<"\n";

                // fout.close();
            }
        }

    };

    ProcessScraper ps;   /// obiect
}
