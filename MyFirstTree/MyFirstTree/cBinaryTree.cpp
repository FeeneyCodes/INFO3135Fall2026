#include "cBinaryTree.h"
#include <iostream>

void cBinaryTree::PrintBreadth(void)
{
	// Start with root node
	if (this->pRoot == NULL)
	{
		return;
	};
	// Call recursive method
	this->m_PrintBreadth(this->pRoot, 1);
	return;
}

void cBinaryTree::m_PrintBreadth(sNode*& pTheNode, int curDepth)
{
	// 1. Print Left
	if (pTheNode->pLeft != NULL)
	{
		this->m_PrintBreadth(pTheNode->pLeft, curDepth + 1);
	}
	// 2. Print me
	std::cout << pTheNode->data << std::endl;

	// 3. Print Right
	if (pTheNode->pRight != NULL)
	{
		this->m_PrintBreadth(pTheNode->pRight, curDepth + 1);
	}
	return;
}

void cBinaryTree::PrintDepth(void)
{
	// Start with root node
	if (this->pRoot == NULL)
	{
		return;
	};
	// Call recursive method
	this->m_PrintDepth(this->pRoot, 1);
	return;
}


void cBinaryTree::m_PrintDepth(sNode*& pTheNode, int curDepth)
{
	// 1. Print me
	std::cout << pTheNode->data << std::endl;

	// 2. Print Left
	if (pTheNode->pLeft != NULL)
	{
		this->m_PrintBreadth(pTheNode->pLeft, curDepth + 1);
	}

	// 3. Print Right
	if (pTheNode->pRight != NULL)
	{
		this->m_PrintBreadth(pTheNode->pRight, curDepth + 1);
	}
	return;
}



void cBinaryTree::AddData(std::string newData)
{
	// Is the tree empty?
	if (this->pRoot == nullptr)
	{	// Yes. This is the 1st node
		this->pRoot = new sNode();
		this->pRoot->data = newData;

		return;
	}
	// Call the recursive method
	this->m_AddData(this->pRoot, newData, 0);

	return;
}

void cBinaryTree::m_AddData(sNode* &pTheNode, 
	                        std::string theData, 
	                        int curDepth)
{
	// Is node null
	if (pTheNode == nullptr)
	{
		pTheNode = new sNode();
		pTheNode->data = theData;
		// Store depth
		pTheNode->depth = curDepth;
		return;
	}

	curDepth++;

	if (curDepth > this->m_DeepestLevelAdded)
	{
		this->m_DeepestLevelAdded = curDepth;
	}

	// Is is LT or GT the data at this node?
	if (theData < pTheNode->data)
	{	// Add to left node
		this->m_AddData(pTheNode->pLeft, theData, curDepth);
	}
	else
	{	// Add to right node
		this->m_AddData(pTheNode->pRight, theData, curDepth);
	}
	return;
}


// Returns NULL if NOT found
sNode* cBinaryTree::pFindNode(std::string theData)
{
	return this->m_pFindNode(this->pRoot, theData, 0);
}

sNode* cBinaryTree::m_pFindNode(sNode*& pTheNode, std::string theData, int curDepth)
{
	// Is this a "leaf" (i.e. NULL) node?
	if (pTheNode == nullptr)
	{
		return nullptr;
	}

	// Is it this node?
	if (pTheNode->data == theData)
	{
		return pTheNode;
	}

	curDepth++;
	// Save this value (how deep we went)
	this->m_DeepestLevel = curDepth;

	// Which side is it?
	if (theData < pTheNode->data)
	{	// Check the left node
		return this->m_pFindNode(pTheNode->pLeft, theData, curDepth);
	}
	else
	{	// Check the right node
		return this->m_pFindNode(pTheNode->pRight, theData, curDepth);
	}

	// This shouldn't happen...
	return nullptr;
}