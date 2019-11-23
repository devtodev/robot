/*
 * dashboard.c
 *
 *  Created on: Sep 11, 2019
 *      Author: Carlos Miguens
 */

#include "dashboard.h"
#include "stdlib.h"

void setRule(Rule *rules, Conditions *condition, int score, Actions *learning)
{
	rules->condition = condition;
	rules->score = score;
	rules->learning = learning;
	rules->next = NULL;
}

Rule *removerule(Rule *rules, Rule *rule)
{
	if (rules == rule)
	{
		return rule->next;
	}
	while ((rules->next != rule) && (rules->next != NULL))
	{
		rules = rules->next;
	}
	if (rules->next == rule)
	{
		rules->next = rules->next->next;
	}
	return rules;
}

void addRule(Rule *rules, Conditions *condition, int score, Actions *learning)
{
	while (rules->next != NULL)
	{
		rules = rules->next;
	}
	rules->next = malloc(sizeof(Rule));
	rules = rules->next;
	setRule(rules, condition, score, learning);
}

void checkrule(Rule *rule)
{
	// verify condition
	int iCondition = 0;
	while (rule->condition != NULL)
	{
		//Conditions *condition = rule->condition;
		//TODO:implement
		iCondition++;

	}
}
