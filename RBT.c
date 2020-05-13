/*
 * @Author: verandert
 * @Date: 2020-05-12 16:36:53
 * @LastEditTime: 2020-05-13 14:32:28
 * @Description: Red-Black Tree
 */
#include<stdio.h>
#include<stdlib.h>

enum Color{red, black};
typedef float elemtype;

typedef struct RbtNode
{
    elemtype key;
    enum Color color;
    struct RbtNode *parent, *lchild, *rchild;
}RbtNode, *Rbtree;

void R_Rotate(Rbtree *HEAD, Rbtree *T);
void L_Rotate(Rbtree *HEAD, Rbtree *T);
int search(Rbtree T, elemtype key, Rbtree *result);
void InsertRbt(Rbtree *T, elemtype key, int found, Rbtree *result, Rbtree *Insert);
void INSERTRBT(Rbtree *T, elemtype key);

int main(){
    Rbtree T, result, Insert;
    T = result = Insert = NULL;
    int found;
    float a[] = {2.4, 3.4, 0.5, 4.6, 7.8, 3.3, 6.8, 1.2, 4.0, 3.2};
    for (int i = 0; i < 10; i++)
    {
        INSERTRBT(&T, a[i]);
    }
    printf("ddd");
    return 0;
}
/**
 * @Description: left balance
 */
void R_Rotate(Rbtree *HEAD, Rbtree *T){
    Rbtree lc = NULL;
    lc = (*T)->lchild;  
    (*T)->lchild = lc->rchild;
    if(lc->rchild) lc->rchild->parent = *T;
    lc->rchild = *T; 
    if((*T)->parent != *T) {
        lc->parent = (*T)->parent;
        if(lc->parent->key > lc->key) lc->parent->lchild = lc;
        else lc->parent->rchild = lc;
    }
    else {lc->parent = lc; *HEAD = lc;}
    lc->rchild->parent = lc;
    *T = lc;
}
/**
 * @Description: right balance
 */
void L_Rotate(Rbtree *HEAD, Rbtree *T){
    Rbtree rc = NULL;
    rc = (*T)->rchild;
    (*T)->rchild = rc->lchild;
    if(rc->lchild) rc->lchild->parent = *T;
    rc->lchild = *T;
    if((*T)->parent != (*T)) {
        rc->parent = (*T)->parent;
        if(rc->parent->key > rc->key) rc->parent->lchild = rc;
        else rc->parent->rchild = rc;
    }
    else {rc->parent  = rc; *HEAD = rc;}
    rc->lchild->parent = rc;
    *T = rc;
}
/**
 * @Description: if key was found in red-black tree, return 1; else return 0 and 
 * if the tree is empty, @result = NUll; else @result will be the inserted location's parent;
 */
int search(Rbtree T, elemtype key, Rbtree *result){
    if(!T) return 0;
    else if(T->key == key) {*result = T; return 1;}
    else if(T->key < key) {*result = T; return search(T->rchild, key, result);}
    else {*result = T; return search(T->lchild, key, result);}
}
/**
 * @Description: key was found in the tree, do nothing and return;
 * key was not found and the tree is empty, key will be the root of the tree;
 * key was not found and the tree is no empty, key will be the result's child;
 * * result's color is black, do nothing;
 * * result's color is red;
 * * * the color of result's bro is red, turn result's color and its bro's color to be black and turn the color of result's parent to be red; call function InsertRbt again;
 * * * the result has no bro or its bro's color is black, need to do something to keep balance;
 */
void InsertRbt(Rbtree *T, elemtype key, int found, Rbtree *result, Rbtree *Insert){
    Rbtree p, pp;
    if(!found){
        if(!(*Insert)){
            p = (Rbtree)malloc(sizeof(RbtNode));
            if(!p) exit(0);
            p->color = red; p->parent = p;
            p->lchild = p->rchild = NULL;
            p->key = key;
            if(!(*result)) {
                p->color = black;
                *T = p;
            } else if((*result)->key > key) {p->parent = *result; (*result)->lchild = p;}
            else {p->parent = *result; (*result)->rchild = p;}
        }
        if((*result) && (*result)->color == red){
            pp = (*result)->parent;
            if(pp->lchild && pp->rchild && pp->rchild->color == red && pp->lchild->color == red){
                pp->lchild->color = pp->rchild->color = black;
                pp->color = red;
                if(pp->parent == pp) pp->color = black;
                else if(pp->parent != pp && pp->parent->color == red){
                    InsertRbt(T, pp->key, 0, &(pp->parent), &pp);
                }
            } else
            {
                if((*result) == (*result)->parent->lchild){
                    if((*result)->key > key){
                        (*result)->color = black;
                        (*result)->parent->color = red;
                        p = (*result)->parent;
                        R_Rotate(T, &p);
                    } else
                    {
                        (*result)->rchild->color = black;
                        (*result)->parent->color = red;
                        p = *result;
                        L_Rotate(T, &p);
                        p = p->parent;
                        R_Rotate(T, &p);
                    }                  
                } else
                {
                    if((*result)->key < key){
                        (*result)->color = black;
                        (*result)->parent->color = red;
                        p = (*result)->parent;
                        L_Rotate(T, &p);
                    } else
                    {
                        (*result)->lchild->color = black;
                        (*result)->parent->color = red;
                        p = *result;
                        R_Rotate(T, &p);
                        p = p->parent;
                        L_Rotate(T, &p);
                    }
                }  
            }  
        }
    }
}
/**
 * @Description: function INSERTRBT
 */
void INSERTRBT(Rbtree *T, elemtype key){
    Rbtree result, insert;
    int found;
    result = insert = NULL;
    found = search(*T, key, &result);
    InsertRbt(T, key, found, &result, &insert);
}