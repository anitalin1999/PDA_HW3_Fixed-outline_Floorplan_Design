#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Data.hpp"

struct TREE{
    NODE* root;
    CONTOURNODE* head;
    std::vector<NODE*> treeNodes;
    std::vector<NODE**> treeAddress;

    NODE* initSol(int seed, std::vector<BLOCK*> blockList, double outline){
        std::vector<BLOCK*> BST = blockList;
        std::shuffle(BST.begin(), BST.end(), std::default_random_engine(seed));
        int rowWidth = 0;
        root = new NODE(BST[0]);
        rowWidth += root->getWidth();
        NODE* curNode = root;
        NODE* curLeft = root;
        for(int i=1; i<BST.size(); ++i){
            BLOCK* curBlock = BST[i];
            rowWidth += curBlock->getWidth();
            
            if(rowWidth > outline){
                curNode->right = new NODE(BST[i]);
                curNode = curNode->right;
                curLeft = curNode;
                rowWidth = BST[i]->getWidth();
            } 
            else{
                curLeft->left = new NODE(BST[i]);
                curLeft = curLeft->left;
            }
        }
        return root;
    }

    
    void dfsContour(NODE* node, NODE* parent, int x, char type){
        if(node == nullptr) return;
        else if(parent == nullptr){
            node->block->x = 0;
            checkY(node->getX(), node);
        }
        //std::cout << node->block->name << " parent x = " << x << std::endl;
        else{
            if(type == 'R'){
                //std::cout << "left child" << std::endl;
                node->block->x = x + parent->getWidth();
                checkY(node->getX(), node);
            }
            else{
                //std::cout << "right child" << std::endl;
                node->block->x = x;
                checkY(node->getX(), node);
            }
        }
        
        dfsContour(node->left, node, node->getX(), 'R');
        dfsContour(node->right, node, node->getX(), 'U');

        return;
    }

    void dfsContour(){
        CONTOURNODE* temp = new CONTOURNODE();
        while(head->next != nullptr){
            temp = head;
            head = head->next;
            free(temp);
        }
        
        //std::cout << " all contour deleted" << "\n";
        dfsContour(root, nullptr, 0, 'U');
        return;
    }

    void getNodes(NODE* node){
        if(node == nullptr) return;
        else{
            treeNodes.emplace_back(node);
            treeAddress.emplace_back(&node);
            getNodes(node->left);
            getNodes(node->right);
        }
        return;
    }
    
    void getNodes(){
        treeNodes.clear();
        treeAddress.clear();
        getNodes(root);
        return;
    }

    void checkY(int x, NODE* node){
        int pos = 1;
        node->block->y = 0;
        CONTOURNODE* ptr = head;
        if(ptr->next == nullptr){
            addContour(node->block, pos);
            return;
        }
        else{
            while(ptr->next != nullptr){
                if(ptr->next->getX() + ptr->next->getWidth() > x){
                    if(ptr->next->getY() + ptr->next->getHeight() > node->getY()) node->block->y = ptr->next->getY() + ptr->next->getHeight(); 
                    break;
                }
                else{
                    ptr = ptr->next;
                    ++pos;
                }
            }
            if(ptr->next == nullptr){
                addContour(node->block, pos);
                return;
            }
            ptr = ptr->next;

            std::vector<CONTOURNODE*> contourOverlap;
            while(ptr->next != nullptr){
                if(ptr->getX() + ptr->getWidth() < x + node->getWidth()){
                    contourOverlap.emplace_back(ptr);
                    if(node->block->y < ptr->getY() + ptr->getHeight()) node->block->y = ptr->getY() + ptr->getHeight();
                    ptr = ptr->next;
                }
                else{
                    break;
                }
            }
            if(ptr != nullptr){
                if(node->block->y < ptr->getY() + ptr->getHeight()) node->block->y = ptr->getY() + ptr->getHeight();
            }
            replaceContour(node->block, pos, contourOverlap);
        }
        return;
    }


