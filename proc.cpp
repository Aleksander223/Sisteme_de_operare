/*
  Gets the processes from /proc and hierarchizes them using a general tree
*/

#include "tree.cpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace proc {
    class ProcessScraper {
    private:
        const std::string path = "/proc";

        std::vector< unsigned int > procList;
    public:
        void scrapeProcesses() {
            for (const auto & entry : std::filesystem::directory_iterator(path)) {
                std::string fileName = entry.path().filename();
                if (fileName[0] >= '0' && fileName[0] <= '9') {
                    int processID = std::stoi(fileName);

                    procList.push_back(processID);
                }
            }
        }

        void printProcesses() {
            for (auto i: procList) {
                std::cout << i << std::endl;
            }
        }

        ProcessScraper() {
            scrapeProcesses();
        }
    };
}
