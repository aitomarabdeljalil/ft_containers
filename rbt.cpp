/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 18:20:56 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/12 18:51:40 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

enum Color {BLACK, RED};
enum Events {
	def = 0,
	LeftRot,
	LeftRotColorSwap,
	RightRot,
	RightRotColorSwap,
};
// template <typename T>
struct Node
{
	int data;
	Color color;
	Node *left, *right, *parent;
	Events event;
	Node(int _data, Node* _parent) : data(_data), parent(_parent) {
		this->color = RED;
		this->left = this->right = NULL;
		event = def;
	}
};

void print2DUtil(Node* root, int space)
{
    if (root == NULL)
        return;
    space += 5;
    print2DUtil(root->right, space);
    std::cout << std::endl;
    for (int i = 5; i < space; i++)
        std::cout << " ";
    std::cout << (root->color == RED ? "\e[0;31m" : "") << root->data << "\e[0m\n";
	// std::cout << root->data << (root->color == RED ? "R" : "B")  << "\n";
    print2DUtil(root->left, space);
}

void print2D(Node* root)
{
    print2DUtil(root, 0);
	std::cout << "-------------------------\n";
}

// template <typename T>
class RedBlackTree
{
public:
	Node *root;
	Node *nil;
public:
	RedBlackTree() :root(NULL) {
		// nil = new Node(INT_MIN);
		// nil->color = BLACK;
		// nil->left = nil;
		// nil->right = nil;
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
		if ( b ) b->parent = node;
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
		if (b) b->parent = node;
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
	void	colorSwap(Node* nd1, Node* nd2)
	{
		Color colorTmp = nd1->color;
		nd1->color = nd2->color;
		nd2->color = colorTmp;
	}
	Node*    rbInsertFixup(Node* node, int key, Node* _parent = NULL)
	{
		if (!node) {
			node = new Node(key, _parent);
			if (_parent) {
				if (node->parent->color == RED) {
					Node* grandParent = node->parent->parent;
					Node* uncle = getUncle(node);
					if (grandParent && uncle && uncle->color == RED) {
						node->parent->color = BLACK;
						uncle->color = BLACK;
						grandParent->color = RED;
						print2D(root);
					} 
					else if (grandParent) {
						if (node->data < node->parent->data && node->parent->data < grandParent->data) {
							grandParent->event = RightRotColorSwap;
						} else if (node->data > node->parent->data && node->parent->data > grandParent->data) {
							grandParent->event = LeftRotColorSwap;
						} else if (node->data > node->parent->data &&  node->parent->data < grandParent->data) {
							node->parent->event = LeftRot;
							grandParent->event = RightRotColorSwap;
						} else {
							node->parent->event = RightRot;
							grandParent->event = LeftRotColorSwap;
						}
					}
				}	
			} 
		}
		else if (key > node->data)
			node->right = rbInsertFixup(node->right, key, node);
		else if (key < node->data)
			node->left = rbInsertFixup(node->left, key, node);
		else
			return node;
		if (!node->parent)
			node->color = BLACK;
		if (node->event != def) {
			Events ev = node->event;
			node->event = def;
			if (ev == RightRot || ev == RightRotColorSwap) {
				node = RRotation(node);
				if (ev == RightRotColorSwap)
					colorSwap(node, node->right);
			}
			if (ev == LeftRot || ev == LeftRotColorSwap) {
				node = LRotation(node);
				if (ev == LeftRotColorSwap)
					colorSwap(node, node->left);
			}
		}
		return node;
	}
	void insert(Node *node, int key)
	{
		root = rbInsertFixup(node, key);
	}
	Node*	SearchNode(Node* node, int key)
	{
		if (!node || node->data == key)
			return node;
		if (key < node->data)
			return SearchNode(node->left, key);
		else
			return SearchNode(node->right, key);
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

	obj.insert(obj.root, 50);
	obj.insert(obj.root, 60);
	obj.insert(obj.root, 20);
	// print2D(obj.root);
	obj.insert(obj.root, 10);
	// print2D(obj.root);
	obj.insert(obj.root, 15);
	// print2D(obj.root);
	obj.insert(obj.root, 18);
	// print2D(obj.root);
	obj.insert(obj.root, 16);
	print2D(obj.root);
	// Node* tst = obj.SearchNode(obj.root, 16);
	// if (tst)
	// 	std::cout << tst->data <<"\n";
	// else
	// 	std::cout << "no";
	return 0;
}
