#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>

struct BLOCK{
    std::string name;
    int x;
    int y;
    int width;
    int height;
    bool rotate;
    int bestX;
    int bestY;
    int bestRotate;
    BLOCK(std::string name, int width, int height): name(name), x(0), y(0), width(width), height(height), rotate(false), bestX(0), bestY(0), bestRotate(false) {}
    BLOCK(BLOCK* block) : name(block->name), x(block->x), y(block->y), width(block->getWidth()), height(block->getHeight()), rotate(block->rotate),  bestX(0), bestY(0), bestRotate(false) {}

    int getWidth(){ return rotate ? height : width; }
    int getHeight(){ return rotate ? width : height; }
    int getPinX(){ return x + getWidth() / 2; }
    int getPinY(){ return y + getHeight() / 2; }
    int getBestWidth(){ return bestRotate ? height : width; }
    int getBestHeight(){ return bestRotate ? width : height; }
    int getBestPinX(){ return bestX + getBestWidth() / 2; }
    int getBestPinY(){ return bestY + getBestHeight() / 2; }
};

struct PIN{
    std::string name;
    int x;
    int y;
    PIN(std::string name, int x, int y): name(name), x(x), y(y) {}
};

struct NET{
    std::vector<PIN*> terminals;
    std::vector<BLOCK*> blocks;
    NET(std::vector<PIN*> terminals, std::vector<BLOCK*> blocks): terminals(terminals), blocks(blocks) {}

    int getHPWL(){
        int maxX = 0, maxY = 0;
        int minX = std::numeric_limits<int>::max();
        int minY = std::numeric_limits<int>::max();
        for(auto t : terminals){
            minX = std::min(minX, t->x);
            minY = std::min(minY, t->y);
            maxX = std::max(maxX, t->x);
            maxY = std::max(maxY, t->y);
        }
        for(auto b : blocks){
            minX = std::min(minX, b->getPinX());
            minY = std::min(minY, b->getPinY());
            maxX = std::max(maxX, b->getPinX());
            maxY = std::max(maxY, b->getPinY());
        }   
        return (maxX - minX) + (maxY - minY);
    }
    
    int getBestHPWL(){
        int maxX = 0, maxY = 0;
        int minX = std::numeric_limits<int>::max();
        int minY = std::numeric_limits<int>::max();
        for(auto t : terminals){
            minX = std::min(minX, t->x);
            minY = std::min(minY, t->y);
            maxX = std::max(maxX, t->x);
            maxY = std::max(maxY, t->y);
        }
        for(auto b : blocks){
            minX = std::min(minX, b->getBestPinX());
            minY = std::min(minY, b->getBestPinY());
            maxX = std::max(maxX, b->getBestPinX());
            maxY = std::max(maxY, b->getBestPinY());
        }   
        return (maxX - minX) + (maxY - minY);
    }

};

struct NODE{
    NODE* left;
    NODE* right;
    BLOCK* block;
    NODE(BLOCK* block) : left(nullptr), right(nullptr), block(block) {}    
    int getWidth(){ return block->rotate ? block->height : block->width; }
    int getHeight(){ return block->rotate ? block->width : block->height; }
    int getX() { return block->x; }
    int getY() { return block->y; }
    int getPinX(){ return block->x + getWidth() / 2; }
    int getPinY(){ return block->y + getHeight() / 2; }
    
};

struct CONTOURNODE{
    CONTOURNODE* next;
    CONTOURNODE* prev;
    BLOCK* block;
    CONTOURNODE() : next(nullptr), prev(nullptr), block(nullptr) {}
    CONTOURNODE(BLOCK* block) : next(nullptr), prev(nullptr), block(block) {}
    int getWidth(){ return block->rotate ? block->height : block->width; }
    int getHeight(){ return block->rotate ? block->width : block->height; }
    int getX() { return block->x; }
    int getY() { return block->y; }
    int getPinX(){ return block->x + getWidth() / 2; }
    int getPinY(){ return block->y + getHeight() / 2; }
};