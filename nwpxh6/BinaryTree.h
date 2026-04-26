#pragma once
#include <exception>

class KeyNotFoundException : public std::exception {
public:
	const char* what() const noexcept override {
		return "Error: Key not found in the binary tree !";
	}
};

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

	BinaryTree() : root(nullptr) {}

	Value& operator[](const Key& k)
	{
		if (root == nullptr)
		{
			root = new Node(k);
			return root->value;
		}

		Node* current = root;
		Node* parent = nullptr;

		while (current != nullptr)
		{
			if (k == current->key)
				return current->value;

			parent = current;
			if (k < current->key)
				current = current->left;
			else
				current = current->right;
		}

		Node* newNode = new Node(k);
		if (k < parent->key)
			parent->left = newNode;
		else
			parent->right = newNode;

		return newNode->value;
	}

	const Value& operator[](const Key& k) const
	{
		Node* current = root;
		while (current != nullptr)
		{
			if (k == current->key)
				return current->value;
			if (k < current->key)
				current = current->left;
			else
				current = current->right;
		}
		throw KeyNotFoundException();
	}

	bool empty() const
	{
		return root == nullptr;
	}



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

