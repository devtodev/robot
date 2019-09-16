/*
 * dashboard.c
 *
 *  Created on: Sep 11, 2019
 *      Author: Carlos Miguens
 */

#include "dashboard.h"
#include "stdlib.h"

void initPoint(Point *point, Conditions *condition, int score, Actions *learning)
{
	point->condition = condition;
	point->score = score;
	point->learning = learning;
	point->next = NULL;
}

Point *removePoint(Point *root, Point *point)
{
	if (root == point)
	{
		return point->next;
	}
	while ((root->next != point) && (root->next != NULL))
	{
		root = root->next;
	}
	if (root->next == point)
	{
		root->next = root->next->next;
	}
	return root;
}

void addPoint(Point *point, Conditions *condition, int score, Actions *learning)
{
	while (point->next != NULL)
	{
		point = point->next;
	}
	point->next = malloc(sizeof(Point));
	point = point->next;
	initPoint(point, condition, score, learning);
}

void checkPoint(Point *point)
{
	// verify condition
	int iCondition = 0;
	while (point->condition != NULL)
	{
		Conditions *condition = point->condition;
		iCondition++;

	}
}
