/**
 * drt library
 *
 * drt multiplatform development toolkit
 *
 * ----------------------------------------------------------------------------------
 *
 * Copyright (C) 2004-2008 Zbyněk Vyškovský
 *
 * ----------------------------------------------------------------------------------
 *
 * LICENSE:
 *
 * This file is part of drt
 *
 * drt is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * drt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with drt; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @copyright	2004-2008 Zbyněk Vyškovský
 * @link	mailto:kvr@matfyz.cz
 * @link	http://kvr.matfyz.cz/drt/
 * @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
 **/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <dr/Array.hxx>
#include <dr/List.hxx>
#include <dr/Hash.hxx>
#include <dr/Avl.hxx>

#include <dr/testenv/testenv.hxx>
#include <dr/testenv/TestObject.hxx>

DR_NS_USE
DR_TESTENV_NS_USE

#define TEST_ARRAY
#define TEST_LIST
#define TEST_HASH
#define TEST_AVL

#ifdef TEST_ARRAY
TESTNS(array);
void test()
{
	{
		RArray<TestObject> ar;

		ar.appendNoref(new TestObject(4));
		ar.appendDoref(tref(new TestObject(9)));
		ar.appendDoref(tref(new TestObject(7)));
		ar.shrinkEnd(2);
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(array);
#endif

#ifdef TEST_LIST
TESTNS(list);
void test()
{
	{
		RList<TestObject> list;
		list.accAppendingNew()->setNoref(new TestObject(3));
		list.append(tref(new TestObject(5)));
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(list);
#endif

#ifdef TEST_HASH
TESTNS(hash);
void test()
{
	{
		RHash<String, TestObject> hash;
		hash["bla"].setNoref(new TestObject(3));
		hash["sehy"].setDoref(tref(new TestObject(4)));
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(hash);
#endif

#ifdef TEST_AVL
typedef SArray<int>		TestList;
typedef TAvl<int, int>		TestTree;
typedef TAvl<int, int>::Node	TestNode;

TESTNS(avl)

static bool dumpLevel(TestNode *node, unsigned level, unsigned req_level, int *pos)
{
	static int lwidth[10] = { 33, 22, 14, 9, 6, 4, 2 };
	if (level == req_level) {
		if (node) {
			*pos -= printf("%*d", *pos, node->v);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		bool found = false;
		*pos -= level >= sizeof(lwidth)/sizeof(lwidth[0]) ? 1 : lwidth[level];
		found |= dumpLevel(node ? (TestNode *)node->lchild : NULL, level+1, req_level, pos);
		*pos += 1+2*(level >= sizeof(lwidth)/sizeof(lwidth[0]) ? 1 : lwidth[level]);
		found |= dumpLevel(node ? (TestNode *)node->rchild : NULL, level+1, req_level, pos);
		*pos -= 1+level >= sizeof(lwidth)/sizeof(lwidth[0]) ? 1 : lwidth[level];
		return found;
	}
}

static void dumpTree(TestTree *tree)
{
	int pos;
	for (unsigned level = 0; dumpLevel(tree->iterTop(), 0, level, &(pos = 64)); level++)
		puts("");
}

static int checkSubtree(TestNode *node)
{
	int lh, rh;
	if ((node->refs[0]) != NULL) {
		CHECK(node->refs[0]->parent == node);
		CHECK(node->refs[0]->direction == -1);
		lh = checkSubtree((TestNode *)node->refs[0]);
	}
	else {
		lh = 0;
	}
	if ((node->refs[2]) != NULL) {
		CHECK(node->refs[2]->parent == node);
		CHECK(node->refs[2]->direction == 1);
		rh = checkSubtree((TestNode *)node->refs[2]);
	}
	else {
		rh = 0;
	}
	CHECK(node->balance >= -1 && node->balance <= 1);
	CHECK(node->balance == rh-lh);
	return (lh > rh ? lh : rh)+1;
}

static void checkTree(TestTree *tree)
{
	size_t cnti = 0;
	for (TestTree::Node *i = tree->iterFirst(); i != NULL; i = tree->iterNext(i))
		cnti++;
	CHECK(cnti == tree->getCount());
	if (tree->iterTop())
		checkSubtree(tree->iterTop());
}

static void addValue(TestList *list, TestTree *tree, int value)
{
	list->append(value);
	CHECK(tree->create(value, value));
	checkTree(tree);
}

static void remValue(TestList *list, TestTree *tree, size_t idx)
{
	int value = list->operator[](idx);
	list->remove(idx);
	CHECK(tree->remove(value));
	checkTree(tree);
}

void test()
{
	TAvl<int, int> tree;
	SArray<int> added;
	if (0) {
		addValue(&added, &tree, 5);
		addValue(&added, &tree, 1);
		addValue(&added, &tree, 2);
		addValue(&added, &tree, 7);
		addValue(&added, &tree, 6);
		addValue(&added, &tree, 4);
		addValue(&added, &tree, 3);
		dumpTree(&tree);
		remValue(&added, &tree, 1);
	}
	else {
		for (int r = 0; r < 8; r++) {
			for (int i = 0; i < 64; i++) {
				int k;
				while (tree.find((k = rand())) != NULL);
				//printf("adding %d\n", k);
				addValue(&added, &tree, k);
			}
			while (added.count() != 0) {
				int idx = rand()%added.count();
				//printf("remove %d\n", added[idx]);
				remValue(&added, &tree, idx);
			}
		}
	}
}
TESTNSE(avl)
#endif

int main() { test_init();
#ifdef TEST_ARRAY
	TESTRUN(array);
#endif
#ifdef TEST_LIST
	TESTRUN(list);
#endif
#ifdef TEST_HASH
	TESTRUN(hash);
#endif
#ifdef TEST_AVL
	TESTRUN(avl);
#endif
	return 0;
}
