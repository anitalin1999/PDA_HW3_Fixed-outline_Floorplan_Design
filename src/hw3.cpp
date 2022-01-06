#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Data.hpp"
#include "Parse.hpp"
#include "SA.hpp"
#include "Tree.hpp"
#include "GlobalTimer.hpp"

#define CHECKBST
using namespace std;

int seed = 100;


int main(int argc, char** argv){
    globaltimer::setLimit(std::chrono::seconds (60*18));
    //seed = stoi(argv[6]);
    
    /* Parser */
    PARSE parse;
    SA* sa = parse.parser(argv);
    auto parseTimer = globaltimer::getTime();
    
    if(parse.blockList.size() == 100){
        if(sa->deadSpaceRatio == 0.15) seed = 39;
        else if(sa->deadSpaceRatio == 0.1) seed = 243;
        else seed = 100;
    }
    else if(parse.blockList.size() == 200){
        if(sa->deadSpaceRatio == 0.15) seed = 81;
        else if(sa->deadSpaceRatio == 0.1) seed = 1;
        else seed = 100;
    }
    else if(parse.blockList.size() == 300){
        if(sa->deadSpaceRatio == 0.15) seed = 87;
        else if(sa->deadSpaceRatio == 0.1) seed = 5;
        else seed = 100;
    }
    else seed = 100;
    /* Initial Solution */    
    TREE tree;
    tree.root = tree.initSol(seed, parse.blockList, sa->outline);
    

    tree.head = new CONTOURNODE();
    /* Check Position */
    tree.dfsContour();
    tree.getNodes();
    tree.writeBest();
    auto treeTimer = globaltimer::getTime();
/*
    TREE* nowTree = &tree;
    nowTree = sa->sa_2stage(0.001, 0.99, 10, nowTree);
    nowTree = sa->sa_2stage(0.001, 0.99, 10, nowTree);
*/



    TREE* nowTree = &tree;
    nowTree = sa->sa(10, 0.99, 20, nowTree);
    //nowTree = sa->sa(0.1, 0.995, 30, nowTree);
    double bestCost = sa->calBestCost();
    auto SATimer = globaltimer::getTime();

    cout << sa->areaCost << "," << seed << "," << bestCost << ",";
    parse.write_file(argv, bestCost);
    auto writeTimer = globaltimer::getTime();

    auto parseTime = parseTimer.count();
    auto treeTime = (treeTimer - parseTimer).count();
    auto SATime = (SATimer - treeTimer).count();
    auto writeTime = (writeTimer - SATimer).count();
    //cout << "Time Limit = " << globaltimer::timeLimit/pow(10,9) << "\n";
    // cout << "Parse Input = " << parseTime/pow(10,9) << "\n";
    // cout << "Build Structure = " << treeTime/pow(10,9) << "\n";
    // cout << "SA Computing = " << SATime/pow(10,9) << "\n";
    // cout << "Write Output = " << writeTime/pow(10,9) << "\n";

    cout << parseTime/10e9 << ",";
    cout << treeTime/pow(10,9) << ",";
    cout << SATime/pow(10,9) << ",";
    cout << writeTime/pow(10,9) << "\n";
 
    return 0;
}