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

namespace proc {

    struct Parameters{
	    std::string path = " ";
        std::string ppid =" ";
    };

    void* make_directory(void *argv){
        //std::cout<<"\nIn thread directory\n";
        Parameters *aux = (Parameters*)argv;
        std::experimental::filesystem::path p(aux->path);

        while(!std::experimental::filesystem::exists(p.parent_path())){
            //std::cout<<"Blocat\n";
        };

        std::experimental::filesystem::create_directory(aux->path);
        return (void*)true;
    }

    void* make_directories(void *argv){
        ///std::cout<<"\nIn thread directories\n";
        Parameters *aux = (Parameters*)argv;

        std::experimental::filesystem::path p(aux->path);

        while(!std::experimental::filesystem::exists(p.parent_path())){
            ///std::cout<<"Blocat iar\n";
        };

        std::ofstream fout(aux->path);
        fout << "Statusul lui:\t" << aux->ppid << "\n\n\tStatus:\n" ;  // scriem in acel fisier
        std::ifstream status("/proc/" + aux->ppid + "/status");
        std::string line;
        while(std::getline(status, line))
                fout<<line<<"\n";
        status.close();

        fout << "\n\n\tLimits:\n";
        std::ifstream limits("/proc/" + aux->ppid + "/limits");
        while(std::getline(limits, line))
                fout<<line<<"\n";
        limits.close();

        fout << "\n\n\tStat:\n";
        std::ifstream stat("/proc/" + aux->ppid + "/stat");
        while(std::getline(stat, line))
                fout<<line<<"\n";
        stat.close();

        fout << "\n\n\tSched:\n";
        std::ifstream sched("/proc/" + aux->ppid + "/sched");
        while(std::getline(sched, line))
                fout<<line<<"\n";
        sched.close();

        fout.close();

        return (void*)true;
    }

    class ProcessScraper {
    private:
        const std::string path = "/proc";
        int processNo = 1, filesNo;
        pthread_t *processThreads, *filesThreads;
        Parameters *filesThreadsPaths;
        Parameters *processThreadsPaths;
        std::map<std::string, std::vector<int>> parentDict;// un PPid -> vector de Pid asociati lui
        std::string getParentID (int pid) {
            std::string ppid; // PPid se afla in /proc/pid/status la randul ce incepe cu PPid -> linia 7

            // status file
            std::string statPath = "";
            statPath += "/proc/" + std::to_string(pid) + "/status";

            std::ifstream fin(statPath);
            std::string line;
            for(unsigned i=0; i<7; i++)
                std::getline(fin, line);
            fin.close();
            ppid = line.substr(line.find("\t") + 1);
            return ppid;
        }
    public:
        ProcessScraper() {
            scrapeProcesses();

            std::cout<<"Nr of procc "<<processNo<<"\n";

            processThreads = new pthread_t[60*processNo];   /// ar trebui facut un map, vectorul asta e prea mare pt a cupride id urile si sunt doar vreo 250 in medie
            processThreadsPaths = new struct Parameters[100*processNo];

            filesNo = processNo;
            filesThreads = new pthread_t[60*processNo];
            filesThreadsPaths = new struct Parameters[100*processNo];
        }

        void scrapeProcesses() {
            for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
                std::string fileName = entry.path().filename();
                if (fileName[0] > '0' && fileName[0] <= '9') {
                    int processID = std::stoi(fileName);
                    parentDict[getParentID(processID)].push_back(processID);
                    processNo++;
                }
            }
        }

        int printProcesses() {
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

            for(unsigned i = 1; i <= processNo; i++){
                void *rez;

                if(pthread_join(processThreads[i], &rez)){
                    perror(NULL);
                    return errno;
                }

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

                    std::string new_path = current_path  + std::to_string(it);
                    processThreadsPaths[it].path = new_path;
                    processThreadsPaths[it].ppid = ppid;

                    pthread_create(&processThreads[it], NULL, make_directory, &processThreadsPaths[it]);

                    new_path = new_path + "/";

                    //std::experimental::filesystem::create_directory(new_path);
                    createPSTree(std::to_string(it), new_path);

                }
            else {
                // nu il gaseste deci e frunza
                // daca e frunza trebuie sa scriem in el un fisier status

                filesThreadsPaths[std::stoi(ppid)].path = current_path + "status.txt";
                filesThreadsPaths[std::stoi(ppid)].ppid = ppid;

                pthread_create(&filesThreads[std::stoi(ppid)], NULL, make_directories, &filesThreadsPaths[std::stoi(ppid)]);
 
                // std::ofstream fout(current_path + "status.txt");
                // fout << "Statusul lui:\t" << ppid << "\n\n\tStatus:\n" ;  // scriem in acel fisier
                // std::ifstream status("/proc/" + ppid + "/status");
                // std::string line;
                // while(std::getline(status, line))
                //         fout<<line<<"\n";
                // status.close();

                // fout << "\n\n\tLimits:\n";
                // std::ifstream limits("/proc/" + ppid + "/limits");
                // while(std::getline(limits, line))
                //         fout<<line<<"\n";
                // limits.close();

                // fout << "\n\n\tStat:\n";
                // std::ifstream stat("/proc/" + ppid + "/stat");
                // while(std::getline(stat, line))
                //         fout<<line<<"\n";
                // stat.close();

                // fout << "\n\n\tSched:\n";
                // std::ifstream sched("/proc/" + ppid + "/sched");
                // while(std::getline(sched, line))
                //         fout<<line<<"\n";
                // sched.close();

                // fout.close();
            }
        }

    };
}
