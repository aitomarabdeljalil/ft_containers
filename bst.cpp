/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 11:20:23 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/07 12:52:16 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

struct Node {
	int key;
	Node *left, *right;
	Node (int _key)
	{
		this->key = _key;
		left = right = NULL;
	}
};

class bst
{
public:
	Node *root;
public:
	bst() : root() {};
	Node *  insert(Node *node, int key)
	{
		if (!node)
			return new Node(key);
		if (key < node->key)
			node->left = insert(node->left, key);
		if (key > node->key)
			node->right = insert(node->right, key);
		return node;
	}
	Node*	findMin(Node *node)
	{
		if (!node->left)
			return node;
		return findMin(node->left);
	}	
	Node*	deleteNode(Node *node, int key)
	{
		Node	*tmp;
	
		if (!node)
			return node;
		if (key < node->key)
			node->left = deleteNode(node->left, key);
		else if (key > node->key)
			node->right = deleteNode(node->right, key);
		else {
			if (!node->left) {
				tmp = node->right;
				delete node;
				return tmp;
			} else if (!node->right) {
				tmp = node->left;
				delete node;
				return tmp;
			}
			tmp = findMin(node->right);
			node->key = tmp->key;
			node->right = deleteNode(node->right, tmp->key);
		}
		return node;
	}
	Node*	Successor(Node* node, int key)
	{
		Node*	tmp = NULL;
		
		while (node)
		{
			if (key >= node->key)
				node = node->right;
			else {
				tmp = node;
				node = node->left;
			}
		}
		return tmp;
	}
	Node*	Predecessor(Node* node, int key)
	{
		Node* tmp = NULL;
		
		while (node)
		{
			if (key <= node->key)
				node = node->left;
			else {
				tmp = node;
				node = node->right;
			}
		}
		return tmp;
	}
	~bst() {};
};

void inorder(Node* root)
{
	if (root != NULL)
	{
		inorder(root->left);
		std::cout << root->key << " ";
		inorder(root->right);
	}
}

void printGivenLevel(Node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        std::cout <<" "<< root->key;
    else if (level > 1) {
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}

// int main()
// {
// 	bst obj;
 
// 	obj.root = obj.insert(NULL, 17);
// 	obj.root = obj.insert(obj.root, 13);
// 	obj.root = obj.insert(obj.root, 20);
// 	obj.root = obj.insert(obj.root, 18);
// 	obj.root = obj.insert(obj.root, 25);
// 	obj.root = obj.insert(obj.root, 5);
// 	obj.root = obj.insert(obj.root, 14);
// 	obj.root = obj.insert(obj.root, 15);
// 	obj.root = obj.insert(obj.root, 10);
// 	obj.root = obj.insert(obj.root, 9);
// 	inorder(obj.root);
// 	std::cout << std::endl;
// 	// obj.deleteNode(obj.root, 13);
// 	// std::cout << std::endl;
// 	// printGivenLevel(obj.root, 2);
// 	// Node* suc = obj.Successor(obj.root, 60);
// 	// if (suc)
// 	// 	std::cout << suc->key << std::endl;
// 	// else
// 	// 	std::cout << "NULL" << std::endl;
// 	// Node* per = obj.Predecessor(obj.root, 5);
// 	// if (per)
// 	// 	std::cout << per->key << std::endl;
// 	// else
// 	// 	std::cout << "NULL" << std::endl;
// 	return 0;
// }