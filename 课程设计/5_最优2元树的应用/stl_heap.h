#ifndef _MY_STL_HEAP
#define _MY_STL_HEAP
#include "stl_vector.h"
#include "stl_functional.h"

//插入
template<class Iter, class Compare>
void pushHeap(Iter beg, Iter end, Compare cmp)
{
	if (end - 1 <= beg)
		return;
	siftFix(beg, end, cmp);
}
template<class Iter>
void pushHeap(Iter beg, Iter end)
{
	typedef typename Iter::value_type value_type;

	if (end - 1 <= beg)
		return;
	siftFix(beg, end, Less<value_type>());
}
template<class Iter, class Compare>
void siftFix(Iter beg, Iter end, Compare cmp)
{
	typedef typename Iter::value_type value_type;

	//新加入的元素的值
	value_type val = *(end - 1);
	//要处理的hole的位置
	int hole = (end - 1) - beg;
	//hole的父节点
	int parent = (hole - 1) / 2;
	//上溯 结束 条件 hole越界 or 不满足cmp关系
	while (hole >= 0 && hole != parent && cmp( *(beg + parent), val))
	{
		//父节点的值下来
		*(beg + hole) = *(beg + parent);
		//hole上去
		hole = parent;
		parent = (hole - 1) / 2;
	}
	//hole上去到能上去的最高位置
	*(beg + hole) = val;
}



//将最顶上的元素放到最后
template<class Iter>
void popHeap(Iter beg, Iter end)
{
	typedef typename Iter::value_type value_type;

	if (end - 1 <= beg)
		return;
	sinkFix(beg, end, Less<value_type>());
}
template<class Iter, class Compare>
void popHeap(Iter beg, Iter end, Compare cmp)
{
	if (end - 1 <= beg)
		return;
	sinkFix(beg, end, cmp);
}
template<class Iter, class Compare>
void sinkFix(Iter beg, Iter end, Compare cmp)
{
	typedef typename Iter::value_type value_type;

	//要处理的元素的值
	value_type val = *beg;
	int hole = 0;
	int leftNode = hole * 2 + 1;
	int rightNode = leftNode + 1;
	int len = end - beg;
	//寻找最后一个元素要去的hole，因为第一个元素要占用
	while (rightNode < len)
	{
		//选中符合条件的那个孩子交换，hole下沉
		if (cmp(*(beg + rightNode), *(beg + leftNode)))
		{
			*(beg + hole) = *(beg + leftNode);
			hole = leftNode;
		}
		else
		{
			*(beg + hole) = *(beg + rightNode);
			hole = rightNode;
		}
		leftNode = hole * 2 + 1;
		rightNode = leftNode + 1;
	}
	//还有一个单独的左孩子，继续hole下沉
	if (leftNode < len)
	{
		*(beg + hole) = *(beg + leftNode);
		hole = leftNode;
	}
	//最后一个位置要被val占用，
	*(beg + hole) = *(end - 1);
	//下来会打乱顺序，再进行上溯
	siftFix(beg, beg + hole + 1, cmp);
	*(end - 1) = val;
}

#endif // !_MY_STL_HEAP
