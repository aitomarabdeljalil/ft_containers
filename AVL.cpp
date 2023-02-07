/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 12:53:27 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/07 18:10:10 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

//https://www.youtube.com/watch?v=YWqla0UX-38&list=PLdo5W4Nhv31bbKJzrsKfMpo_grxuLl8LU&index=61&ab_channel=Jenny%27sLecturesCSIT
struct Node {
	int key;
	Node *left, *right;
	Node (int _key)
	{
		this->key = _key;
		left = right = NULL;
	}
};

/*
	case 1: x < y < z
		x                    y
		 \					/ \
		  y        => 	   x   z     left rotation LR
		   \
		   	z
	same for x > y > z => right rotation RR
	case 2: x < z && z > y
		x				x
		 \				 \	
		  z		=>	 	  y   =>  case 1 LR;  RLR right left rotation
		 /				   \
		y					z
	same for z > x && y > x => use case 1 RR;  LRR left right rotation
*/

class AVL
{
public:
    Node *root;
public:
    AVL() : root(NULL) {};
    Node *  insert(Node *node, int key)
	{
		if (!node)
			return new Node(key);
		if (key < node->key)
			node->left = insert(node->left, key);
		if (key > node->key)
			node->right = insert(node->right, key);
		return node;
	};
    ~AVL() {};
};
