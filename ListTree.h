#pragma once


template <class T> class Node
{
public:
	char name[5];
	T mValue; //the object information
	
	Node<T>* pNext; //pointer to the next node element

	Node() {
		strcpy(name, "NODE");
		pNext = NULL;		
	};
	~Node()
	{		
	}
	//Methods omitted for brevity
};

// Single linked list
template<typename T> class CLinkList
{
protected:
	Node<T> *mpRootNode;
	int GetCount(Node<T> *base_node);
	
public:
	~CLinkList() { DeleteAllNode(); };
	CLinkList<T>(void) { mpRootNode = NULL; };
	Node<T> *GetHeaderNode() { return mpRootNode; };
	Node<T> *AddToTail(Node<T> *newNode);
	Node<T> *GetItem(int index);
	Node<T> *GetTail(Node<T> *cur);
	int GetCount(void) { return GetCount(mpRootNode); }
	int DeleteAllNode();
	int DeleteNode(Node<T> *base_node);
	int InsertNode(Node<T> *base_node, Node<T> *in_node);
	
};




template<typename T>  Node<T> *CLinkList<T>::GetItem(int index)
{
	Node<T> *temp_node;
	int count = 0;
	temp_node = mpRootNode;
	while (1)
	{
		if (!temp_node->pNext || index == count)
			break;
		count++;
		temp_node = temp_node->pNext;
	}
	return temp_node;
}


template<typename T> int CLinkList<T>::DeleteNode(Node<T> *base_node)
{
	Node<T> *temp_node;
	Node<T> *next_node;

	temp_node = base_node;
	next_node = temp_node;
	while (1)
	{
		if (next_node)
		{
			next_node = temp_node->pNext;
			if (mpRootNode == temp_node)
			{
				mpRootNode = next_node;
			}
		}
		else
		{
			if (mpRootNode == temp_node)
			{
				mpRootNode = NULL;
			}
		}
		//break;
		delete temp_node;
		if (!next_node)
			break;
		temp_node = next_node;
	}
	return 0;
}

template<typename T> int CLinkList<T>::DeleteAllNode()
{
	DeleteNode(mpRootNode);
	mpRootNode = NULL;
	return 0;
}



//Get List Counter
template<typename T> int CLinkList<T>::GetCount(Node<T> *base_node)
{
	Node<T> *temp_node;
	int count = 0;
	temp_node = base_node;
	if (!base_node)
		return 0;
	while (1)
	{
		count++;
		if (!temp_node->pNext)
			break;
		temp_node = temp_node->pNext;
	}
	return count;
}

template<typename T>  int CLinkList<T>::InsertNode(Node<T> *base_node, Node<T> *in_node)
{

	Node<T> *temp_node;
	int count = 0;

	if (base_node == NULL)
	{
		if (mpRootNode)
		{
			base_node = mpRootNode;
		}
		else
		{
			mpRootNode = in_node;
			return 0;
		}
	}
	temp_node = base_node;
	while (1)
	{
		if (!temp_node->pNext)
			break;
		count++;
		temp_node = temp_node->pNext;
	}
	temp_node->pNext = in_node;
	return 1;
}



// Returns the last node of the list 
template<typename T>  Node<T> *CLinkList<T>::GetTail(Node<T> *pCur_Node)
{
	while (pCur_Node != NULL && pCur_Node->pNext != NULL)
		pCur_Node = pCur_Node->pNext;
	return pCur_Node;
}

template<typename T>  Node<T> *CLinkList<T>::AddToTail(Node<T> *newNode)
{
	Node<T> *pTail = GetTail(mpRootNode);
	if (pTail == NULL && mpRootNode == NULL)
	{
		mpRootNode = newNode;
		return newNode;
	}
	else
	{
		pTail->pNext = newNode;
		newNode->pNext = NULL;
		return newNode;
	}
}