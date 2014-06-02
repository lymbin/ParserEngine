/*
 * Inventory.h
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#ifndef INVENTORY_H_
#define INVENTORY_H_

struct inventory_item
{
	int main_classification;
	int sub_classification;
	std::string itemName;

	unsigned int item_stack;
};


#endif /* INVENTORY_H_ */
