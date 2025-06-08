#include "pipeline.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    FilterFunc func;
    void* context;
} FilterEntry;

struct Pipeline {
    FilterEntry* filters;
    size_t count;
    size_t capacity;
    pthread_mutex_t lock;
};

Pipeline* pipeline_create(size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 4;

    Pipeline* p = malloc(sizeof(Pipeline));
    if (!p) return NULL;

    p->filters = calloc(initial_capacity, sizeof(FilterEntry));
    if (!p->filters) {
        free(p);
        return NULL;
    }

    p->count = 0;
    p->capacity = initial_capacity;

    if (pthread_mutex_init(&p->lock, NULL) != 0) {
        free(p->filters);
        free(p);
        return NULL;
    }

    return p;
}

int pipeline_add_filter(Pipeline* p, FilterFunc func, void* context) {
    if (!p || !func) return -1;

    pthread_mutex_lock(&p->lock);

    if (p->count == p->capacity) {
        size_t new_capacity = p->capacity * 2;
        FilterEntry* resized = realloc(p->filters, new_capacity * sizeof(FilterEntry));
        if (!resized) {
            pthread_mutex_unlock(&p->lock);
            return -1;
        }
        p->filters = resized;
        p->capacity = new_capacity;
    }

    p->filters[p->count].func = func;
    p->filters[p->count].context = context;
    p->count++;

    pthread_mutex_unlock(&p->lock);
    return 0;
}

void pipeline_apply(Pipeline* p, PipelineData* input) {
    if (!p || !input) return;

    pthread_mutex_lock(&p->lock);
    for (size_t i = 0; i < p->count; ++i) {
        fprintf(stderr, "[LOG] Applying filter %zu\n", i);
        p->filters[i].func(input, p->filters[i].context);
    }
    pthread_mutex_unlock(&p->lock);
}

void pipeline_destroy(Pipeline* p) {
    if (!p) return;
    pthread_mutex_destroy(&p->lock);
    free(p->filters);
    free(p);
}
