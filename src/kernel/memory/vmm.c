#include "vmm.h"


typedef struct _allocationNode {        
    uint32_t *begin;      
    uint32_t *end;       
    uint32_t is_free;     
    struct _allocationNode *prev;
    struct _allocationNode *next;
} allocationNode;


typedef struct _vmm_context {
    uint32_t pd_phys;    
    allocationNode *allocs;   
} vmmContext;

// extern void load_page_directory(uint32_t phys_addr);










vmmContext* _sys_vmm_create_context() {
    vmmContext* ctx = (vmmContext*) _sys_pmm_alloc_page(1);
    ctx->pd_phys =(uint32_t) _sys_create_pageDirectory();
    void* alloc_ll_page=_sys_pmm_alloc_page(1);
    ctx->allocs = NULL;
    return ctx;
}


void* _sys_vmm_alloc(vmmContext* ctx, uint32_t size) {
    if (size == 0) return NULL;

    if(ctx->allocs=NULL){

    }
}



void _sys_vmm_switch_context(vmmContext *ctx) {
    if (!ctx) return;
    _sys_load_page_directory(ctx->pd_phys);
}