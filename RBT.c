/*
 * @Author: verandert
 * @Date: 2020-05-12 16:36:53
 * @LastEditTime: 2020-05-17 13:56:06
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
void DelRbt(Rbtree *T, elemtype key);
void FixRbt(Rbtree *T, Rbtree index, int flag);
Rbtree FindSubstitute(Rbtree *T, elemtype key);

int main(){
    Rbtree T, result, Insert;
    T = result = Insert = NULL;
    int found;
    float a[] = {2.4, 3.4, 0.5, 4.6, 7.8, 3.3, 6.8, 1.2, 4.0, 3.2};
    for (int i = 0; i < 10; i++)
    {
        INSERTRBT(&T, i);
    }
    DelRbt(&T, 3);
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
/**
 * @Description: 删除key节点转化为删除子叶节点(非NULL)
 */
Rbtree FindSubstitute(Rbtree *T, elemtype key){
    Rbtree index, p;
    index = p = NULL;
    if(search(*T, key, &index)){
        while (index->lchild || index->rchild)
        {
            if(index->lchild && !(index->rchild)){
                index->key = index->lchild->key;
                index = index->lchild;
            }else
            {
                p = index->rchild;
                while (p->lchild)
                {
                    p = p->lchild;
                }
                index->key = p->key;
                index = p;
            } 
        }
    }
    return index;
}
/**
 * @Description: 修复删除子叶节点后被破坏的红黑树性质5
 */
void FixRbt(Rbtree *T, Rbtree index, int flag){
    Rbtree p = NULL;
    if(index->color == red){
        if(!flag){
            if(index == index->parent->lchild) index->parent->lchild = NULL;
            else index->parent->rchild = NULL;
            free(index);
            index = NULL;
        }else index->color = black;
    } else
    {
        if(index->parent == index){
            if(!flag){
                free(index);
                *T = index = NULL;
            }
        } else
        {
            if(index->parent->lchild == index){
                p = index->parent->rchild;
                if(p->color == red){
                    p->color = black;
                    index->parent->color = red;
                    L_Rotate(T, &(index->parent));
                } else
                {
                    if(p->rchild && p->rchild->color == red){
                        p->color = index->parent->color;
                        index->parent->color = black;
                        p->rchild->color = black;
                        L_Rotate(T, &(index->parent));
                    } else if(p->lchild && p->lchild->color == red){
                        p->lchild->color = index->parent->color;
                        index->parent->color = black;
                        R_Rotate(T, &p);
                        L_Rotate(T, &(index->parent));
                    } else
                    {
                        index->parent->rchild->color = red;
                        FixRbt(T, index->parent, 1);
                    }
                    if(!flag){
                        index->parent->lchild = NULL;
                        free(index);
                        index = NULL;
                    }
                }
                
            } else
            {
                p = index->parent->lchild;
                if(p->color == red){
                    p->color = black;
                    index->parent->color = red;
                    R_Rotate(T, &(index->parent));
                } else
                {
                    if(p->lchild && p->lchild->color == red){
                        p->color = index->parent->color;
                        index->parent->color = black;
                        p->lchild->color = black;
                        R_Rotate(T, &(index->parent));
                    } else if(p->rchild && p->rchild->color == red){
                        p->rchild->color = index->parent->color;
                        index->parent->color = black;
                        L_Rotate(T, &p);
                        R_Rotate(T, &(index->parent));
                    } else
                    {
                        index->parent->lchild->color = red;
                        FixRbt(T, index->parent, 1);
                    }
                    if(!flag){
                        index->parent->rchild = NULL;
                        free(index);
                        index = NULL;
                    }
                }
            }
            
        }
    }
}
/**
 * @Description: 删除红黑树中的节点
 */
void DelRbt(Rbtree *T, elemtype key){
    Rbtree index;
    index = FindSubstitute(T, key);
    if(index) FixRbt(T, index, 0);
}