#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <math.h>
#include <utility>
#include "Data.hpp"
#include "Tree.hpp"
#include "GlobalTimer.hpp"

struct SA{
    std::unordered_map<std::string, BLOCK*> blockMap;
    std::vector<BLOCK*> blockList;
    std::vector<NET*> netList;
    int wireLength;
    int areaCost;
    int outline;
    bool areaMeet = false;
    int stage;
    const double deadSpaceRatio;

    SA(std::unordered_map<std::string, BLOCK*> blockMap, std::vector<BLOCK*> blockList, std::vector<NET*> netList, int outline, double deadSpaceRatio) 
        : blockMap(blockMap), blockList(blockList), netList(netList), wireLength(0), areaCost(0), outline(outline), areaMeet(false), stage(1), deadSpaceRatio(deadSpaceRatio) {}


    double calCost_2stage(){
        areaCost = 0;
        wireLength = 0;
        for(auto n : netList){
            wireLength += n->getHPWL();
        }
        for(auto b : blockList){
            if((b->x + b->getWidth() - outline) > 0 && (b->y + b->getHeight() - outline) > 0){
                areaCost += (b->x + b->getWidth() - outline) * (b->y + b->getHeight() - outline);
            }
            else if((b->x + b->getWidth() - outline) > 0 && (b->y + b->getHeight() - outline) <= 0){
                areaCost += (b->x + b->getWidth() - outline) * b->getHeight();
            } 
            else if((b->x + b->getWidth() - outline) <= 0 && (b->y + b->getHeight() - outline) > 0){
                areaCost += (b->y + b->getHeight() - outline) * b->getWidth();
            }
        }
        if(stage == 1 && areaCost == 0){
            areaMeet = true;
            //std::cout << "areaMeet!!\n" ;
            return areaCost;
        }
        if(stage == 2) return wireLength;
        else return areaCost;
    }

    double calCost(){
        areaCost = 0;
        wireLength = 0;
        for(auto n : netList){
            wireLength += n->getHPWL();
        }
        for(auto b : blockList){
            if((b->x + b->getWidth() - outline) > 0 && (b->y + b->getHeight() - outline) > 0){
                areaCost += (b->x + b->getWidth() - outline) * (b->y + b->getHeight() - outline);
            }
            else if((b->x + b->getWidth() - outline) > 0 && (b->y + b->getHeight() - outline) <= 0){
                areaCost += (b->x + b->getWidth() - outline) * b->getHeight();
            } 
            else if((b->x + b->getWidth() - outline) <= 0 && (b->y + b->getHeight() - outline) > 0){
                areaCost += (b->y + b->getHeight() - outline) * b->getWidth();
            }
        }
        
        //return areaCost*1000 + wireLength;
        return areaCost*10000 + wireLength;
    }

    double calBestCost(){
        areaCost = 0;
        wireLength = 0;
        for(auto n : netList){
            wireLength += n->getBestHPWL();
        }
        for(auto b : blockList){
            if((b->bestX + b->getBestWidth() - outline) > 0 && (b->bestY + b->getBestHeight() - outline) > 0){
                areaCost += (b->bestX + b->getBestWidth() - outline) * (b->bestY + b->getBestHeight() - outline);
            }
            else if((b->bestX + b->getBestWidth() - outline) > 0 && (b->bestY + b->getBestHeight() - outline) <= 0){
                areaCost += (b->bestX + b->getBestWidth() - outline) * b->getBestHeight();
            } 
            else if((b->bestX + b->getBestWidth() - outline) <= 0 && (b->bestY + b->getBestHeight() - outline) > 0){
                areaCost += (b->bestY + b->getBestHeight() - outline) * b->getBestWidth();
            }
        }
        
        return wireLength;
    }
    

