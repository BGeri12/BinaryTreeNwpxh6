#pragma once
#include <exception>

class KeyNotFoundException : public std::exception
{
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

	BinaryTree(): root(nullptr) {}

	~BinaryTree(){ DestroyTree(root);}

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

	const Value& operator[](const Key& k) const
	{
		Node* node = FindNodeHelper(k);
		
		if (node != nullptr)
			return node->value;

		throw KeyNotFoundException();
	}

	bool Contains(const Key& k) const
	{
		return FindNodeHelper(k) != nullptr;
	}

	Value& operator[](const Key& k)
	{
		bool wasInserted;
		Node* node = InsertNodeHelper(k, wasInserted);
		return node->value;
	}

	bool Insert(const Key& k, const Value& v)
	{
		bool wasInserted;
		Node* node = InsertNodeHelper(k, wasInserted);
		if (wasInserted)
		{
			node->value = v;
			return true;
		}

		return false;
	}

	bool Remove(const Key& k) {
		bool success = false;
		root = RemoveNodeHelper(root, k, success);
		return success;
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

	Node* FindNodeHelper(const Key& k) const
	{
		Node* current = root;
		while (current != nullptr)
		{
			if (k == current->key)
				return current;

			if (k < current->key)
				current = current->left;
			else
				current = current->right;
		}
		return nullptr;
	}

	Node* InsertNodeHelper(const Key& k, bool& wasInserted) 
	{
		wasInserted = false;

		if (root == nullptr)
		{
			root = new Node(k);
			wasInserted = true;
			return root;
		}

		Node* current = root;
		Node* parent = nullptr;

		while (current != nullptr)
		{
			if (k == current->key)
				return current;
			
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

		wasInserted = true;
		return newNode;
	}

	Node* RemoveNodeHelper(Node* node, const Key& k, bool& success) {
		if (node == nullptr)
			return nullptr;

		if (k < node->key) {
			node->left = RemoveNodeHelper(node->left, k, success);
		}
		else if (k > node->key) {
			node->right = RemoveNodeHelper(node->right, k, success);
		}
		else
		{
			success = true;

			if (node->left == nullptr)
			{
				Node* temp = node->right;
				delete node;
				return temp;
			}
			else if (node->right == nullptr)
			{
				Node* temp = node->left;
				delete node;
				return temp;
			}
			Node* temp = node->right;
			while (temp->left != nullptr)
			{
				temp = temp->left;
			}
			node->key = temp->key;    
			node->value = temp->value;
			node->right = RemoveNodeHelper(node->right, temp->key, success);
		}
		return node;
	}

	class Stack 
	{
	private:
		struct StackNode
		{
			Node* treeNode;
			StackNode* next;
			
			StackNode(Node* node) 
			:treeNode{ node }
			,next{ nullptr }
			{
			}
		};
		StackNode* topNode;

		void CopyFromHelper(const Stack& other)
		{
			if (other.topNode == nullptr)
			{
				topNode = nullptr;
				return;
			}

			topNode = new StackNode(other.topNode->treeNode);
			StackNode* currentThis = topNode;
			StackNode* currentOther = other.topNode->next;

			while (currentOther != nullptr)
			{
				currentThis->next = new StackNode(currentOther->treeNode);
				currentThis = currentThis->next;
				currentOther = currentOther->next;
			}
		}

	public:
		Stack() : topNode{nullptr} {}

		~Stack() 
		{
			while (!IsEmpty()) 
			{
				Pop();
			}
		}

		bool IsEmpty() 
		{
			return topNode == nullptr;
		}

		Node* Top() const
		{
			if (topNode != nullptr) return topNode->treeNode;
			return nullptr;
		}

		void Pop() 
		{
			if (topNode != nullptr)
			{
				StackNode* temp = topNode;
				topNode = topNode->next;
				delete temp;
			}
		}

		void Push(Node* node) 
		{
			StackNode* newNode = new StackNode(node);
			newNode->next = topNode;
			topNode = newNode;
		}

		Stack(const Stack& other)
		:topNode { nullptr }
		{
			CopyFromHelper(other);
		}

		Stack& operator=(const Stack& other)
		{
			if (this != &other)
			{
				while (!IsEmpty()) 
				{
					Pop();
				}

				CopyFromHelper(other);
			}
			return *this;
		}
	};

	public:
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Value;
			using difference_type = std::ptrdiff_t;
			using pointer = Value*;
			using reference = Value&;

		private:
			Stack path;
			Node* current;

			void MoveToLeftMost(Node* node)
			{
				while (node != nullptr)
				{
					path.Push(node);
					node = node->left;
				}
			}

		public:
			Iterator() : current{ nullptr } {}

			Iterator(Node* root)
			{
				MoveToLeftMost(root);
				current = path.IsEmpty() ? nullptr : path.Top();
			}

			reference operator*() const
			{
				return current->value;
			}

			reference& operator*()
			{
				return current->value;
			}

			const Key& GetKey() const
			{
				return current->key;
			}

			Iterator& operator++()
			{
				if (path.IsEmpty())
				{
					current = nullptr;
					return *this;
				}

				Node* node = path.Top();
				path.Pop();

				if (node->right != nullptr)
				{
					MoveToLeftMost(node->right);
				}

				current = path.IsEmpty() ? nullptr : path.Top();
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator temp = *this;
				++(*this);
				return temp;
			}

			bool operator==(const Iterator& other) const
			{
				return current == other.current;
			}

			bool operator!=(const Iterator& other) const
			{
				return current != other.current;
			}
		};


		Iterator Begin()
		{
			return Iterator(root);
		}

		Iterator End()
		{
			return Iterator();
		}

		private:
			void printInorder(Node* node, std::ostream& os) const
			{
				if (node != nullptr)
				{
					printInorder(node->left, os);
					os << "[" << node->key << ": " << node->value << "] ";
					printInorder(node->right, os);
				}
			}
		public:
			friend std::ostream& operator<<(std::ostream& os, const BinaryTree& tree) {
				os << "{ ";
				tree.printInorder(tree.root, os);
				os << "}";
				return os; 
			}
};

