/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedBlackTree.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:20:36 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/19 11:48:06 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDBLACKTREE_HPP
# define REDBLACKTREE_HPP
# include <iostream>
# include <sstream>
# include <vector>
# include <algorithm>
# include <climits>
# include <random>
# include <ctime>
# include <unistd.h>
// for ckecker
# include <cstddef>
# include <map>
# include <ostream>
# include <queue>
# include <iomanip>
// end

enum Color {RED, BLACK, DBLACK};
enum Events {
	def = 0,
	LeftRot,
	LeftRotColorSwap,
	RightRot,
	RightRotColorSwap,
};

template<typename T>
struct Node
{
	T data;
	Color color;
	Node<T> *left, *right, *parent;
	Events event;
	Node(T _data, Node<T>* _parent) : data(_data), parent(_parent) {
		this->color = RED;
		this->left = this->right = NULL;
		event = def;
	}
	bool		isRed() {
		return (this->color == RED) : true ? false;
	}
	Node<T>*    getParent() {
		return this->parent;
	}
	bool        hasChild() {
		if (this->left || this->right)
			return true;
		return false;
	}
	Node<T>*    findSib() {
		Node<T>*    _parent = this->getParent();
		if (_parent && _parent->left && _parent->left->data == this->data)
			return _parent->right;
		return _parent->left;
	}
};

template <class T>
class RedBlackTree
{
	private:
		/*--- Rotations ---*/
		Node<T>*   RRotation(Node<T>* node)
		{
			Node<T>* p = node->left;
			Node<T>* b = p->right;
			if (node == root) root = p;
			if ( b ) b->parent = node;
			node->left = b;
			p->parent = node->parent;
			Node<T>* qparent = node->parent;
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
		Node<T>*   LRotation(Node<T>* node)
		{
			Node<T>* q = node->right;
			Node<T>* b = q->left;
			if (node == root) root = q;
			node->right = b;
			if (b) b->parent = node;
			q->left = node;
			q->parent = node->parent;
			if (node->parent) {
				if (node->parent->data > node->data)
					node->parent->left = q;
				else
					node->parent->right = q;
			}
			node->parent = q;
			return q;
		}
		/*--- recolore ---*/
		void	colorSwap(Node* nd1, Node* nd2)
		{
			Color colorTmp = nd1->color;
			nd1->color = nd2->color;
			nd2->color = colorTmp;
		}
		/*--- insertion ---*/
		Node<T>*	rbInsertFixup(Node<T>* node, T key, Node<T>* _parent = NULL)
		{
			if (!node) {
				node = new Node(key, _parent);
				return node;
			}
			else if (key > node->data)
				node->right = rbInsertFixup(node->right, key, node);
			else if (key < node->data)
				node->left = rbInsertFixup(node->left, key, node);
			else
				return node;
			if (!node->getParent())
				node->color = BLACK;
			else if ((node->left && node->left->color == RED) ||  (node->right && node->right->color == RED)) {
				if (node->color == RED) {
					Node<T>* sib = node->findSib();
					if (sib && sib->color == RED) {
						node->color = BLACK;
						sib->color = BLACK;
						node->getParent()->color = RED;
					} 
					else {
						if (key < node->data && node->data < node->getParent()->data) {
							node->getParent()->event = RightRotColorSwap;
						} else if (key > node->data && node->data > node->getParent()->data) {
							node->getParent()->event = LeftRotColorSwap;
						} else if (key > node->data &&  node->data < node->getParent()->data) {
							node->event = LeftRot;
							node->getParent()->event = RightRotColorSwap;
						} else {
							node->event = RightRot;
							node->getParent()->event = LeftRotColorSwap;
						}
					}
				}	
			}
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
		/*--- deletion ---*/
		void	fixDB(Node<T>* node)
		{
			Node<T>* tmp = node;
			if (node->color != DBLACK) return ;
			if (!node->getParent()) {
				node->color = BLACK;
				return ;
			}
			Node<T>* sib = node->findSib();
			Node<T>* nparent = node->getParent();
			Node<T>* grandparent = sib->getParent()->parent;
			if (sib && sib->isRed()) {
				if (nparent->left == node)
					nparent = LRotation(nparent);
				else
					nparent = RRotation(nparent);
				colorSwap(nparent, sib);
				fixDB(node);
			} else {
				if (!sib || (sib && !sib->hasChild())
					|| (sib && (sib->right && sib->left) && sib->left->color == BLACK && sib->right->color == BLACK)) {
					node->color = BLACK;
					node->parent->color = Color(node->parent->color + 1);
					if (sib) sib->color = RED;
					fixDB(node->parent);
				} else if ((node->parent->left == node && sib->left && sib->left->color == RED) 
						|| (node->parent->right == node && sib->right && sib->right->color == RED)) {
					Node<T>* nc;
					node->color = BLACK;
					if (node->parent->left == node && sib->left && sib->left->color == RED) {
						nc = sib->left;
						tmp = LRotation(RRotation(sib)->parent);
						colorSwap(tmp, node->parent);
						tmp->left->color=BLACK;
					} else {
						nc = sib->right;
						tmp = RRotation(LRotation(sib)->parent);
						colorSwap(tmp, node->parent);
						tmp->right->color=BLACK;
					}
				} else {
					Node<T>* np = NULL;
					node->color = BLACK;
					if (node->parent->left == node && sib->right && sib->right->color == RED) {
						np = sib->right;
						tmp = LRotation(sib->parent);
						colorSwap(tmp, node->parent);
						tmp->left->color=BLACK;
						tmp->right->color=BLACK;
					} else {
						np = sib->left;
						tmp = RRotation(sib->parent);
						colorSwap(tmp, node->parent);
						tmp->left->color=BLACK;
						tmp->right->color=BLACK;
					}
				}
			}
		}
		void	deleteNode(Node<T> *&node, T key)
		{
			//BST deletion
			Node<T>	*tmp;
			if (!node)
				return ;
			else if (key < node->data)
				deleteNode(node->left, key);
			else if (key > node->data)
				deleteNode(node->right, key);
			else if (!node->left && !node->right) {
				if (!node->isRed())
					node->color = DBLACK;
				fixDB(node);
				delete(node);
				node = NULL;
			} else if (!node->left || !node->right) {
				tmp = node;
				if (!node->left)
					node = node->right;
				else if (!node->right)
					node = node->left;
				node->parent = tmp->parent;
				node->color = tmp->color;
				delete tmp;
				tmp = NULL;
			} else {
				tmp = findMin(node->right);
				node->data = tmp->data;
				deleteNode(node->right, tmp->data);
			}
		}
	public:
		RedBlackTree();
		void		insert(T key) {
			root = rbInsertFixup(root, key);
		}
		Node<T>*	find(T key) {
			Node<T>*	node = root;

			if (!node || node->data == key)
				return node;
			if (key < node->data)
				return find(node->left, key);
			return find(node->right, key);
		}
		void		remove(T key) {
			deleteNode(root, key);
		}
		~RedBlackTree();
	private:
		Node<T>* root;
};

#endif