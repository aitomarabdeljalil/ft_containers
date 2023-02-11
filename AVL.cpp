/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 12:53:27 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/08 18:19:18 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

//https://www.youtube.com/watch?v=YWqla0UX-38&list=PLdo5W4Nhv31bbKJzrsKfMpo_grxuLl8LU&index=61&ab_channel=Jenny%27sLecturesCSIT
struct Node {
	int key;
	Node *left, *right;
	int height;
	Node (int _key)
	{
		this->key = _key;
		left = right = NULL;
		this->height = 1;
	}
};

/*
	case 1: x < y < z
		x                  
		 \				  y
		  y     => 	     / \
		   \			x   z     	left rotation LR
		   	z
	same for x > y > z => right rotation RR :
			x
		   /			  y
		  y		=> 	     / \     	right rotation RR
		 /			    z   y
		z
	case 2: x < z && z > y
		x			x
		 \			 \	
		  z		=>	  y  	=>  case 1 LR;  		RLR right left rotation
		 /			   \
		y				z
	same for z > x && y < x => use case 1 RR : LRR left right rotation :
		  x				x
		 /		   	   /
		z		=>	  y		=>  case 1 RR; 			LRR left right rotation
		 \		 	 /
		  y			z
*/

class AVL
{
public:
    Node *root;
public:
    AVL() : root(NULL) {};
	int		height(Node* node)
	{
		if (!node)
			return 0;
		return	node->height;
	}
	// RR right rotation
    Node*	RRotation(Node* node) // node -> x
	{
		Node*	lnode = node->left; // lnode -> y
		Node*	subtree = lnode->right;
		lnode->right = node;
		node->left = subtree;
		node->height = (height(node->left) > height(node->right)) ? height(node->left) + 1 : height(node->right) + 1;
		lnode->height = (height(lnode->left) > height(lnode->right)) ? height(lnode->left) + 1 : height(lnode->right) + 1;
		return lnode;
	}
	//LR right rotation
    Node*	LRotation(Node* node) // node -> x
	{
		Node*	rnode = node->right; // lnode -> y
		Node*	subtree = rnode->left;
		rnode->left = node;
		node->right = subtree;
		node->height = (height(node->left) > height(node->right)) ? height(node->left) + 1 : height(node->right) + 1;
		rnode->height = (height(rnode->left) > height(rnode->right)) ? height(rnode->left) + 1 : height(rnode->right) + 1;
		return rnode;
	}
	int		getBalanceFactor(Node* node)
	{
		if (!node)
			return 0;
		return height(node->left) - height(node->right);
	}
	Node*	insert(Node *node, int key)
	{
		//BST insertion
		if (!node)
			return new Node(key);
		else if (key < node->key)
			node->left = insert(node->left, key);
		else if (key > node->key)
			node->right = insert(node->right, key);
		else
			return node;
		//balancing
		node->height = (height(node->left) > height(node->right)) ? height(node->left) + 1 : height(node->right) + 1;
		int	bf = getBalanceFactor(node); 
		if (bf > 1 && key < node->left->key)
			return RRotation(node);
		if (bf < -1 && key > node->right->key)
			return LRotation(node);
		if (bf > 1 && key > node->left->key) {
			node->left = LRotation(node->left);
			return RRotation(node);
		}
		if (bf < -1 && key < node->right->key) {
			node->right = RRotation(node->right);
			return LRotation(node);
		}
		return node;
	}
	Node*	findMin(Node *node)
	{
		if (!node->left)
			return node;
		return findMin(node->left);
	}
	Node*	deleteNode(Node* node, int key)
	{
		//BST deletion
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
		//balancing
		node->height = (height(node->left) > height(node->right)) ? height(node->left) + 1 : height(node->right) + 1;
		int	bf = getBalanceFactor(node);
		
		if (bf > 1 && getBalanceFactor(node->left) >= 0)
			return RRotation(node);
		if (bf > 1 && getBalanceFactor(node->left) < 0) {
			node->left = LRotation(node->left);
			return RRotation(node);
		}
		if (bf < -1 && getBalanceFactor(node->right) <= 0)
			return LRotation(node);
		if (bf < -1 && getBalanceFactor(node->right) > 0) {
			node->right = RRotation(node->right);
			return LRotation(node);
		}
		return node;
	}
    ~AVL() {};
};

void print2DUtil(Node* root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += 5;
 
    // Process right child first
    print2DUtil(root->right, space);
 
    // Print current node after space
    // count
    std::cout << std::endl;
    for (int i = 5; i < space; i++)
        std::cout << " ";
    std::cout << root->key << "\n";
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(Node* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

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
// 	AVL obj;

// 	obj.root = obj.insert(NULL, 14);
// 	obj.root = obj.insert(obj.root, 17);
// 	obj.root = obj.insert(obj.root, 11);
// 	obj.root = obj.insert(obj.root, 7);
// 	obj.root = obj.insert(obj.root, 53);
// 	obj.root = obj.insert(obj.root, 4);
// 	obj.root = obj.insert(obj.root, 13);
// 	obj.root = obj.insert(obj.root, 12);
// 	obj.root = obj.insert(obj.root, 8);
// 	obj.root = obj.insert(obj.root, 60);
// 	obj.root = obj.insert(obj.root, 19);
// 	obj.root = obj.insert(obj.root, 16);
// 	obj.root = obj.insert(obj.root, 20);
// 	// inorder(obj.root);
// 	print2D(obj.root);
// 	std::cout << "--------------------------------------------------" << std::endl;
// 	obj.root = obj.deleteNode(obj.root, 8);
// 	obj.root = obj.deleteNode(obj.root, 7);
// 	obj.root = obj.deleteNode(obj.root, 11);
// 	obj.root = obj.deleteNode(obj.root, 14);
// 	print2D(obj.root);
// 	std::cout << "--------------------------------------------------" << std::endl;
// 	return 0;
// }