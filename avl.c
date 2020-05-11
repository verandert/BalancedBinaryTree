/*
 * @Author: verandert
 * @Date: 2020-05-11 17:04:57
 * @LastEditTime: 2020-05-11 21:14:41
 * @Description: Balanced Binary Tree 
 */
#include <stdio.h>
#include <stdlib.h>

#define LH 1
#define RH -1
#define EH 0

#define EQ(a, b) a == b
#define LT(a, b) a < b

enum bool {false, true};
typedef float elemtype;
typedef struct BSTNode
{
    elemtype data;
    int bf;
    struct BSTNode *lchild, *rchild;
}BSTNode, *BSTree;

void R_Rotate(BSTree *T);
void L_Rotate(BSTree *T);
void left_balanced(BSTree *T);
void right_balanced(BSTree *T);
void InsertAvl(BSTree *T, elemtype key, enum bool *taller);

int main(){
    BSTree T = NULL;
    enum bool taller;
    float a[5] = {1.3, 2.4, 0.9, 4.5, 3.5};
    for (int i = 0; i < 5; i++)
    {
        InsertAvl(&T, a[i], &taller);
    }
    printf("11");
    return 0;
}
/**
 * @Description: 右旋
 */
void R_Rotate(BSTree *T){
    BSTree lc;
    lc = (*T)->lchild;
    (*T)->lchild = lc->rchild;
    lc->rchild = *T;
    *T = lc;
}
/**
 * @Description: 左旋
 */
void L_Rotate(BSTree *T){
    BSTree rc;
    rc = (*T)->rchild;
    (*T)->rchild = rc->lchild;
    rc->lchild = *T;
    *T = rc;
}
/**
 * @Description: 左子树平衡处理
 */
void left_balanced(BSTree *T){
    BSTree lc, rd;
    lc = (*T)->lchild;
    switch (lc->bf)
    {
    case LH://单向右旋
        (*T)->bf = EH; lc->bf = EH;
        R_Rotate(T);
        break;
    case RH://双向旋转先左后右
        rd = lc->rchild;
        switch (rd->bf)
        {
        case LH:
            (*T)->bf = RH; lc->bf = EH;
            break;
        case EH:
            (*T)->bf = lc->bf = EH;
            break;
        case RH:
            (*T)->bf = EH; lc->bf = LH;
            break;
        }
        rd->bf = EH;
        L_Rotate(&((*T)->lchild));
        R_Rotate(T);
    }
}
/**
 * @Description: 右子树平衡处理
 */
void right_balanced(BSTree *T){
    BSTree rc, ld;
    rc = (*T)->rchild;
    switch (rc->bf)
    {
    case RH://单向左旋处理
        (*T)->bf = rc->bf = EH;
        L_Rotate(T);
        break;
    case LH://双向旋转先右后左
        ld = rc->lchild;
        switch (ld->bf)
        {
        case EH:
            (*T)->bf = rc->bf = EH;
            break;
        case LH:
            (*T)->bf = EH; rc->bf = RH;
            break;
        case RH:
            (*T)->bf = LH; rc->bf = EH;
            break;
        }
        ld->bf = EH;
        R_Rotate(&((*T)->rchild));
        L_Rotate(T);
    }
}
/**
 * @Description: 动态查找--平衡二叉排序树实现
 */
void InsertAvl(BSTree *T, elemtype key, enum bool *taller){
    if(!(*T)){
        if(((*T) = (BSTree)malloc(sizeof(BSTNode)))){
            (*T)->rchild = (*T)->lchild = NULL;
            (*T)->data = key; (*T)->bf = EH;
            *taller = true;
        }
    } else
    {
        if(EQ(key, (*T)->data)){
            *taller = false;
        }else if(LT(key, (*T)->data)){
            InsertAvl(&((*T)->lchild), key, taller);
            if(*taller == true){
                switch ((*T)->bf)
                {
                case EH:
                    (*T)->bf = LH; *taller = true;
                    break;
                case LH:
                    left_balanced(T); *taller = false;
                case RH:
                    (*T)->bf = EH; *taller = false;
                }
            }
        }else
        {
            InsertAvl(&((*T)->rchild), key, taller);
            if (*taller == true)
            {
                switch ((*T)->bf)
                {
                case EH:
                    (*T)->bf = RH; *taller = true;
                    break;
                case RH:
                    right_balanced(T); *taller = false;
                    break;
                case LH:
                    (*T)->bf = EH; *taller = true;
                    break;
                }
            }
            
        }
        
    }
    
}
