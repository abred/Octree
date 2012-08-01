#ifndef __OCTREEE_H__
#define __OCTREEE_H__


struct Node
{
	Node* mParent;
	
	Node* mChildren[8];
	
	int mIndex;
	
	Node(Node* parent , Node* children[8] ,int index):
		mParent(parent),
		mChildren({children[0], children[1], children[2], children[3], children[4], children[5], children[6], children[7]}),
		mIndex(index)	
	{}
	
	Node():
		mParent(nullptr),
		mChildren(nullptr),
		mIndex(-1)
	{}

};

class Octree
{
	public:
		Octree ();
		~Octree ();

	private:
			
};



#endif /* __OCTREEE_H__ */

