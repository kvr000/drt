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

#include <dr/tenv/tenv.hxx>
#include <dr/tenv/TestIdObject.hxx>

DR_NS_USE
DR_TENV_NS_USE

#define TEST_ARRAY
#define TEST_LIST
#define TEST_HASH
#define TEST_AVL

#ifdef TEST_ARRAY
TENV_NS(array);
void test()
{
	{
		RArray<TestIdObject> ar;

		ar.appendNoref(new TestIdObject(4));
		ar.appendDoref(tref(new TestIdObject(9)));
		ar.appendDoref(tref(new TestIdObject(7)));
		ar.shrinkEnd(2);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}
TENV_NSE(array);
#endif

#ifdef TEST_LIST
TENV_NS(list);
void test()
{
	{
		RList<TestIdObject> list;
		list.accAppendingNew()->setNoref(new TestIdObject(3));
		list.append(tref(new TestIdObject(5)));
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}
TENV_NSE(list);
#endif

#ifdef TEST_HASH
TENV_NS(hash);
void test()
{
	{
		RHash<String, TestIdObject> hash;
		hash["bla"].setNoref(new TestIdObject(3));
		hash["sehy"].setDoref(tref(new TestIdObject(4)));
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}
TENV_NSE(hash);
#endif

#ifdef TEST_AVL
typedef SArray<int>		TestList;
typedef TAvl<int, int>		TestTree;
typedef TAvl<int, int>::Node	TestNode;

TENV_NS(avl)

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
		TENV_CHECK(node->refs[0]->parent == node);
		TENV_CHECK(node->refs[0]->direction == -1);
		lh = checkSubtree((TestNode *)node->refs[0]);
	}
	else {
		lh = 0;
	}
	if ((node->refs[2]) != NULL) {
		TENV_CHECK(node->refs[2]->parent == node);
		TENV_CHECK(node->refs[2]->direction == 1);
		rh = checkSubtree((TestNode *)node->refs[2]);
	}
	else {
		rh = 0;
	}
	TENV_CHECK(node->balance >= -1 && node->balance <= 1);
	TENV_CHECK(node->balance == rh-lh);
	return (lh > rh ? lh : rh)+1;
}

static void checkTree(TestTree *tree)
{
	size_t cnti = 0;
	for (TestTree::Node *i = tree->iterFirst(); i != NULL; i = tree->iterNext(i))
		cnti++;
	TENV_CHECK(cnti == tree->count());
	if (tree->iterTop())
		checkSubtree(tree->iterTop());
}

static void addValue(TestList *list, TestTree *tree, int value)
{
	list->append(value);
	TENV_CHECK(tree->create(value, value));
	checkTree(tree);
}

static void remValue(TestList *list, TestTree *tree, size_t idx)
{
	int value = list->operator[](idx);
	list->remove(idx);
	TENV_CHECK(tree->remove(value));
	checkTree(tree);
}

static void testBStrAvl()
{
	static const BString a_bstr("a");
	static const BString b_bstr("b");
	static const BString c_bstr("c");
	static const BString d_bstr("d");
	static const BString e_bstr("e");
	TAvl<BString, BString> tree;
	tree.create(a_bstr, a_bstr);
	tree.create(b_bstr, b_bstr);
	tree.create(c_bstr, c_bstr);
	tree.create(d_bstr, d_bstr);
	tree.create(e_bstr, e_bstr);

	TENV_CHECK(tree.accValue(b_bstr));
	TENV_CHECK(tree.accValue(d_bstr));
	TENV_CHECK(tree.accValue(a_bstr));
	TENV_CHECK(tree.iterFirst()->v == a_bstr);
	TENV_CHECK(tree.iterNext(tree.iterFirst())->v == b_bstr);
	TENV_CHECK(tree.iterNext(tree.iterNext(tree.iterFirst()))->v == c_bstr);
	TENV_CHECK(tree.iterNext(tree.iterNext(tree.iterNext(tree.iterFirst())))->v == d_bstr);
	TENV_CHECK(tree.iterNext(tree.iterNext(tree.iterNext(tree.iterNext(tree.iterFirst()))))->v == e_bstr);
}

static void testBStrAvlReal()
{
	static const BString s0("e");
	static const BString s1("r");
	static const BString s2("v");
	static const BString s3("m");
	static const BString s4("w");
	static const BString s5("i");
	static const BString s6("_");
	static const BString s7("_");
	TAvl<BString, BString> tree;
	tree.create(s0, s0);
	tree.create(s1, s1);
	tree.create(s2, s2);
	tree.create(s3, s3);
	tree.create(s4, s4);
	tree.create(s5, s5);
	tree.create(s6, s6);
	tree.create(s7, s7);

	TENV_CHECK(tree.count() == 7);
	TENV_CHECK(tree.accValue(s0));
	TENV_CHECK(tree.accValue(s3));
	TENV_CHECK(tree.accValue(s7));
	//TENV_CHECK(tree.iterFirst()->v == a_bstr);
	//TENV_CHECK(tree.iterNext(tree.iterFirst())->v == b_bstr);
	//TENV_CHECK(tree.iterNext(tree.iterNext(tree.iterFirst()))->v == c_bstr);
	//TENV_CHECK(tree.iterNext(tree.iterNext(tree.iterNext(tree.iterFirst())))->v == d_bstr);
	//TENV_CHECK(tree.iterNext(tree.iterNext(tree.iterNext(tree.iterNext(tree.iterFirst()))))->v == e_bstr);
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
				while (tree.find((k = rand())) != NULL) ;
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
	testBStrAvl();
	testBStrAvlReal();
}
TENV_NSE(avl)
#endif

int main()
{
	tenv_init();
#ifdef TEST_ARRAY
	TENV_RUN(array);
#endif
#ifdef TEST_LIST
	TENV_RUN(list);
#endif
#ifdef TEST_HASH
	TENV_RUN(hash);
#endif
#ifdef TEST_AVL
	TENV_RUN(avl);
#endif
	return 0;
}
