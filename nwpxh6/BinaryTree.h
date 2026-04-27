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

	~BinaryTree() {
		DestroyTree(root);
	}

	BinaryTree(const BinaryTree& other) : root{CopyTree(other.root)} {}

	BinaryTree& operator=(const BinaryTree& other)
	{
		if (this != &other)
		{
			DestroyTree(root);
			root = CopyTree(other.root);
		}
		return *this;
	}

	BinaryTree(BinaryTree&& other) noexcept
	: root(other.root)
	{
		other.root = nullptr;
	}

	BinaryTree& operator=(BinaryTree&& other) noexcept
	{
		if (this != &other)
		{
			DestroyTree(root);
			root = other.root;
			other.root = nullptr;
		}
		return *this;
	}

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

	bool Empty() const
	{
		return root == nullptr;
	}

	void Clear()
	{
		DestroyTree(root);
		root = nullptr;
	}

	bool Contains(const Key& k) const
	{
		Node* current = root;
		while (current != nullptr)
		{
			if (k == current->key)
				return true;
			if (k < current->key)
				current = current->left;
			else
				current = current->right;
		}
		return false;
	}

	bool Insert(const Key& k, const Value& v) {
		
		if (root == nullptr) {
			root = new Node(k);
			root->value = v;
			return true;
		}

		Node* current = root;
		Node* parent = nullptr;

		while (current != nullptr) {
			if (k == current->key) 
				return false; 

			parent = current;
			if (k < current->key)
				current = current->left;
			else 
				current = current->right;
		}

		Node* newNode = new Node(k);
		newNode->value = v;
		if (k < parent->key) parent->left = newNode;
		else parent->right = newNode;

		return true;
	}

	bool Remove(const Key& k) {
		bool success = false;
		root = RemoveNode(root, k, success);
		return success;
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

	void DestroyTree(Node* node)
	{
		if (node != nullptr)
		{
			DestroyTree(node->left);
			DestroyTree(node->right);
			delete node;
		}
	}

	Node* CopyTree(const Node* other)
	{
		if (other == nullptr)
			return nullptr;

		Node* newNode = new Node(other->key);
		newNode->value = other->value;
		newNode->left = CopyTree(other->left);
		newNode->right = CopyTree(other->right);
		return newNode;
	}

	Node* RemoveNode(Node* node, const K& key, bool& success) {
		if (node == nullptr)
			return nullptr;

		if (key < node->key) {
			node->left = RemoveNode(node->left, key, success);
		}
		else if (key > node->key) {
			node->right = RemoveNode(node->right, key, success);
		}
		else {
			success = true;

			if (node->left == nullptr) {
				Node* temp = node->right;
				delete node;
				return temp;
			}
			else if (node->right == nullptr) {
				Node* temp = node->left;
				delete node;
				return temp;
			}
			Node* temp = node->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			node->key = temp->key;    
			node->value = temp->value;
			node->right = RemoveNode(node->right, temp->key, success);
		}
		return node;
	}

};

