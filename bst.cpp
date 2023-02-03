/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 11:20:23 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/03 15:10:57 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

struct Node {
    int key;
    Node *left, *right;
    Node (int _key)
    {
        this->key = _key;
        left = right = nullptr;
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
    Node * findNode(Node *node, int key)
    {
        if (!node)
            return NULL;
        if (key < node->key)
            node = findNode(node->left, key);
        else if (key > node->key)
            node = findNode(node->right, key);
        return node;
    }
    bool    hasChild(Node* node)
    {
        if (node->left != NULL || node->right != NULL)
            return true;
        return false;
    }
    void    deleteNode(Node *node, int key)
    {
        //case 1 : no child
        if ()
        //case 2 : one child
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

int main()
{
    bst obj;
 
    obj.root = obj.insert(NULL, 50);
    obj.root = obj.insert(obj.root, 30);
    obj.root = obj.insert(obj.root, 20);
    obj.root = obj.insert(obj.root, 40);
    obj.root = obj.insert(obj.root, 70);
    obj.root = obj.insert(obj.root, 60);
    obj.root = obj.insert(obj.root, 80);
 
    inorder(obj.root);
    
    // std::cout << obj.root->key << std::endl;
 
    return 0;
}