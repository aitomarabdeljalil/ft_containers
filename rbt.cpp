/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 18:20:56 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/18 11:51:02 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <random>
#include <ctime>
#include <unistd.h>
// for ckecker
#include <cstddef>
#include <map>
#include <ostream>
#include <queue>
#include <iomanip>
// end
enum Color {RED, BLACK, DBLACK};
enum Events {
	def = 0,
	LeftRot,
	LeftRotColorSwap,
	RightRot,
	RightRotColorSwap,
};

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
    print2DUtil(root->left, space);
}

void print2D(Node* root)
{
    print2DUtil(root, 0);
	std::cout << "-------------------------\n";
}

enum e_directions { LEFT, DEFAULT, RIGHT };
struct output {
	Node	*_nd;
	int			 _lvl;
	int			 _spaces;
	int			 _addit;
	int			 _length;
	e_directions _dir;
	static int	 total_lvls;
	static int	 total_diff;
	static int	 _width;
	output( Node *nd, int lvl, int length, e_directions dir )
		: _nd( nd ), _lvl( lvl ), _length( length ? length : 1 ), _dir( dir ), _addit( 0 ) {};
};

int output::total_lvls = 0;
int output::total_diff = 2;
int output::_width	 = 0;

class RedBlackTree
{
public:
	Node *root;
	Node *nil;
public:
	RedBlackTree() :root(NULL)
	{
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
		if (node == root) root = p;
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

	void rightRotation(Node* x)
	{
		Node* y = x->left;
		x->left = y->right;
		if (y->right != NULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	void leftRotation(Node* x)
	{
		Node* y = x->right;
		x->right = y->left;
		if (y->left != NULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
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
	Node*	rbInsertFixup(Node* node, int key, Node* _parent = NULL)
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
		if (!node->parent)
			node->color = BLACK;
		else if ((node->left && node->left->color == RED) ||  (node->right && node->right->color == RED)) {
			if (node->color == RED) {
				Node* sib = findSib(node->parent, node);
				if (sib && sib->color == RED) {
					node->color = BLACK;
					sib->color = BLACK;
					node->parent->color = RED;
				} 
				else {
					if (key < node->data && node->data < node->parent->data) {
						node->parent->event = RightRotColorSwap;
					} else if (key > node->data && node->data > node->parent->data) {
						node->parent->event = LeftRotColorSwap;
					} else if (key > node->data &&  node->data < node->parent->data) {
						node->event = LeftRot;
						node->parent->event = RightRotColorSwap;
					} else {
						node->event = RightRot;
						node->parent->event = LeftRotColorSwap;
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
	void	insert(Node *&node, int key, const int idx)
	{
		// std::cout << " INSERTING " << std::setw( 16 ) << key << "\n";
		root = rbInsertFixup(node, key);
		// log();
		// checking
		std::map< Node *, std::vector< int > > mp;
		std::cout << "\e[1;31m";
		assert( check( root, mp ) );
		std::cout << "\e[0m";
		std::cout << "idx " << std::setw(10) << idx <<" INSERTED " << std::setw( 16 ) << key
				  << " SUCCESSFULLY\n\e[F\e[K";
				//   << " SUCCESSFULLY\n";
	}
	void	remove(Node *&node, int key, const int idx)
	{
		// std::cout << " DELETING " << std::setw( 16 ) << key << "\n";
		// log();
		deleteNode(node, key);
		// log();
		// checking
		std::map< Node *, std::vector< int > > mp;
		std::cout << "\e[1;31m";
		assert( check( root, mp ) );
		std::cout << "\e[0m";
		std::cout << "idx " << std::setw(10) << idx <<" DELETED " << std::setw( 16 ) << key
				  << " SUCCESSFULLY\n\e[F\e[K";
				//   << " SUCCESSFULLY\n";
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
	Node*	findMin(Node *node)
	{
		if (!node->left)
			return node;
		return findMin(node->left);
	}
	Node*	findSib(Node* parent, Node* child)
	{
		if (parent && parent->left && parent->left->data == child->data)
			return parent->right;
		return parent->left;
	}
	void	fixDB(Node* node)
	{
		Node* tmp = node;
		if (node->color != DBLACK) return ;
		if (!node->parent) {
			node->color = BLACK;
			return ;
		}
		Node* sib = findSib(node->parent, node);
		Node* grandparent = sib->parent->parent;
		Node* nparent = node->parent;
		if (sib && sib->color == RED) {	
		// std::cout << "sib : " << sib->data << "node parent " << nparent->data << "\n";
		// log();
			colorSwap(nparent, sib);
			// if (nparent->left == node)
			// 	leftRotation(nparent);
			// else
			// 	rightRotation(nparent);
			if (nparent->left == node)
				nparent = LRotation(nparent);
			else
				nparent = RRotation(nparent);
			// log();
			// std::cout << "i am " << node->data << ", my parent is " << node->parent->data<< ", my grandparent is " << node->parent->parent->data << '\n';
			// grandparent = node->parent;
			// std::cout << "gp is " << grandparent->data << '\n';
			// exit(0);
			// node->parent = node->parent;
			// print2D(nparent);
			// log();
			fixDB(node);
			// print2D(root);
		} else {
			if (!sib || (sib && (!sib->right && !sib->left)) 
				|| (sib && (sib->right && sib->left) && sib->left->color == BLACK && sib->right->color == BLACK)) {
				node->color = BLACK;
				node->parent->color = Color(node->parent->color + 1);
				if (sib) sib->color = RED;
				// log();
				fixDB(node->parent);
			} else if ((node->parent->left == node && sib->left && sib->left->color == RED) 
					|| (node->parent->right == node && sib->right && sib->right->color == RED)) {
				Node* nc;
				node->color = BLACK;
				if (node->parent->left == node && sib->left && sib->left->color == RED) {
					nc = sib->left;
					tmp = LRotation(RRotation(sib)->parent);
					colorSwap(tmp, node->parent);
					tmp->left->color=BLACK;
					// if (grandparent) grandparent->right = tmp;
				} else {
					nc = sib->right;
					tmp = RRotation(LRotation(sib)->parent);
					colorSwap(tmp, node->parent);
					tmp->right->color=BLACK;
					// if (grandparent) grandparent->left = tmp;
				}
			} else {
				Node* np = NULL;
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
	void	deleteNode(Node *&node, int key)
	{
		//BST deletion
		Node	*tmp;
		if (!node)
			return ;
		else if (key < node->data)
			deleteNode(node->left, key);
		else if (key > node->data)
			deleteNode(node->right, key);
		else if (!node->left && !node->right){
			if (node->color == BLACK)
				node->color = DBLACK;
			fixDB(node);
			delete(node);
			node = NULL;
		} else if (!node->left || !node->right){
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
	~RedBlackTree() {};
	void _getTotalLvls( Node *x, const int lvl ) {
		if ( !x ) return;
		else if ( lvl >= output::total_lvls )
			output::total_lvls = lvl + 1;
		_getTotalLvls( x->left, lvl + 1 );
		_getTotalLvls( x->right, lvl + 1 );
	};
	void _store( Node *x, std::vector< output > &out, const int lvl, e_directions dir ) {
		if ( lvl == output::total_lvls ) return;
		_store( ( x ? x->left : x ), out, lvl + 1, LEFT );
		std::stringstream ss;
		if ( x ) ss << x->data;
		out.push_back( output( x, lvl, ss.str().length(), dir ) );
		_store( ( x ? x->right : x ), out, lvl + 1, RIGHT );
	};
	void _log( std::vector< output > out ) {
		if ( !out.size() ) {
			std::cout << "empty rbt\n";
			return;
		}
		int indent = 2;
		for ( int l = output::total_lvls - 1, i = -1, lsl = -1, diff = 2; l >= 0;
			  l--, i = -1, lsl = -1, diff *= 2 ) {
			while ( ++i < out.size() ) {
				if ( l != out[ i ]._lvl ) continue;
				if ( lsl == -1 && l == output::total_lvls - 1 ) out[ i ]._spaces = 0;
				else
					out[ i ]._spaces = indent + out[ i ]._addit;
				out[ i ]._addit += out[ i ]._length;
				if ( out[ i ]._dir == RIGHT && out[ i - ( diff / 2 ) ]._dir == RIGHT )
					out[ i - ( diff / 2 ) ]._addit +=
						out[ i ]._addit + ( out[ i ]._length + out[ i - diff ]._length ) / 2;
				if ( lsl == -1 ) indent *= ( indent ? 2 : 1 );
				lsl = i;
			}
			output::total_diff = diff;
		}
		// std::cout << std::setw( 5 ) << "@|" << std::setw( 11 ) << "nb|" << std::setw( 7 ) <<
		// "lvl|"
		// 		  << std::setw( 7 ) << "sp|" << std::setw( 7 ) << "addit|" << '\n';
		// for ( int i = 0; i < out.size(); i++ )
		// 	std::cout << std::setw( 4 ) << i << "|" << std::setw( 10 )
		// 			  << ( out[ i ]._nd ? out[ i ]._nd->_elt : 0 ) << "|" << std::setw( 6 )
		// 			  << out[ i ]._lvl << "|" << std::setw( 6 ) << out[ i ]._spaces << "|"
		// 			  << std::setw( 6 ) << out[ i ]._addit << "|" << '\n';
		for ( int l = 0, i = -1, diff = output::total_diff, width = 0; l < output::total_lvls;
			  l++, i = -1, diff /= 2, width = 0 ) {
			if ( l != 0 ) {
				std::cout << "\e[2m";
				while ( ++i < out.size() ) {
					if ( l != out[ i ]._lvl ) continue;
					for ( int s = 0, end = out[ i ]._spaces + out[ i ]._length / 2; s < end; s++ ) {
						if ( out[ i ]._dir == RIGHT && out[ i ]._nd && out[ i - diff ]._nd )
							std::cout << "─";
						else if ( out[ i ]._dir == RIGHT && out[ i ]._nd && !out[ i - diff ]._nd &&
								  s > end / 2 )
							std::cout << ( s == end / 2 + 1 ? "└" : "─" );
						else if ( out[ i ]._dir == RIGHT && !out[ i ]._nd && out[ i - diff ]._nd &&
								  s < end / 2 )
							std::cout << ( s == end / 2 - 1 ? "┘" : "─" );
						else
							std::cout << " ";
					}
					if ( out[ i ]._nd ) std::cout << ( out[ i ]._dir == LEFT ? "┌" : "┐" );
					else
						std::cout << ' ';
				}
				std::cout << "\e[22m\n";
			}
			i = -1;
			while ( ++i < out.size() ) {
				if ( l != out[ i ]._lvl ) continue;
				for ( int s = 0; s < out[ i ]._spaces; s++, width++ ) std::cout << ' ';
				if ( out[ i ]._nd )
					std::cout << ( out[ i ]._nd->color == RED ? "\e[1;31m" : "\e[1;37m" )
							  << ( out[ i ]._nd->color == DBLACK ? "\e[4m" : "" )
							  << out[ i ]._nd->data << "\e[0m";
				else
					std::cout << ' ';
				width += out[ i ]._length;
			}
			output::_width = std::max( width + 10, output::_width );
			std::cout << '\n';
		}
		std::cout << "\e[2m";
		for ( int i = 0; i < output::_width; i++ ) std::cout << "─";
		std::cout << "\e[22m\n";
	};
	// tmp
	void log( void ) {
		log(root);
	}
	void log( Node* nd ) {
		if ( !nd ) return;
		output::total_lvls = 0;
		output::total_diff = 2;
		output::_width	   = 0;
		std::vector< output > out;
		_getTotalLvls( nd, 0 );
		_store( nd, out, 0, DEFAULT );
		_log( out );
	};
	// checker {made by: jalalium}
	bool check( Node *nd, std::map< Node *, std::vector< int > > &mp ) {
		bool ok = true;
		if (!nd)
		return ok;
		if ( nd->left != NULL ) {
			if ( nd->data <= nd->left->data ) {
				std::cout << "LEFT CHILD LARGER OR EQUAL TO PARENT " << nd->data << " "
						  << nd->left->data << std::endl;
				return false;
			}
			if ( nd->color == RED && nd->left->color == RED ) {
				std::cout << "RED PARENT WITH RED CHILD: " << nd->data << " " << nd->left->data
						  << std::endl;
				return false;
			}
			ok &= check( nd->left, mp );
		}
		if ( nd->right != NULL ) {
			if ( nd->data >= nd->right->data ) {
				std::cout << "RIGHT CHILD SMALLER OR EQUAL TO PARENT " << nd->data << " "
						  << nd->left->data << std::endl;
				return false;
			}
			if ( nd->color == RED && nd->right->color == RED ) {
				std::cout << "RED PARENT WITH RED CHILD: " << nd->data << " " << nd->right->data
						  << std::endl;
				return false;
			}
			ok &= check( nd->right, mp );
		}
		std::vector< int > tmp;
		if ( nd->left != NULL ) {
			for ( int z : mp[ nd->left ] ) tmp.push_back( z );
		} else
			tmp.push_back( 0 );
		if ( nd->right != NULL ) {
			for ( int z : mp[ nd->right ] ) tmp.push_back( z );
		} else
			tmp.push_back( 0 );
		std::sort( tmp.begin(), tmp.end() );
		for ( int &z : tmp ) z += ( nd->color == BLACK );
		mp[ nd ] = tmp;
		if ( tmp[ 0 ] != tmp.back() ) {
			std::cout << "NODE " << nd->data << std::endl;
			for ( int z : tmp ) std::cout << z << " ";
			std::cout << std::endl;
		}
		return ( ok & ( tmp[ 0 ] == tmp.back() ) );
	}
	// end of checker!
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

int main(void) {
	
	// random
	{
	std::system( "clear" );
	std::cout << std::boolalpha;
	srand( time( NULL ) );
	std::random_device rd;
	std::mt19937	   mt( rd() );
		for ( int test = 0, total_tests = 50, sz = 1000, range = INT_MAX/sz; test < total_tests; test++ ) {
			std::cout << "\e[s\e[1;37m"
					  << "────────────────────────>"
					  << "     TEST " << test + 1 << " of " << total_tests
					  << " START    "
						 "<────────────────────────"
					  << "\e[0m\n";
			std::vector< int > in;
			for ( int i = 0; i < sz; i++ )
			// for ( int i = 0, range = 10; i < sz; i++ )
				in.push_back( rand() % range + range * i );
			std::vector< int > out( in );
			std::shuffle( in.begin(), in.end(), mt );
			std::shuffle( out.begin(), out.end(), mt );
			// print all
			// std::cout << "insert ";
			// for ( int i = 0; i < sz; i++ ) std::cout << in.at( i ) << ' ';
			// std::cout << '\n';
			// std::cout << "delete ";
			// for ( int i = 0; i < sz; i++ ) std::cout << out.at( i ) << ' ';
			// std::cout << '\n';
			// exit( 0 );
			RedBlackTree n;
			std::cout << "\e[1;34m"
					  << "────────>    STARTING INSERTION    "
					  << "\e[0m\n";
			usleep( 10000 );
			for ( int i = 0; i < sz; i++ ) n.insert(n.root, in.at( i ), i );
			std::cout << "\e[F\e[J\e[1;32m"
					  << "────────>    INSERTION SUCCESS    "
					  << "\e[0m\n";
			usleep( 10000 );
			// n.log();
			std::cout << "\e[1;34m"
					  << "────────>    STARTING DELETION    "
					  << "\e[0m\n";
			usleep( 10000 );
			for ( int i = 0; i < sz; i++ ) n.remove(n.root, out.at( i ), i);
			std::cout << "\e[F\e[J\e[1;32m"
					  << "────────>    DELETION SUCCESS    "
					  << "\e[0m\n";
			usleep( 10000 );
			std::cout << "\e[u\e[J\e[0;32m"
					  << "────────────────────────>"
					  << "    TEST " << test + 1 << " of " << total_tests
					  << " SUCCESS   "
						 "<────────────────────────"
					  << "\e[0m\n";
			// n.log();
		}
		std::cout << "\e[1;32m"
				  << "────────────────────────>    SUCCEEDED ALL TESTS   "
					 "<────────────────────────"
				  << "\e[0m\n";
	}
	
	// {
	// RedBlackTree obj;

	// obj.insert(obj.root, 1);
	// obj.insert(obj.root, 0);
	// obj.insert(obj.root, 2);
	// // obj.log();
	// obj.insert(obj.root, 3);
	// // obj.log();
	// // obj.insert(obj.root, 4);
	// // obj.log();
	// obj.insert(obj.root, 5);
	// // obj.log();
	// obj.insert(obj.root, 6);
	// // std::cout << "right after insert 6\n";
	// // obj.log();
	// // obj.insert(obj.root, 7);
	// // for (int i = 0; i < 8; i++)
	// // {
	// // 	obj.insert(obj.root, i);
	// // 	if (!isBalanced(obj.root))
	// // 	{
	// // 		std::cout << "shot " << std::endl;
	// // 		exit(5);
	// // 	}
	// // }
	// // std::cout << "right after insert 7\n";
	// obj.log();
	// // Node* tst = obj.SearchNode(obj.root, 16);
	// // if (tst)
	// // 	std::cout << tst->data <<"\n";
	// // else
	// // 	std::cout << "no";
	// // std::cout << "--> delete\n";
	// obj.deleteNode(obj.root, 6);
	// obj.log();
	// obj.deleteNode(obj.root, 0);
	// // std::cout << obj.root->data << obj.root->left->data << '\n';
	// // print2D(obj.root);
	// obj.log();
	// // print2D(obj.root);
	// // obj.deleteNode(obj.root, 10);
	// // obj.log();
	// // obj.deleteNode(obj.root, 60);
	// // obj.log();
	// // }
	
	// {
	// 	int in[] = {188,90,149,72,20,49,211,208,50,2,105,195,36,138,243,280,221,83,231,62,295,126,116,17,161,152,265,252,173,279};
	// 	int out[] = {83,126,49,188,138,221,173,105,62,50,90,195,279,231,208,72,295,149,243,17,152,20,116,2,161,265,36,211,252,280};
	// 	RedBlackTree obj;
	// 	for (int i = 0; i < 30;i++) obj.insert(obj.root, in[i], i);
	// 	// obj.log();
	// 	for (int i = 0; i < 30;i++) obj.remove(obj.root, out[i], i);
	// 	// obj.log();
		
	// }
	return 0;
}
