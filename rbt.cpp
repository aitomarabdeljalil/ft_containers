/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 18:20:56 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/11 18:51:18 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

enum Color {BLACK, RED};
// template <typename T>
struct Node
{
	int data;
	Color color;
	Node *left, *right, *parent;
	Node(int _data) : data(_data) {
		this->color = RED;
		this->left = this->right = this->parent = NULL;
	}
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
    std::cout << (root->color == RED ? "\e[0;31m" : "") << root->data << "\n";
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(Node* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}
// template <typename T>
class RedBlackTree
{
public:
	Node *root;
	Node *nil;
public:
	RedBlackTree() :root(NULL) {
		nil = new Node(INT_MIN);
		nil->color = BLACK;
		nil->left = nil;
		nil->right = nil;
	};
	/*
			->
		   /Q               P
		   / \             / \
		  P   C     =>    A   Q
		 / \                 / \
		A   B               B   C
	*/
	Node*   RRotation(Node* node) //node => Q
	{
		Node* p = node->left;
		Node* b = p->right;
		b->parent = node; 
		node->left = b;
		p->parent = node->parent;
		Node* qparent = node->parent;
		if (qparent) {
			if (qparent->data > node->data)
				qparent->left = p;
			else
				qparent->right = p;
		}    
		p->right = node;
		node->parent = p;
		return p;
	}
	/*
							<-
			Q               P \
		   / \             / \
		  P   C     <=    A   Q
		 / \                 / \
		A   B               B   C
	*/
	Node*   LRotation(Node* node)
	{
		Node* q = node->right;
		Node* b = q->left;
		b->parent = node;
		node->right = b;
		q->parent = node->parent;
		Node* pparent = node->parent;
		if (pparent) {
			if (pparent->data > node->data)
				pparent->left = q;
			else
				pparent->right = q;
		}
		q->left = node;
		node->parent = q;
		return q;
	}
	Node*	getUncle(Node* node)
	{
		Node*	gParent = node->parent ? node->parent->parent : NULL;
		if (gParent && node->data < gParent->data)
			return gParent->right;
		else if (gParent && node->data > gParent->data)
			return gParent->left;
		return NULL;
	}
	Node*    rbInsertFixup(Node* node, int key)
	{
		if (!node){
			node = new Node(key);
			std::cout << node->color;}
		else if (key > node->data)
		{
			node->right = rbInsertFixup(node->right, key);
			node->right->parent = node;
		}
		else if (key < node->data)
		{
			node->left = rbInsertFixup(node->left, key);
			node->left->parent = node;
		}
		else
			return node;
		print2D(root);
		//balancng & recoloring
		if (!node->parent)
			node->color = BLACK;
		else {
			if (node->parent->color == RED) {
				Node* uncle = getUncle(node);
				if (node->parent->parent && uncle->color == RED) {
					node->parent->color = BLACK;
					uncle->color = BLACK;
					uncle->parent->color = RED;
				} 
				else if (node->parent->parent) {
					if (node->data < node->parent->data && node->parent->data < uncle->parent->data) {
						return RRotation(uncle->parent)->left;
					}
				}
			}
		}
		return node;
	}
	void insert(Node *node, int key)
	{
		root = rbInsertFixup(node, key);
		print2D(root);
		// std::cout << "here\n";
	}
	~RedBlackTree() {};
};

void inorder(Node* root)
{
	if (root != NULL)
	{
		inorder(root->left);
		std::cout << root->data << " ";
		inorder(root->right);
	}
}

void printGivenLevel(Node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        std::cout <<" "<< root->data;
    else if (level > 1) {
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}

int main() {
	RedBlackTree obj;

	obj.insert(obj.root, 3);
	obj.insert(obj.root, 2);
	obj.insert(obj.root, 1);
	// inorder(obj.root);
	return 0;
}
