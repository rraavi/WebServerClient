/*
 * stacklist.h
 *
 *  Created on: Oct 6, 2015
 *      Author: Raghu
 */

#ifndef STACKLIST_H_
#define STACKLIST_H_
#include <iostream>
#include<iostream>
#include<cstdlib>
#include <errno.h>
#include <string.h>
using namespace std;

struct node
{
	int info;
    struct node *link;
}*top;

class stack_list
{
	public:
	node *push(node *, int);
	node *pop(node *);
	void traverse(node *);
	stack_list()
	{
		top = NULL;
	}
};

#endif /* STACKLIST_H_ */
