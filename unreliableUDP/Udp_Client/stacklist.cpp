/*
 * stacklist.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: Raghu
 */

#include "stacklist.h"


//int main()
//{
//        int choice, item;
//        stack_list sl;
//                top = sl.push(top, item);
//                top = sl.pop(top);
//                sl.traverse(top);
//        return 0;
//    }
    /*
     * Push Element into the Stack
     */
    node *stack_list::push(node *top, int item)
    {
        node *tmp;
        tmp = new (struct node);
        tmp->info = item;
        tmp->link = top;
        top = tmp;
        return top;
    }
    /*
     * Pop Element from the Stack
     */
    node *stack_list::pop(node *top)
    {
        node *tmp;
        if (top == NULL)
            cout<<"Stack is Empty"<<endl;
        else
        {
            tmp = top;
            cout<<"Element Popped: "<<tmp->info<<endl;
            top = top->link;
            free(tmp);
        }
        return top;
    }
    /*

     * Traversing the Stack

     */
//    void stack_list::traverse(node *top)
//    {
//        node *ptr;
//        ptr = top;
//        if (top == NULL)
//            cout<<"Stack is empty"<<endl;
//        else
//        {
//            cout<<"Stack elements :"<<endl;
//            while (ptr != NULL)
//            {
//                cout<<ptr->info<<endl;
//                ptr = ptr->link;
//            }
//        }