    PERTURB* perturb(TREE* tree, int type){
        int a, b;
        a = rand() % tree->treeNodes.size();
        b = rand() % tree->treeNodes.size();
        while(a == b){
            b = rand() % tree->treeNodes.size();
        }
        if(type == 0){
            tree->swapNode(tree->treeNodes[a], tree->treeNodes[b]);
            tree->dfsContour();
            tree->getNodes();
            return new SWAPNODE(tree, tree->treeNodes[a], tree->treeNodes[b]);
        }
        else if(type == 1){
            tree->rotateNode(tree->treeNodes[a]);
            tree->dfsContour();
            tree->getNodes();
            return new ROTATENODE(tree, tree->treeNodes[a]);
        }
        else{
            tree->swapSubtree(tree->treeAddress[a], tree->treeAddress[b]);
            tree->dfsContour();
            tree->getNodes();
            return new SWAPSUBTREE(tree, tree->treeAddress[a], tree->treeAddress[b]);
        }
        
    }

    TREE* sa_2stage(double const &c, double const &r, double const &k, TREE* tree){
        int MT = 0, uphill = 0, reject = 0, N = k * blockList.size();

        double bestCost = 0, cost = 0;
        bestCost = cost = calCost_2stage();
        do{
            double T0 = 1000;
            do{
                MT = uphill = reject = 0;
                do{
                    int perturbType = rand() % 3;
                    PERTURB* nowPerturb = perturb(tree, perturbType);
                    ++MT;
                    double newCost = calCost_2stage();
                    double deltaCost = newCost - cost;
                    if(stage == 1 && areaCost == 0){
                        cost = newCost;
                        tree->writeBest();
                        bestCost = cost;
                        //std::cout << areaMeet << " bestCost = " << bestCost << "\n";
                        
                        stage = 2;
                        bestCost = calCost_2stage();
                        
                        //std::cout << "stage 2 : bestCost = " << bestCost << "\n";
                        return tree;
                    }
                    
                    else if((deltaCost < 0 || (double)rand() / RAND_MAX < exp(-deltaCost / T0)) && (stage == 1 || (stage == 2 && areaCost == 0)) ){
                        if(deltaCost > 0) ++uphill;
                        cost = newCost;
                        if(cost < bestCost){
                            tree->writeBest();
                            bestCost = cost;
                            //std::cout << areaMeet << " bestCost = " << bestCost << "\n";
                        } 
                    }
                    else{
                        ++reject;
                        nowPerturb->undoPerturb();
                    }
                    delete nowPerturb;
                    
                }while(uphill <= N && MT <= 2 * N);
                T0 = r*T0;
            }while(reject / MT < 0.95 && T0 >= c);
            //std::cout << "area not meet yet , start a new round\n";
        }while(stage == 1);
        
        
        return tree;
    }
    
    TREE* sa(double const &c, double const &r, double const &k, TREE* tree){
        int MT = 0, uphill = 0, reject = 0, N = k * blockList.size();

        double bestCost = 0, cost = 0;
        bestCost = cost = calCost();
        //add 186
        //do{
            double T0 = 1000;
            do{
                MT = uphill = reject = 0;
                do{
                    int perturbType = rand() % 3;
                    PERTURB* nowPerturb = perturb(tree, perturbType);
                    ++MT;
                    double newCost = calCost();
                    double deltaCost = newCost - cost;
                    if(deltaCost < 0 || (double)rand() / RAND_MAX < exp(-deltaCost / T0)){
                        if(deltaCost > 0) ++uphill;
                        cost = newCost;
                        if(cost < bestCost && areaCost == 0){
                            tree->writeBest();
                            bestCost = cost;
                            //std::cout << areaCost << " bestCost = " << bestCost << "\n";
                        } 
                    }
                    else{
                        ++reject;
                        nowPerturb->undoPerturb();
                    }
                    delete nowPerturb;
                    
                }while(uphill <= N && MT <= 2 * N);
                T0 = r*T0;
            }while(reject / MT < 0.95 && T0 >= c && !globaltimer::TLE());
        // add 215
        //}while(areaCost != 0 && !globaltimer::TLE());
        
        
        return tree;
    }
};