/* This file is generated from ./emit/genemits.pl... */

DR_NS_BEGIN


bool Emitter0_g::emitAND_g(void *r)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot0_g *slot = (Eslot0_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r))
			return false;
	}
	return ret;
}

bool Emitter0_g::emitOR_g(void *r)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot0_g *slot = (Eslot0_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r))
			return true;
	}
	return ret;
}

bool Emitter1_g::emitAND_g(void *r, const void *a1)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot1_g *slot = (Eslot1_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1))
			return false;
	}
	return ret;
}

bool Emitter1_g::emitOR_g(void *r, const void *a1)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot1_g *slot = (Eslot1_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1))
			return true;
	}
	return ret;
}

bool Emitter2_g::emitAND_g(void *r, const void *a1, const void *a2)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot2_g *slot = (Eslot2_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2))
			return false;
	}
	return ret;
}

bool Emitter2_g::emitOR_g(void *r, const void *a1, const void *a2)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot2_g *slot = (Eslot2_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2))
			return true;
	}
	return ret;
}

bool Emitter3_g::emitAND_g(void *r, const void *a1, const void *a2, const void *a3)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot3_g *slot = (Eslot3_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2, a3))
			return false;
	}
	return ret;
}

bool Emitter3_g::emitOR_g(void *r, const void *a1, const void *a2, const void *a3)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot3_g *slot = (Eslot3_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2, a3))
			return true;
	}
	return ret;
}

bool Emitter4_g::emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot4_g *slot = (Eslot4_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2, a3, a4))
			return false;
	}
	return ret;
}

bool Emitter4_g::emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot4_g *slot = (Eslot4_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2, a3, a4))
			return true;
	}
	return ret;
}

bool Emitter5_g::emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot5_g *slot = (Eslot5_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2, a3, a4, a5))
			return false;
	}
	return ret;
}

bool Emitter5_g::emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot5_g *slot = (Eslot5_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2, a3, a4, a5))
			return true;
	}
	return ret;
}

bool Emitter6_g::emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot6_g *slot = (Eslot6_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2, a3, a4, a5, a6))
			return false;
	}
	return ret;
}

bool Emitter6_g::emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot6_g *slot = (Eslot6_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2, a3, a4, a5, a6))
			return true;
	}
	return ret;
}

bool Emitter7_g::emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot7_g *slot = (Eslot7_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2, a3, a4, a5, a6, a7))
			return false;
	}
	return ret;
}

bool Emitter7_g::emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot7_g *slot = (Eslot7_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2, a3, a4, a5, a6, a7))
			return true;
	}
	return ret;
}

bool Emitter8_g::emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7, const void *a8)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot8_g *slot = (Eslot8_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r, a1, a2, a3, a4, a5, a6, a7, a8))
			return false;
	}
	return ret;
}

bool Emitter8_g::emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7, const void *a8)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot8_g *slot = (Eslot8_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r, a1, a2, a3, a4, a5, a6, a7, a8))
			return true;
	}
	return ret;
}


DR_NS_END
