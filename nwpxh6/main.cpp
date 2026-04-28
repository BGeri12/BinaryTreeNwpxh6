#include <iostream>
#include "BinaryTree.h"

int main()
{
	BinaryTree<int, std::string> tree;
	tree.Insert(40, "Hello");
	tree.Insert(20, "World");
	tree.Insert(60, "Binary");
	tree.Insert(12, "Tree");
	tree.Insert(25, "Example");

	try
	{
		std::cout << "Value for key 40: " << tree[40] << std::endl;
		std::cout << "Value for key 20: " << tree[20] << std::endl;
		std::cout << "Value for key 60: " << tree[60] << std::endl;
		std::cout << "Value for key 12: " << tree[12] << std::endl;
		std::cout << "Value for key 25: " << tree[25] << std::endl;
		std::cout << "Contains key 30? " << (tree.Contains(30) ? "Yes" : "No") << std::endl;
		std::cout << "Contains key 20? " << (tree.Contains(20) ? "Yes" : "No") << std::endl;
		std::cout << "Removing key 20..." << std::endl;
		tree.Remove(20);
		std::cout << "Contains key 20 after removal? " << (tree.Contains(20) ? "Yes" : "No") << std::endl;
		
		std::cout << "Binary search tree content (Inorder)" << std::endl;
		for (auto it = tree.Begin(); it != tree.End(); ++it)
		{
			std::cout << "Key: " << it.GetKey() << ", Value: " << *it << std::endl;
		}

		tree.Clear();
		std::cout << "Is the tree empty after clearing? " << (tree.Empty() ? "Yes" : "No") << std::endl;
	
		std::cout << "Binary search tree content (Inorder)" << std::endl;
		for (auto it = tree.Begin(); it != tree.End(); ++it)
		{
			std::cout << "Key: " << it.GetKey() << ", Value: " << *it << std::endl;
		}
	}
	catch (const KeyNotFoundException& e)
	{
		std::cerr << e.what() << std::endl;
	}

	
}