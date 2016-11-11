//
//  gthr.h
//  
//
//  Created by fred on 11/9/16.
//
//

#ifndef ____gthr__
#define ____gthr__

#include <stdio.h>
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
class gthread{
    private:
        struct gt gttbl[MaxGThreads];
        struct gt *gtcur;
    public:
        void gtinit(void);
        void gtret(int ret);
        void gtswtch(struct gtctx *old, struct gtctx *new);
        bool gtyield(void);
        static void gtstop(void);
        int gtgo(void (*f)(void));
};

#endif /* defined(____gthr__) */



