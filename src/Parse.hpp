#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Data.hpp"
#include "SA.hpp"

struct PARSE{
    std::unordered_map<std::string, BLOCK*> blockMap;
    std::unordered_map<std::string, PIN*> pinMap;
    std::vector<BLOCK*> blockList;
    std::vector<NET*> netList;
    double totalBlockArea = 0;

    void read_blockFile(std::string const &blockFile){
        std::ifstream fin(blockFile);
        std::string s;
        while(getline(fin, s)){
            if(s.empty()) continue;
            std::stringstream ss(s);
            std::string buff, name;
            int x[4], y[4];

            ss >> name;
            if(name == "NumHardRectilinearBlocks" || name == "NumTerminals") continue;

            ss >> buff;
            if(buff != "hardrectilinear") continue;
            std::string blockInfo;
            std::getline(ss, blockInfo);
            sscanf(blockInfo.c_str(), " 4 (%d, %d) (%d, %d) (%d, %d) (%d, %d)", x, y, x+1, y+1, x+2, y+2, x+3, y+3);
            int width = *std::max_element(x, x+4) - *std::min_element(x, x+4);
            int height = *std::max_element(y, y+4) - *std::min_element(y, y+4);
            totalBlockArea += width * height;
            // create BLOCK
            struct BLOCK *block = new BLOCK(name, width, height);
            blockMap[name] = block;
            blockList.emplace_back(block);
            
        }
    #ifdef CHECKBLOCK
        for(auto b : blockMap){
            std::cout << b.first << " | " << b.second->getWidth() << " " << b.second->getHeight() << "\n";
        }
    #endif 
        return;
    }

    void read_pinFile(std::string const &pinFile){
        std::ifstream fin(pinFile);
        std::string s;
        while(getline(fin, s)){
            std::stringstream ss(s);
            std::string name;
            int x, y;
            ss >> name >> x >> y;
            struct PIN *pin = new PIN(name, x, y);
            pinMap[name] = pin;
        }
    #ifdef CHECKPIN
        for(auto p : pinMap){
            std::cout << p.first << " | " << p.second->x << " " << p.second->y << "\n";
        }
    #endif
        return;
    }

    void read_netFile(std::string const &netFile){
        std::ifstream fin(netFile);
        std::string s;
        while(std::getline(fin, s)){
            std::stringstream ss(s);
            std::string buff;
            ss >> buff;
            if(buff == "NumNets" || buff == "NumPins : 1873") continue;
            if(buff == "NetDegree"){
                int degree;
                std::vector<PIN*> terminals;
                std::vector<BLOCK*> blocks;
                ss >> buff >> degree;
                for(int i=0; i<degree; ++i){
                    std::getline(fin, buff);
                    if(buff[0] == 'p') terminals.emplace_back(pinMap[buff]);
                    else blocks.emplace_back(blockMap[buff]);
                }
                struct NET* net = new NET(terminals, blocks);
                netList.emplace_back(net);
            }
        }
    #ifdef CHECKNET
        for(auto n : netList){
            for(auto nt : n->terminals){
                std::cout << nt->name << " ";
            }
            for(auto nb : n->blocks){
                std::cout << nb->name << " ";
            }
            std::cout << "\n";
        }

        for(auto n : netList){
            std::cout << ngetHPWL() << "\n";
        }
    #endif

        return;
    }

    SA* parser(char** argv){
        read_blockFile(argv[1]);
        read_pinFile(argv[3]);
        read_netFile(argv[2]);
        double deadSpaceRatio = std::stod(argv[5]);
        int outline = floor(sqrt(totalBlockArea * (1 + deadSpaceRatio)));
        //std::cout << outline << "\n";
        return new SA(blockMap, blockList, netList, outline, deadSpaceRatio);
    }

    void write_file(char** argv, int wireLength){
        std::ofstream fout(argv[4]);
        fout << "Wirelength " << wireLength << "\n";
        fout << "Blocks" << "\n";
        for(auto b : blockList){
            fout << b->name << " " << b->bestX << " " << b->bestY << " " << b->bestRotate << "\n";
        }
        return;
    }

};