    void addContour(BLOCK* block, int pos){
        //std::cout << "add contour by " << block->name << "| pos= " << pos << std::endl;
        CONTOURNODE* newP = new CONTOURNODE(block);;    // the block node to be insert
        CONTOURNODE* temp = head;       // insertion position's prev
        CONTOURNODE* temp2 = nullptr;   // insertion position's next
        
        while(pos != 1){
            temp = temp->next;
            --pos;
        }  
        
        if(temp->next == nullptr){
            temp->next = newP;
            newP->prev = temp;
        }
        else{
            temp2 = temp->next;
            temp->next = newP;
            temp2->prev = newP;
            newP->next = temp2;
            newP->prev = temp;
        }
        return;
    }

    void replaceContour(BLOCK* block, int pos, std::vector<CONTOURNODE*> contourOverlap){

        if(contourOverlap.empty()){
            addContour(block, pos);
            return;
        } 
        //std::cout << "replace by " << block->name << " | pos = " << pos << std::endl;
        CONTOURNODE* newP = new CONTOURNODE(block);
        if(contourOverlap.back()->next != nullptr){
            /*
            std::cout << "contour to be replaced: ";    
            for(auto t : contourOverlap){
                std::cout << t->block->name << " ";
            }
            std::cout << std::endl;
            */
            CONTOURNODE* temp = contourOverlap.front()->prev;
            CONTOURNODE* temp2 = contourOverlap.back()->next;

            temp->next = newP; 
            temp2->prev = newP; 
            newP->next = temp2;
            newP->prev = temp;
        }
        else{
            /*
            std::cout << "contour to be replaced: ";
            for(auto t : contourOverlap){
                std::cout << t->block->name << " ";
            }
            std::cout << std::endl;
            */
            contourOverlap.front()->prev->next = newP;
            newP->next = nullptr;
            newP->prev = contourOverlap.front()->prev;
        }
        

        for(auto c : contourOverlap){
            free(c);
        }
        return;
    }

    void printContour(){
        CONTOURNODE* ptr = head;
        //std::cout << "now Contour : ";
        while(ptr->next != nullptr){
            //std::cout << ptr->next->block->name << " ";
            ptr = ptr->next;
        }
        //std::cout << "\n\n";
        return;
    }

    void printLocation(CONTOURNODE* head, NODE* node, NODE* parent, int x, char type){
        if(node == nullptr) return;
        //std::cout << node->block->name << " | " << node->getX() << " " << node->getY() << std::endl; 
        printLocation(head, node->left, node, node->getX(), 'R');
        printLocation(head, node->right, node, node->getX(), 'U');

        return;
    }


    void swapNode(NODE* a, NODE* b){
        std::swap(a->block, b->block);
        getNodes();
        return;
    }
    
    void rotateNode(NODE* a){
        a->block->rotate = !a->block->rotate;
        getNodes();
        return;
    }

    void swapSubtree(NODE** a, NODE** b){
        std::swap(*a, *b); 
        getNodes();
        return;
    }

    void writeBest(){
        for(auto n : treeNodes){
            n->block->bestX = n->getX();
            n->block->bestY = n->getY();
            n->block->bestRotate = n->block->rotate;
        }
        return;
    }

};

struct PERTURB{
    virtual void undoPerturb() = 0;
    virtual ~PERTURB() {}
};

struct SWAPNODE : public PERTURB{
    TREE* tree;
    NODE* a;
    NODE* b;
    SWAPNODE(TREE* tree, NODE* a, NODE* b) : tree(tree), a(a), b(b) {}
    void undoPerturb(){ tree->swapNode(a, b); }
};

struct ROTATENODE : public PERTURB{
    TREE* tree;
    NODE* a;
    ROTATENODE(TREE* tree, NODE* a) : tree(tree), a(a) {}
    void undoPerturb() { tree->rotateNode(a); }
};

struct SWAPSUBTREE : public PERTURB{
    TREE* tree;
    NODE** a;
    NODE** b;
    SWAPSUBTREE(TREE* tree, NODE** a, NODE** b) : tree(tree), a(a), b(b) {}
    void undoPerturb() { tree->swapSubtree(a, b); }
};
