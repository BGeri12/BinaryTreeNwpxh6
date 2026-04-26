#pragma once
#include <exception>

template<typename Key, typename Value>
class BinaryTree
{
public:
	// rule of 0
	//BinaryTree() = default;
	//~BinaryTree() = default;
	//BinaryTree(const BinaryTree& other) = default;
	//BinaryTree& operator=(const BinaryTree& other) = default;
	//BinaryTree(BinaryTree&& other) = default;
	//BinaryTree& operator=(BinaryTree&& other) = default;



private:
	struct Node
	{
		Key key;
		Value value;
		Node* left;
		Node* right;

		Node(const Key& k) : key(k), value(Value()), left(nullptr), right(nullptr) {}
	};

	Node* root;
};

