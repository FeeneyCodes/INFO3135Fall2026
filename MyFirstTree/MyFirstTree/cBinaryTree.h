//#pragma once
#ifndef _cBinaryTree_HG_
#define _cBinaryTree_HG_

#include <string>

struct sNode
{
	std::string data;	
	//
	int population = 0;
	std::string favFood;
	// .. and other things, maybe
	//sNode(std::string theData)
	//{
	//	this->data = theData;
	//}
	sNode* pLeft = nullptr;
	sNode* pRight = nullptr;
	// 
	int depth = -1; 

	//sNode* pNodes[5];

	int indexLeft = 0;
	int indexRight = 0;

};

class cBinaryTree
{
public:
	sNode* pRoot = nullptr;
	// Insert into binary tree
	void AddData(std::string newData);
	// Returns NULL if NOT found
	sNode* pFindNode(std::string theData);

	int m_DeepestLevel = 0;

	int m_DeepestLevelAdded = 0;

	void PrintBreadth(void);

	void PrintDepth(void);

private:
	// This will be recursive
	void m_AddData(sNode* &pTheNode, std::string theData, int curDepth);
	// 
	sNode* m_pFindNode(sNode* &pTheNode, std::string theData, int curDepth);

	void m_PrintBreadth(sNode*& pTheNode, int curDepth);

	void m_PrintDepth(sNode*& pTheNode, int curDepth);

};

#endif // !_cBinaryTree_HG_
