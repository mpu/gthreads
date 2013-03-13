#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum {
	MaxGThreads = 4,
	StackSize = 0x400000,
};

struct gt {
	struct gtctx {
		uint64_t rsp;
		uint64_t r15;
		uint64_t r14;
		uint64_t r13;
		uint64_t r12;
		uint64_t rbx;
		uint64_t rbp;
	} ctx;
	enum {
		Unused,
		Running,
		Ready,
	} st;
};

struct gt gttbl[MaxGThreads];
struct gt *gtcur;

void gtinit(void);
void gtret(int ret);
void gtswtch(struct gtctx *old, struct gtctx *new);
bool gtyield(void);
static void gtstop(void);
int gtgo(void (*f)(void));

void
gtinit(void)
{
	gtcur = &gttbl[0];
	gtcur->st = Running;
}

void __attribute__((noreturn))
gtret(int ret)
{
	if (gtcur != &gttbl[0]) {
		gtcur->st = Unused;
		gtyield();
		assert(!"reachable");
	}
	while (gtyield())
		;
	exit(ret);
}

bool
gtyield(void)
{
	struct gt *p;
	struct gtctx *old, *new;

	p = gtcur;
	while (p->st != Ready) {
		if (++p == &gttbl[MaxGThreads])
			p = &gttbl[0];
		if (p == gtcur)
			return false;
	}

	if (gtcur->st != Unused)
		gtcur->st = Ready;
	p->st = Running;
	old = &gtcur->ctx;
	new = &p->ctx;
	gtcur = p;
	gtswtch(old, new);
	return true;
}

static void
gtstop(void) { gtret(0); }

int
gtgo(void (*f)(void))
{
	char *stack;
	struct gt *p;

	for (p = &gttbl[0];; p++)
		if (p == &gttbl[MaxGThreads])
			return -1;
		else if (p->st == Unused)
			break;

	stack = malloc(StackSize);
	if (!stack)
		return -1;

	*(uint64_t *)&stack[StackSize -  8] = (uint64_t)gtstop;
	*(uint64_t *)&stack[StackSize - 16] = (uint64_t)f;
	p->ctx.rsp = (uint64_t)&stack[StackSize - 16];
	p->st = Ready;

	return 0;
}


/* Now, let's run some simple threaded code. */

void
f(void)
{
	static int x;
	int i, id;

	id = ++x;
	for (i = 0; i < 10; i++) {
		printf("%d %d\n", id, i);
		gtyield();
	}
}

int
main(void)
{
	gtinit();
	gtgo(f);
	gtgo(f);
	gtret(1);
}
